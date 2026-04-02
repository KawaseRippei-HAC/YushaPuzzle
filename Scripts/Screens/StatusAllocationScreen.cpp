#include "Screens/StatusAllocationScreen.h"
#include "Screens/OptionAssets.h"
#include "Skills/SkillTable.h"

// 位置
static const GSvector2 BACK_BTN_POS{ WINDOW_CENTER.x, 900 };
// 追加で上がる値
static constexpr int ADD_PARAMATER{ 2 };

/* --- ステータス --- */
// ステータス数
static constexpr int STATUS_NUM{ 5 };
// ステータス名と数値の間
static constexpr float STATUS_SPACE{ 500 };
// ステータスの描画位置の補正
static const GSfloat STATUS_POS_CORRECTION{ 75.0f };
// ステータスの基準位置（左上の座標 数値の参照用）
static const GSvector2 STATUS_POS{ 250, 350 + STATUS_POS_CORRECTION };
// ステータスの表示位置
static const GSvector2 STATUS_SCREEN_POS{ (1920 - 2048) * 0.5f, (1080 - 2048) * 0.5f + STATUS_POS_CORRECTION };
// 数字の文字サイズ
static constexpr int FONT_SIZE{ 64 };
// 数値と数値の上下の空間
static constexpr float SPACE{ 125 };

/* --- スキル --- */
static const GSvector2 SKILL_BTN_SIZE{ 128, 128 };	// ボタンサイズ
static const GSvector2 SKILL_BTN_SPACE{ (BTN_TEXTURE_SIZE_256 - SKILL_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_256 - SKILL_BTN_SIZE.y) * 0.5f };	// テクスチャ内のボタンまでのスペース
// 基準位置
static const GSvector2 SKILL_POS{ 1165 + 20 , 300 + 20 + STATUS_POS_CORRECTION };
// スキルとスキルの間隔
static const GSvector2 SKILL_SPACE{ 130, 130 };
// 横一列のスキルの数
static constexpr int SKILL_COUNT{ 4 };

/* --- ステータス振り分け --- */
// 位置
static const GSvector2 STATUS_ALOOCATION_POS{ 500, 200 };
// 中心
static const GSvector2 STATUS_ALOOCATION_CENTER{ 1024 * 0.5f, 1024 * 0.5f };

/* --- ステータスポイント --- */
// 位置
static const GSvector2 STATUS_POINT_POS{ 355, 300 };
// 中心
static const GSvector2 STATUS_POINT_CENTER{ 512 * 0.5f, 512 * 0.5f };
// カラー
static const GScolor STATUS_POINT_COLOR{ 0.5f, 0.5f, 1.0f, 1.0f };

// ステータスアップボタンの位置
static const GSvector2 STATUS_BTN_POS{ 900, 200 + 75 };
// ステータスボタンのサイズ
static const GSvector2 STATUS_BTN_SIZE{ 50, 50 };

/* --- ステータスポイント関連 --- */
// ステータスポイントの位置
static const GSvector2 SP_POS{ 600, 300 - FONT_SIZE  * 0.5f};
// 文字サイズ
static constexpr float FONT_SCALE{ 0.5f };
// 数字と数字の間隔
static constexpr int NUM_SPACE{ 25 };

StatusAllocationScreen::StatusAllocationScreen(HeroCharacter* hero) :
	hero_{ hero } {
}
StatusAllocationScreen::~StatusAllocationScreen() {

}

// 初期化
void StatusAllocationScreen::initialize() {
	button_manager_.clear();

	// --振り分けボタンを設定--
	GSvector2 status_btn_pos = STATUS_BTN_POS;
	for (int i = 0; i < 5; ++i) {
		status_btn_pos.y += SPACE;
		ButtonWidget* status_btn = new ButtonWidget{ status_btn_pos, STATUS_BTN_SIZE, Texture::UpBox };
		status_btn->attach_click([=]() { status_up(i); });
		status_btn->attach_hover([=]() { skill_up_id_ = i; });
		// ボタンの登録
		button_manager_.add_button(status_btn);
	}

	/* --- スキルボタン --- */
	// 勇者の所持スキルの取得
	hero_skill_state_ = hero_->skill_actor()->skills();
	// スキルの一覧にボタン追加
	for (size_t i = 0; i < hero_skill_state_.size(); ++i) {
		// スキル情報を取得
		const SkillDefinition* skill = SkillTable::get_skill_data(hero_skill_state_[i].id);

		// スキルボタンの登録
		ButtonWidget* skill_btn = new ButtonWidget{ GSvector2{ SKILL_POS.x + (i % SKILL_COUNT) * SKILL_SPACE.x, SKILL_POS.y + (i / SKILL_COUNT) * SKILL_SPACE.y},
			SKILL_BTN_SIZE, skill->texture, SKILL_BTN_SPACE };
		skill_btn->attach_click([=]() {});
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
void StatusAllocationScreen::update(float delta_time) {
	skill_up_id_ = -1;
	is_pop_up_ = false;
	button_manager_.update(delta_time);
}
// 描画
void StatusAllocationScreen::draw(void) const {
	// BG
	gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);

	// ステータス振り分け
	gsDrawSprite2D(Texture::StatusAllocation, &STATUS_ALOOCATION_POS, NULL, &STATUS_ALOOCATION_CENTER, NULL, NULL, NULL);

	// ステータスポイント
	gsDrawSprite2D(Texture::StatusPoint, &STATUS_POINT_POS, NULL, &STATUS_POINT_CENTER, &STATUS_POINT_COLOR, NULL, NULL);

	// ステータスの描画
	gsDrawSprite2D(Texture::StatusScreen, &STATUS_SCREEN_POS, NULL, NULL, NULL, NULL, NULL);

	// ステータスの数値を描画
	for (int i = 0; i < STATUS_NUM; i++) {
		draw_status(i);
	}

	// ステータスポイント取得
	const std::string sp_str = std::to_string(hero_->status_point());
	// ステータスポイントの数値
	number_texture_.draw(SP_POS, sp_str, STATUS_POINT_COLOR, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);

	// ボタンの描画
	button_manager_.draw();

	// スキルレベル描画
	draw_skill_level();

	// ステータスの上昇値を描画
	status_up_draw(skill_up_id_);

	// ポップアップ描画
	if (is_pop_up_) skill_pop_up();
}

void StatusAllocationScreen::status_up(int num) {
	switch (num)
	{
	case 0: hero_->add_max_hp(ADD_PARAMATER * 10); break;
	case 1: hero_->add_attack(ADD_PARAMATER); break;
	case 2: hero_->add_defense(ADD_PARAMATER); break;
	case 3: hero_->add_speed(ADD_PARAMATER); break;
	case 4: hero_->add_luck(ADD_PARAMATER); break;
	}
	hero_->add_status_point(-1);

	// ステータスポイントが無くなったら終了
	if (hero_->status_point() <= 0) {
		ScreenManager::pop_screen();
	}
}

// ステータス描画
void StatusAllocationScreen::draw_status(int num) const {
	std::string str;
	switch (num) {
	case 0: str += std::to_string(hero_->hp()) + "/" + std::to_string(hero_->max_hp()); break;
	case 1: str += std::to_string(hero_->attack()); break;
	case 2: str += std::to_string(hero_->defense()); break;
	case 3: str += std::to_string(hero_->speed()); break;
	case 4: str += std::to_string(hero_->luck()); break;
	default: break;
	}
	GSfloat len = str.length();
	GSvector2 pos = { STATUS_POS.x + STATUS_SPACE, STATUS_POS.y + SPACE * num };
	pos -= { len* FONT_SIZE - (len - 1) * 25, FONT_SIZE * 0.5f };
	number_texture_.draw(pos, str, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);
}

// ステータスの上昇値の描画
void StatusAllocationScreen::status_up_draw(int num) const {
	if (skill_up_id_ <= -1) return;
	// 位置
	GSvector2 pos{ STATUS_POS.x + STATUS_SPACE, STATUS_POS.y + SPACE * num - (NUMBER_SIZE * 0.25f) };
	const int add_paramater = (num == 0) ? ADD_PARAMATER * 10 : ADD_PARAMATER;
	// 上昇値
	std::string str = "+" + std::to_string(add_paramater);

	number_texture_.draw(pos, str, STATUS_POINT_COLOR, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);
	//gsDrawSpriteFont(&pos, str.c_str());
}

// スキルレベル描画
void StatusAllocationScreen::draw_skill_level(void) const {
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
void StatusAllocationScreen::skill_pop_up(void) const {
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
