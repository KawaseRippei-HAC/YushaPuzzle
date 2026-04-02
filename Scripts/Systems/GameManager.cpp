#include "Systems/GameManager.h"

#include <format>
#include <sstream>

#include <GSeffect.h>

#include "Cameras/DraggableCamera.h"
#include "Cameras/HeroFollowCamera.h"
#include "Characters/Monsters/DemonKingTable.h"

/*定義*/
// フェーズ終了ボタン位置
#define PHASE_END_BUTTON_POSITION GSvector2{ 960.0f - 64.0f, 940.0f - 64.0f}
// フェーズ終了ボタン範囲
#define PHASE_END_BUTTON_AREA GSvector2{ 128.0f, 128.0f }
// フェーズ終了ボタン間隔
#define PHASE_END_BUTTON_SPACE GSvector2{ 256.0f - 64.0f, 256.0f - 64.0f }

// 手札入れ替えボタン範囲
#define HAND_REPLACEMENT_BUTTON_AREA GSvector2{ 128.0f, 128.0f }
// 手札入れ替えボタン間隔
#define HAND_REPLACEMENT_BUTTON_SPACE GSvector2{ (256.0f - HAND_REPLACEMENT_BUTTON_AREA.x) * 0.5f, (256.0f - HAND_REPLACEMENT_BUTTON_AREA.y) * 0.5f }

// カメラ初期位置
#define CAMERA_INITIAL_POSITION GSvector3{ -12.0f, 17.0f, -17.0f }
// カメラ初期注視点
#define CAMERA_INITIAL_LOOKAT GSvector3{ -12.0f, 0.0f, 0.0f }
// カメラオフセット
#define CAMERA_OFFSET GSvector3{ 0.0f, 17.0f, -17.0f }

/*定数*/
// カード設置フェーズ時間
static constexpr float CARD_PLACEMENT_PHASE_TIME{ 60.0f * 60.0f };

// 数字テクスチャ
static const NumberTexture NUMBER_TEXTURE{ Texture::Number, 128, 128 };
// タイマー数字位置
static const GSvector2 TIMER_NUM_POSITION{ 500.0f, 885.0f };
// タイマー色
static const GScolor TIMER_COLOR{ GSvector3::zero(), 1.0f };
// タイマー数字スケール
static const GSvector2 TIMER_NUM_SCALE{ GSvector2::one() * 0.3f };
// タイマー数字間隔
static constexpr float TIMER_NUM_SPACE{ -10.0f };

// 手札入れ替えボタン位置
static const GSvector2 HAND_REPLACEMENT_BUTTON_POSITION{ 0.0f, 600.0f };
// チャージ数字位置
static const GSvector2 HAND_REPLACEMENT_CHARGE_NUM_POSITION{
	HAND_REPLACEMENT_BUTTON_POSITION.x + 256.0f * 0.5f - (64.0f * 0.75f),
	HAND_REPLACEMENT_BUTTON_POSITION.y + 256.0f * 0.5f - (64.0f * 0.75f) };
// チャージ色
static const GScolor HAND_REPLACEMENT_CHARGE_COLOR{ GSvector3::one(), 1.0f };
// チャージ数字スケール
static const GSvector2 HAND_REPLACEMENT_CHARGE_NUM_SCALE{ GSvector2::one() * 0.75f };

// カメラ補間時間
static constexpr float CAMERA_INTERPOLATION_TIME{ 60.0f };

// 手札入れ替え可能ターン数
static constexpr int HAND_REPLACABLE_COUNT{ 5 };

/*実装*/
// 更新
void GameManager::update(float delta_time) {
	tile_manager_->update(delta_time);
	card_manager_->update(delta_time);

	if (is_in_player_phase_) this->update_cards_placement_phase(delta_time);
	else this->update_heroes_action_phase(delta_time);

	hero_character_->update(delta_time);
	demon_king_character_->update(delta_time);
	cinemachine_->update(delta_time);

	// エフェクトの更新
	card_place_effect_.update(delta_time);
	gsUpdateEffect(delta_time);

	// 仮で魔王が死亡したらゲームクリア
	GameManager::is_game_clear_ = (demon_king_character_) ? demon_king_character_->is_dead() : false;
	GameManager::is_game_over_ = (hero_character_) ? hero_character_->is_dead() : false;
}

// 描画
void GameManager::draw() const {
	gsDrawSkyboxCubemap(Texture::Skybox);
	cinemachine_->draw();
	gsSetEffectCamera();
	tile_manager_->draw();
	hero_character_->draw();
	placeable_highlight_->draw();
	gsDrawEffect();
}

// GUI描画
void GameManager::draw_gui() const {
	card_manager_->draw_gui();
	hero_character_->draw_gui();
	if (is_in_player_phase_) phase_end_button_->draw();
	this->draw_timer();
	this->draw_hand_replacement_button();
}

// 初期化
void GameManager::initialize(const std::string& stage_path) {
	// フラグの初期化
	is_in_player_phase_ = true;
	GameManager::is_game_clear_ = false;
	GameManager::is_game_over_ = false;

	// TODO: 初期化周りのリファクタリング
	event_pool_ = std::make_unique<EventPool>();
	event_pool_->load("Assets/Data/event.txt");
	monster_pool_ = std::make_unique<MonsterPool>();
	monster_pool_->load("Assets/Data/monster.txt");
	mid_boss_pool_ = std::make_unique<MidBossPool>();
	mid_boss_pool_->load("Assets/Data/mid_boss.txt");
	demon_king_character_.reset(DemonKingTable::create(0));

	tile_manager_ = std::make_unique<TileManager>(stage_path);
	tile_manager_->load_map(stage_path, *event_pool_, *monster_pool_, *mid_boss_pool_);
	tile_manager_->make_tip_tiles(*demon_king_character_);

	card_manager_ = std::make_unique<CardManager>();
	card_manager_->load_deck("Assets/Data/deck.csv", *event_pool_, *monster_pool_);

	placeable_highlight_ = std::make_unique<PlaceableHighlight>(*tile_manager_);

	phase_end_button_ = std::make_unique<ButtonWidget>(
		PHASE_END_BUTTON_POSITION, PHASE_END_BUTTON_AREA, Texture::HeroMoveButton, PHASE_END_BUTTON_SPACE);
	phase_end_button_->attach_click([this]() { is_in_player_phase_ = false; });

	hand_replacable_charge_ = HAND_REPLACABLE_COUNT;
	hand_replacement_button_ = std::make_unique<ButtonWidget>(HAND_REPLACEMENT_BUTTON_POSITION + HAND_REPLACEMENT_BUTTON_SPACE,
		HAND_REPLACEMENT_BUTTON_AREA, Texture::HandReplacementButton, HAND_REPLACEMENT_BUTTON_SPACE);
	hand_replacement_button_->attach_click([this]() {
		hand_replacable_charge_ = 0;
		card_manager_->hand_replacement(); }
	);

	hero_character_ = std::make_unique<HeroCharacter>(tile_manager_->graph());

	cinemachine_ = std::make_unique<Cinemachine>();
	cinemachine_->set<DraggableCamera>(CAMERA_INITIAL_POSITION, CAMERA_INITIAL_LOOKAT);

	// カード設置フェーズの開始
	phase_timer_ = CARD_PLACEMENT_PHASE_TIME;
}

// 消去
void GameManager::clear() {
	// 明示的にポインタを開放
	cinemachine_.reset(nullptr);
	hand_replacement_button_.reset(nullptr);
	phase_end_button_.reset(nullptr);
	placeable_highlight_.reset(nullptr);
	demon_king_character_.reset(nullptr);
	hero_character_.reset(nullptr);
	card_manager_.reset(nullptr);
	tile_manager_.reset(nullptr);
	event_pool_.reset(nullptr);
}

// ゲームクリアフラグ
bool GameManager::is_game_clear() noexcept {
	return is_game_clear_;
}

// ゲームオーバーフラグ
bool GameManager::is_game_over() noexcept {
	return is_game_over_;
}

// 勇者キャラクターの取得
HeroCharacter* GameManager::get_hero() const noexcept {
	return hero_character_.get();
}

// プレイヤーフェーズ開始処理
void GameManager::start_player_phase() {
	is_in_player_phase_ = true;
	card_manager_->operable(true);
	card_manager_->hand_replenishment();
	phase_timer_ = CARD_PLACEMENT_PHASE_TIME;
	hand_replacable_charge_ = std::min(hand_replacable_charge_ + 1, HAND_REPLACABLE_COUNT);
	// カメラをドラッグ可能カメラに変更
	const GSvector3 position = hero_character_->transform().position();
	cinemachine_->change<DraggableCamera>(CAMERA_INTERPOLATION_TIME, position + CAMERA_OFFSET, position);
}

// 勇者行動フェーズ開始処理
void GameManager::start_hero_phase() {
	is_in_player_phase_ = false;
	card_manager_->operable(false);
	hero_character_->think_start();
	hero_character_->move_start();
	placeable_highlight_->visible(false);
	// カメラを勇者追従カメラに変更
	cinemachine_->change<HeroFollowCamera>(CAMERA_INTERPOLATION_TIME, *hero_character_);

	// カードの使用数に応じて魔王がレベルアップ
	demon_king_character_->level_up_execute(card_manager_->used_count());
}

// カード設置フェーズの更新
void GameManager::update_cards_placement_phase(float delta_time) {
	// 手札入れ替えボタンの更新
	if (hand_replacable_charge_ >= HAND_REPLACABLE_COUNT)
		hand_replacement_button_->update(delta_time);

	// タイマーの減算
	phase_timer_ = std::max(phase_timer_ - delta_time, 0.0f);
	// 制限時間になったら勇者行動フェーズを開始してリターン
	if (phase_timer_ <= 0.0f) { this->start_hero_phase(); return; }

	// フェーズ終了ボタンの更新
	phase_end_button_->update(delta_time);
	// カード設置フェーズが終了していたら勇者行動フェーズを開始してリターン
	if (!is_in_player_phase_) { this->start_hero_phase(); return; }

	// 設置可能ハイライトの更新
	placeable_highlight_->update(delta_time);

	if (TileData* data = card_manager_->dragging_card_data()) {
		// 設置可能ハイライトの判定
		placeable_highlight_->visible(true);
		placeable_highlight_->judge_placeable(*data); return;
	}
	// カードがドロップされたら
	else if (TileData* data = card_manager_->request_replace_tile()) {
		// タイルの設置を試みて、設置できなければリターン
		placeable_highlight_->visible(false);
		if (!this->try_replace_tile(data)) return;
	}
	else return;

	// 勇者の思考状態を開始
	hero_character_->think_start();

	// 手札のカード一枚も置けなければカード設置フェーズを終了
	is_in_player_phase_ = card_manager_->can_placeable_hands(tile_manager_->graph());

	// カード設置フェーズが終了していたら勇者行動フェーズを開始
	if (!is_in_player_phase_) this->start_hero_phase();
}

// 勇者行動フェーズの更新
void GameManager::update_heroes_action_phase(float delta_time) {
	is_in_player_phase_ = hero_character_->is_action_ended();
	if (is_in_player_phase_) this->start_player_phase();
}

// タイル置換の試行
bool GameManager::try_replace_tile(TileData* data) {
	// マウス座標からタイル座標系を計算
	const auto [x, z] = placeable_highlight_->highlighted_coord();
	// タイルが設置できなければリターン
	if (!tile_manager_->try_replace_tile(x, z, *data)) return false;
	// カードを使用済みにする
	card_manager_->apply_used_card();
	// 設置エフェクトの再生
	card_place_effect_.play(Effect::CardPlacement)
		.position(tile_manager_->calculate_world_position(x, z));
	gsPlaySE(SE::TilePlacement);
	return true;
}

// タイマーの描画
void GameManager::draw_timer() const {
	std::string timer_text{ "--" };
	if (is_in_player_phase_) {
		const int seccond{ static_cast<int>(phase_timer_ / 60.0f) };
		timer_text = std::format("{:02}", seccond);
	}
	NUMBER_TEXTURE.draw(
		TIMER_NUM_POSITION,
		timer_text,
		TIMER_COLOR,
		TIMER_NUM_SCALE,
		TIMER_NUM_SPACE
	);
}

// 手札入れ替えボタンの描画
void GameManager::draw_hand_replacement_button() const {
	if (!is_in_player_phase_) return;

	if (hand_replacable_charge_ >= HAND_REPLACABLE_COUNT) {
		hand_replacement_button_->draw(); return;
	}

	static const GScolor color{ GSvector3::one(), 0.5f };
	gsDrawSprite2D(
		Texture::HandReplacementButtonBack,
		&HAND_REPLACEMENT_BUTTON_POSITION,
		NULL, NULL, &color, NULL, 0.0f
	);

	NUMBER_TEXTURE.draw(
		HAND_REPLACEMENT_CHARGE_NUM_POSITION,
		std::format(
			"{}", HAND_REPLACABLE_COUNT - hand_replacable_charge_
		),
		HAND_REPLACEMENT_CHARGE_COLOR,
		HAND_REPLACEMENT_CHARGE_NUM_SCALE
	);
}