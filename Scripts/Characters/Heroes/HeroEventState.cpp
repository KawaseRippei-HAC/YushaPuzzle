#include "Characters/Heroes/HeroEventState.h"

#include "Tweens/Tween.h"
#include "Interfaces/ITileEvent.h"
#include "Utilities/StateMachine.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Events/EncounterEvent.h"
#include "Events/EncountMover.h"

/*実装*/
// 更新
void HeroEventState::on_update(float delta_time, HeroContext& context) {
	// イベントが無ければ待機状態へ遷移
	if (!event_) {
		change_state(HeroState::Idle);
		return;
	}

	// イベントが終了したら状態を変更
	if (!event_->is_finished()) {
		// イベントを実行
		event_->execute(delta_time, *context.character); return;
	}

	// 中心に到達しておらず、中心に移動すべき場合、中心まで移動する
	if (!is_centering_ && should_move_to_center(context)) {
		const GSvector3 position = context.character->transform().position();
		const GSvector3 center = context.current_tile->world_position();
		const GSvector3 distance = center - position;
		const float multiplier = distance.length() / (TILE_SIZE * 0.5f);
		const float centering_time = HERO_DEFAULT_TURN_SPEED * multiplier;

		Tween::start<GSvector3>(position, center, centering_time,
			[&](const GSvector3& position) { context.character->transform().position(position); }
		).on_complate(
			[this, &context, center]() {
				// 座標を強制補正
				context.character->transform().position(center);
				is_centering_ = false;
			}
		);
		is_centering_ = true;
		context.character->mesh().change_motion(HeroMotion::Walk);
	}

	// 中心に到達したら状態遷移する
	if (!is_centering_) {
		// 勇者が死亡していたら死亡状態へ遷移
		if (context.character->is_dead()) change_state(HeroState::Dead);
		// イベントが移動を妨害しなければ移動状態へ遷移
		else if (!event_->is_impede_movement()) change_state(HeroState::Move);
		// それ以外なら待機状態へ遷移
		else change_state(HeroState::Idle);
	}
}

// 遷移時処理
void HeroEventState::on_enter(HeroContext& context) {
	// 処理するイベントを取得
	event_ = context.current_tile->event();
	// 開始時処理があれば実行
	if (event_) event_->on_enter(*context.character);
}

// 終了時処理
void HeroEventState::on_exit(HeroContext& context) {
	// 終了時処理があれば実行
	if (event_) event_->on_exit(*context.character);
	// イベントへの参照を手放す
	this->event_ = nullptr;

	this->IHeroState::on_exit(context);
}

// 中心まで移動すべきか
bool HeroEventState::should_move_to_center(const HeroContext& context) const noexcept {
	const GSvector3 position = context.character->transform().position();
	const GSvector3 center = context.current_tile->world_position();
	return (center - position).length() > 0.001f;
}