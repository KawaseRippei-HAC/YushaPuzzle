#include "Characters/Heroes/HeroMoveState.h"

#include "Characters/Heroes/HeroCharacter.h"
#include "Characters/Heroes/HeroDefaultMover.h"

/*外部変数*/
// 通常移動処理
static HeroDefaultMover default_mover;

/*実装*/
// 更新時処理
void HeroMoveState::on_update(float delta_time, HeroContext& context) {
	// 移動処理
	(mover_->*on_move_)(delta_time, *context.character);

	// 移動中なら以降を無視
	if (!mover_->is_completed()) return;
	// 移動が完了したら終了処理を実行
	this->move_end(context);

	// パスが無くなったら以降を無視
	if (is_event_active_ || context.path.empty()) return;
	// 再び移動を開始
	this->move_start(context);
}

// 遷移時処理
void HeroMoveState::on_enter(HeroContext& context) {
	// パスが無ければ待機状態へ遷移
	if (context.path.empty()) {
		change_state(HeroState::Idle);
		return;
	}

	// 移動開始
	this->move_start(context);
}

// 移動の開始
void HeroMoveState::move_start(HeroContext& context) {
	// モーションの変更
	context.character->mesh().change_motion(HeroMotion::Walk);
	const float speed = context.character->current_move_speed_multiplier();
	context.character->mesh().motion_speed(speed);

	if (is_in_approach_) start_approach(context);
	else start_departure(context);
}

// 移動の終了
void HeroMoveState::move_end(HeroContext& context) {
	if (is_in_approach_) end_approach(context);
	else end_departure(context);

	context.character->mesh().motion_speed(1.0f);
}

// 接近の開始
void HeroMoveState::start_approach(HeroContext& context) {
	// 目的地への移動処理を取得する
	mover_ = get_mover(destination_tile_, context.path.empty(), *context.character);
	// 移動処理の関数ポインタを設定
	on_move_ = &ICharacterMover::on_approach;

	const GSvector3 from = context.character->transform().position();
	const GSvector3 to = destination_tile_->world_position();

	// 移動処理の初期化
	mover_->init_approach(from, to);
}

// 接近の終了
void HeroMoveState::end_approach(HeroContext& context) {
	// 接近終了
	is_in_approach_ = false;

	// パスをポップ
	context.path.pop();
	// 現在のタイルを更新
	context.current_tile = destination_tile_;
	// 通常移動処理なら座標を補正
	if (mover_ == &default_mover)
		context.character->transform().position(destination_tile_->world_position());

	// 『移動処理が通常移動処理ではない≒処理すべきイベントがある』
	// なので、イベント状態へ遷移
	if (is_event_active_) change_state(HeroState::Event);
	// イベントはないが、パスが空なら思考状態へ遷移
	else if (context.path.empty()) change_state(HeroState::Idle);
}

// 離脱の開始
void HeroMoveState::start_departure(HeroContext& context) {
	is_event_active_ = false;

	// 移動処理が無ければ、現在のタイルの移動処理を取得
	if (!mover_) mover_ = get_mover(context.current_tile, false, *context.character);
	// 移動処理の関数ポインタを設定
	on_move_ = &ICharacterMover::on_departure;

	// 目的地タイルの取得
	destination_tile_ = context.path.top();

	const GSvector3 from = context.current_tile->world_position();
	const GSvector3 path = destination_tile_->world_position() - from;
	transit_position_ = from + (path * 0.5f);

	// 移動処理の初期化
	mover_->init_departure(from, transit_position_);
}

// 離脱の終了
void HeroMoveState::end_departure(HeroContext& context) {
	// 経由座標で座標の補正
	context.character->transform().position(transit_position_);
	// 接近開始
	is_in_approach_ = true;
}

// 移動処理の取得
ICharacterMover* HeroMoveState::get_mover(TileNode* tile, bool is_path_empty, HeroCharacter& charcter) noexcept {
	// HACK: タイルが無ければ通常移動処理を返しているが、
	// ランタイムエラーとかにした方が良いかもしれない
	if (!tile) return &default_mover;

	// イベントの取得
	ITileEvent* event = tile->event();
	// イベントがあり、かつ移動中に有効であるかパスが無ければイベントを有効化
	is_event_active_ = event && (event->is_activate_in_moving(charcter) || is_path_empty);
	// イベントが有効ならイベントの移動処理を、そうでなければ通常移動処理を取得
	ICharacterMover* mover = (is_event_active_) ? event->mover() : &default_mover;
	// 有効なイベントでも移動処理を持たない可能性があるので、移動処理がなければ通常移動処理を返す
	return (mover) ? mover : &default_mover;
}