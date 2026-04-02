#include "Screens/RuleExplanationScreen.h"
#include "Screens/PauseScreen.h"
#include "Screens/OptionScreen.h"

#include "Defines/Assets.h"

/* --- ボタン共通 --- */
// ボタンの間
static constexpr float SPACE{ 150 };

// ボタンの基準位置（戻るボタンの位置）
static const GSvector2 BACK_BTN_POS{ WINDOW_CENTER.x * 0.55f, 900 };
static constexpr int BACK_BTN_SPACE{ 550 };
/* --- オプション画面に戻る --- */
static const GSvector2 BACK_GAME_BTN_SIZE = { BTN_FONT_SIZE_57 * 4, BTN_FONT_SIZE_57 };
static const GSvector2 BACK_GAME_BTN_POS = { BACK_BTN_POS.x - BACK_GAME_BTN_SIZE.x * 1.0f + BACK_BTN_SPACE, BACK_BTN_POS.y - BACK_GAME_BTN_SIZE.y * 0.5f };
static const GSvector2 BACK_GAME_BTN_SPACE = { (BTN_TEXTURE_SIZE_512 - BACK_GAME_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - BACK_GAME_BTN_SIZE.y) * 0.5f };

// コンストラクタ
RuleExplanationScreen::RuleExplanationScreen() {

	// オプション画面に戻るボタンの設定
	ButtonWidget* back_btn = new ButtonWidget{ BACK_GAME_BTN_POS, BACK_GAME_BTN_SIZE, Texture::BackSettings, BACK_GAME_BTN_SPACE };
	back_btn->attach_click([=]() {ScreenManager::pop_screen(); });

	// ボタンの登録
	button_manager_.add_button(back_btn);
}
// デストラクタ
RuleExplanationScreen::~RuleExplanationScreen() {

}

// 初期化
void RuleExplanationScreen::initialize() {

}
// 更新
void RuleExplanationScreen::update(float delta_time) {
	button_manager_.update(delta_time);

	if (gsGetKeyTrigger(PAUSE_KEY)) {

	}
}
// 描画
void RuleExplanationScreen::draw(void) const {

	// BG
	gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);

	button_manager_.draw();
	gsFontParameter(GS_FONT_NORMAL, 64, "MSゴシック");
	const GSvector2 Size{ 1920 / 10, 1080 / 4 };

	const GSvector2 pos = { 60 , -30 };
	gsDrawSprite2D(Texture::RuleExplanationBG, &pos, NULL, NULL, NULL, NULL, NULL);
}
