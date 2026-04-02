#include "Screens/StatusScreen.h"

#include "Defines/Assets.h"
#include "Screens/OptionAssets.h"
#include "Skills/SkillTable.h"
#include "Data/ResultData.h"
#include "Systems/SkillSystem.h"
#include "Skills/SkillActor.h"

// 数値と数値の上下の空間
static constexpr float SPACE{ 125 };

// 音量設定の位置
static const GSvector2 HERO_STATUS_POS{ 450 - BTN_TEXTURE_SIZE_1024 * 0.5f, 150 - BTN_TEXTURE_SIZE_1024 * 0.5f };

// スクリーンの高さ補正
static constexpr float OFFSET_Y{ 30.0f };

/* --- 戻るボタン --- */
static const GSvector2 BACK_BTN_SIZE{ BTN_FONT_SIZE_57 * 5, BTN_FONT_SIZE_57 };	// ボタンサイズ
static const GSvector2 BACK_BTN_POS{ WINDOW_CENTER.x - BACK_BTN_SIZE.x * 0.5f, 900 - BACK_BTN_SIZE.y * 0.5f };			// ボタン位置
static const GSvector2 BACK_BTN_SPACE{ (BTN_TEXTURE_SIZE_512 - BACK_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - BACK_BTN_SIZE.y) * 0.5f };	// テクスチャ内のボタンまでのスペース

/* --- ステータス --- */
// ステータス数
static constexpr int STATUS_NUM{ 5 };
// ステータス名と数値の間
static constexpr float STATUS_SPACE{ 500 };
// ステータスの基準位置（左上の座標 数値の参照用）
static const GSvector2 STATUS_POS{ 250, 350 + OFFSET_Y };
// ステータスの表示位置
static const GSvector2 STATUS_SCREEN_POS{ (1920 - 2048) * 0.5f , (1080 - 2048) * 0.5f + OFFSET_Y };

/* --- スキル --- */
static const GSvector2 SKILL_BTN_SIZE{ 128, 128 };	// ボタンサイズ
static const GSvector2 SKILL_BTN_SPACE{ (BTN_TEXTURE_SIZE_256 - SKILL_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_256 - SKILL_BTN_SIZE.y) * 0.5f };	// テクスチャ内のボタンまでのスペース
// 基準位置
static const GSvector2 SKILL_POS{ 1165 + 20, 300 + 20 + OFFSET_Y };
// スキルとスキルの間隔
static const GSvector2 SKILL_SPACE{ 130, 130 };
// 横一列のスキルの数
static constexpr int SKILL_COUNT{ 4 };

// 数字の文字サイズ
static constexpr int FONT_SIZE{ 64 };
// 数字のスケール
static constexpr float FONT_SCALE{ 0.5f };
static constexpr int NUM_SPACE{ 25 };

// 修正値がプラスの時のカラー
static const GScolor PLUS_MODIFIER_COLOR{ 1.0f, 1.0f, 0.0f, 1.0f };
// 修正値がマイナスの時のカラー
static const GScolor MINUS_MODIFIER_COLOR{ 1.0f, 0.0f, 0.0f, 1.0f };

// Lvのスケール
static const GSvector2 LEVEL_SCALE{ 0.1f, 0.1f };
static const GSvector2 LEVEL_POSITION{ STATUS_POS.x + 25, STATUS_POS.y - SPACE - 25 };

StatusScreen::StatusScreen(HeroCharacter* hero_parameter) :
	hero_character_(hero_parameter) {

	// プレイ中のステータス画面
	if (hero_character_ != nullptr) {
		is_result_status_ = false;
	}
	// リザルトのステータス画面
	else {
		is_result_status_ = true;

		// リザルト画面では、ステータスの変動が無いため先に計算する
		// パラメータを設定
		set_parameter(ResultData::level(), ResultData::hp(), ResultData::max_hp(), ResultData::attack(),
			ResultData::defense(), ResultData::speed(), ResultData::luck());

		// バフとデバフの値を取得
		SkillActor skill_acter;
		skill_acter.add_skills(ResultData::skills());
		modifier_ = skill_acter.non_battle_skill().buffs;

		// スキルの取得
		hero_skill_state_ = ResultData::skills();
	}
	
}
StatusScreen::~StatusScreen() {

}

// 初期化
void StatusScreen::initialize() {
	button_manager_.clear();
	// プレイ中のステータス画面
	if (!is_result_status_) {
		// パラメータを設定
		set_parameter(hero_character_->level(), hero_character_->hp(), hero_character_->max_hp(), hero_character_->attack(),
			hero_character_->defense(), hero_character_->speed(), hero_character_->luck());

		// バフとデバフの値を取得
		modifier_ = hero_character_->modifiers().get_modifiers();

		// スキルを取得
		hero_skill_state_ = hero_character_->skill_actor()->skills();
	}

	/* --- 戻るボタン --- */
	// 戻るボタン追加
	ButtonWidget* back_btn = new ButtonWidget{ BACK_BTN_POS, BACK_BTN_SIZE, Texture::BackSettings, BACK_BTN_SPACE };
	back_btn->attach_click([=]() {ScreenManager::pop_screen(); });
	// ボタンの登録
	button_manager_.add_button(back_btn);

	/* --- スキルボタン --- */
	// スキルの一覧にボタン追加
	for (size_t i = 0; i < hero_skill_state_.size(); ++i) {
		// スキル情報を取得
		const SkillDefinition* skill = SkillTable::get_skill_data(hero_skill_state_[i].id);

		// スキルボタンの登録
		ButtonWidget* skill_btn = new ButtonWidget{ GSvector2{ SKILL_POS.x + (i % SKILL_COUNT) * SKILL_SPACE.x, SKILL_POS.y + (i / SKILL_COUNT) * SKILL_SPACE.y},
			SKILL_BTN_SIZE, skill->texture, SKILL_BTN_SPACE };
		skill_btn->attach_click([=]() { });
		skill_btn->attach_hover([=]() {
			// ポップアップを表示
			is_pop_up_ = true;
			// スキル名
			skill_name_ = skill->name;
			// スキル説明
			skill_description_ = skill->description;
			});

		button_manager_.add_button(skill_btn);
	}

}
// 更新
void StatusScreen::update(float delta_time) {
	is_pop_up_ = false;
	button_manager_.update(delta_time);
}
// 描画
void StatusScreen::draw(void) const {
	// BG
	gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);

	// 勇者ステータス
	gsDrawSprite2D(Texture::HeroStatus, &HERO_STATUS_POS, NULL, NULL, NULL, NULL, NULL);

	// レベルの描画
	draw_level();

	// ステータスの描画
	gsDrawSprite2D(Texture::StatusScreen, &STATUS_SCREEN_POS, NULL, NULL, NULL, NULL, NULL);

	// ステータスの数値を描画
	for (int i = 0; i < STATUS_NUM; i++) {
		// パラメータの描画
		draw_status(i);

		// 修正値の描画
		draw_modifier(i);
	}

	button_manager_.draw();

	// スキルレベル描画
	draw_skill_level();

	// ポップアップ描画
	if (is_pop_up_) skill_pop_up();
}

// レベル描画
void StatusScreen::draw_level(void) const {
	// 文字の描画
	gsDrawSprite2D(Texture::Level, &LEVEL_POSITION, NULL, NULL, NULL, &LEVEL_SCALE, NULL);

	// 値の描画
	std::string str = std::to_string(paramater_.level);
	const GSfloat len = str.length();
	GSvector2 pos = { STATUS_POS.x + STATUS_SPACE - (len * FONT_SIZE - (len - 1) * 25),
		STATUS_POS.y - SPACE - (FONT_SIZE * 0.5f) };
	number_texture_.draw(pos, str, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);
}

// パラメータ表示用の文字列を設定
void StatusScreen::set_parameter(const level_t level, const point_t hp, const point_t max_hp,
	const attack_t attack, const defense_t defense, const speed_t speed, const luck_t luck) {
	paramater_ = { level, hp, max_hp, attack, defense, speed, luck };
}

// ステータス描画
void StatusScreen::draw_status(int num) const {
	// ステータスの数値を描画
	std::string str = "";
	switch (num) {
	case ParamaterState::HP: str = std::to_string(paramater_.hp) + "/" + std::to_string(paramater_.max_hp); break;
	case ParamaterState::Attack: str = std::to_string(paramater_.attack); break;
	case ParamaterState::Defense: str = std::to_string(paramater_.defense); break;
	case ParamaterState::Speed: str = std::to_string(paramater_.speed); break;
	case ParamaterState::Luck: str = std::to_string(paramater_.luck); break;
	default: return;
	}
	const GSfloat len = str.length();
	GSvector2 pos = { STATUS_POS.x + STATUS_SPACE, STATUS_POS.y + SPACE * num };
	pos -= { len* FONT_SIZE - (len - 1) * 25, FONT_SIZE * 0.5f };
	number_texture_.draw(pos, str, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);
}

// 修正値を描画（バフとデバフ）
void StatusScreen::draw_modifier(int num) const {
	int para = 0;
	switch (num) {
	case ParamaterState::HP: return;
	case ParamaterState::Attack: para = modifier_.attack; break;
	case ParamaterState::Defense: para = modifier_.defense; break;
	case ParamaterState::Speed: para = modifier_.speed; break;
	case ParamaterState::Luck: para = modifier_.luck; break;
	default: return;
	}

	// 0の時は描画しない
	if (para == 0) return;
	const bool is_plus = para > 0;
	std::string str = (is_plus) ? "+" : "";
	str += std::to_string(para);

	const GSvector2 pos = { STATUS_POS.x + STATUS_SPACE, STATUS_POS.y + (SPACE * num) - (FONT_SIZE * 0.5f)};
	const GScolor color = (is_plus) ? PLUS_MODIFIER_COLOR : MINUS_MODIFIER_COLOR;
	number_texture_.draw(pos, str, color, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);
}

// スキルレベル描画
void StatusScreen::draw_skill_level(void) const {
	for (size_t i = 0; i < hero_skill_state_.size(); ++i) {
		GSvector2 pos{ SKILL_POS.x + (i % SKILL_COUNT) * SKILL_SPACE.x + SKILL_LEVEL_SPACE.x, SKILL_POS.y + (i / SKILL_COUNT) * SKILL_SPACE.y + SKILL_LEVEL_SPACE.y };
		// 勇者のレベル取得
		const int skill_level = hero_skill_state_[i].level;
		// 左上
		const GSvector2 skill_level_tex_top_left{ (SKILL_LEVEL_TEXTURE_SPACE * skill_level) + (SKILL_LEVEL_TEX_SIZE * (skill_level - 1)), 0.0f };
		const GSrect skill_level_rect{ skill_level_tex_top_left.x, skill_level_tex_top_left.y,
			skill_level_tex_top_left.x + SKILL_LEVEL_TEX_SIZE, skill_level_tex_top_left.y + SKILL_LEVEL_TEX_SIZE };
		gsDrawSprite2D(Texture::SkillLevel, &pos, &skill_level_rect, NULL, NULL, &SKILL_LEVEL_SCALE, NULL);
	}
}

// スキル説明のポップアップ表示
void StatusScreen::skill_pop_up(void) const {
	// マウス位置取得
	int x, y;
	gsGetMouseCursorPosition(&x, &y);
	GSvector2 mouse_pos{ (GSfloat)x, (GSfloat)y };

	// フレーム描画
	GSrect rect{ mouse_pos.x - POP_UP_FRAME_SIZE.x, mouse_pos.y, mouse_pos.x, mouse_pos.y + POP_UP_FRAME_SIZE.y };
	gsDrawRectangle2D(&rect, &POP_UP_FRAME_COLOR);
	// ポップアップ背景描画
	rect = { rect.left + POP_UP_FRAME_WIDTH, rect.top + POP_UP_FRAME_WIDTH, rect.right - POP_UP_FRAME_WIDTH, rect.bottom - POP_UP_FRAME_WIDTH };
	gsDrawRectangle2D(&rect, &POP_UP_BG_COLOR);
	// スキル名描画
	mouse_pos += { -POP_UP_FRAME_SIZE.x + FRAME_NAME_SPACE, FRAME_NAME_SPACE };
	gsSetSpriteFontStyle(GS_FONT_NORMAL, POP_UP_SKILL_NAME_FONT_SIZE, "MSゴシック");
	gsDrawSpriteFont(&mouse_pos, skill_name_.c_str());
	// スキル説明描画
	mouse_pos.y += POP_UP_NAME_DESCRIPTION_SPACE;
	gsSetSpriteFontStyle(GS_FONT_NORMAL, POP_UP_SKILL_DESCRIPTION_FONT_SIZE, "MSゴシック");
	gsDrawSpriteFont(&mouse_pos, skill_description_.c_str());
}
