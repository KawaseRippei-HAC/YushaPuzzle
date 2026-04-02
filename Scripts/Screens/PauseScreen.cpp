#include "Screens/PauseScreen.h"
#include "Screens/OptionScreen.h"
#include "Screens/StatusScreen.h"
//#include "Screens/OptionAssets.h"

#include "Defines/Assets.h"

/* --- ボタン共通 --- */
// ボタンの間
static constexpr float SPACE{ 150 };

// 
static const GSvector2 BTN_POS{ WINDOW_CENTER.x, 300 };

/* --- 音量設定ボタン --- */
// ボタンの大きさ
static const GSvector2 SOUND_BTN_SIZE{ BTN_FONT_SIZE_90 * 4, BTN_FONT_SIZE_90 };
// 位置
static const GSvector2 SOUND_BTN_POS{ BTN_POS - SOUND_BTN_SIZE * 0.5f };
// テクスチャ内のボタンまでのスペース
static const GSvector2 SOUND_BTN_SPACE{ (BTN_TEXTURE_SIZE_1024 - SOUND_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_1024 - SOUND_BTN_SIZE.y) * 0.5f };
/* --- ステータスボタン --- */
// ボタンの大きさ
static const GSvector2 STATUS_BTN_SIZE{ BTN_FONT_SIZE_90 * 7, BTN_FONT_SIZE_90 };
// 位置
static const GSvector2 STATUS_BTN_POS{ WINDOW_CENTER.x - STATUS_BTN_SIZE.x * 0.5f, 450 };
// テクスチャ内のボタンまでのスペース
static const GSvector2 STATUS_BTN_SPACE{ (BTN_TEXTURE_SIZE_1024 - STATUS_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_1024 - STATUS_BTN_SIZE.y) * 0.5f };
/* --- ルール説明ボタン --- */
// ボタンの大きさ
static const GSvector2 EXPLANATION_BTN_SIZE{ BTN_FONT_SIZE_90 * 7, BTN_FONT_SIZE_90 };
// 位置
static const GSvector2 EXPLANATION_BTN_POS{ WINDOW_CENTER.x - EXPLANATION_BTN_SIZE.x * 0.5f, 645 };
// テクスチャ内のボタンまでのスペース
static const GSvector2 EXPLANATION_BTN_SPACE{ (BTN_TEXTURE_SIZE_1024 - EXPLANATION_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_1024 - EXPLANATION_BTN_SIZE.y) * 0.5f };

// ボタンの基準位置（戻るボタンの位置）
static const GSvector2 BACK_BTN_POS{ WINDOW_CENTER.x, 900 };
static constexpr int BACK_BTN_SPACE{ 550 };
/* --- ゲームに戻る --- */
static const GSvector2 BACK_GAME_BTN_SIZE{ BTN_FONT_SIZE_57 * 6, BTN_FONT_SIZE_57 };		// ボタンサイズ
static const GSvector2 BACK_GAME_BTN_POS{ BACK_BTN_POS - BACK_GAME_BTN_SIZE * 0.5f };	// ボタン位置
static const GSvector2 BACK_GAME_BTN_SPACE{ (BTN_TEXTURE_SIZE_512 - BACK_GAME_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - BACK_GAME_BTN_SIZE.y) * 0.5f };	// テクスチャ内のボタンまでのスペース
/* --- タイトルに戻る --- */
static const GSvector2 BACK_OPTION_BTN_SIZE = { BTN_FONT_SIZE_57 * 7, BTN_FONT_SIZE_57 };
static const GSvector2 BACK_OPTION_BTN_POS = { BACK_BTN_POS.x - BACK_OPTION_BTN_SIZE.x * 0.5f - BACK_BTN_SPACE, BACK_BTN_POS.y - BACK_OPTION_BTN_SIZE.y * 0.5f };
static const GSvector2 BACK_OPTION_BTN_SPACE = { (BTN_TEXTURE_SIZE_512 - BACK_OPTION_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - BACK_OPTION_BTN_SIZE.y) * 0.5f };
/* --- リトライ --- */
static const GSvector2 RETRY_BTN_SIZE = { BTN_FONT_SIZE_57 * 4, BTN_FONT_SIZE_57 };
static const GSvector2 RETRY_BTN_POS = { BACK_BTN_POS.x - RETRY_BTN_SIZE.x * 0.5f + BACK_BTN_SPACE, BACK_BTN_POS.y - RETRY_BTN_SIZE.y * 0.5f };
static const GSvector2 RETRY_BTN_SPACE = { (BTN_TEXTURE_SIZE_512 - RETRY_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - RETRY_BTN_SIZE.y) * 0.5f };

// コンストラクタ
PauseScreen::PauseScreen(bool* is_title, bool* is_retry) :
	is_title_(is_title), is_retry_(is_retry) {

	// 音量設定ボタンの設定
	ButtonWidget* option_btn = new ButtonWidget{ SOUND_BTN_POS, SOUND_BTN_SIZE, Texture::SoundSettingLogo, SOUND_BTN_SPACE };
	option_btn->attach_click([=]() {ScreenManager::push_screen(ScreenID::SoundOption); });

	// 勇者ステータスボタンの設定
	ButtonWidget* status_btn = new ButtonWidget{ STATUS_BTN_POS, STATUS_BTN_SIZE, Texture::HeroStatus, STATUS_BTN_SPACE };
	status_btn->attach_click([=]() {ScreenManager::push_screen(ScreenID::Status); });

	// ルール説明ボタンの設定
	ButtonWidget* explanation_btn = new ButtonWidget{ EXPLANATION_BTN_POS, EXPLANATION_BTN_SIZE, Texture::RuleExplanation, EXPLANATION_BTN_SPACE };
	explanation_btn->attach_click([=]() {ScreenManager::push_screen(ScreenID::RuleExplanation); });

	// ゲームに戻るボタンの設定
	ButtonWidget* back_btn = new ButtonWidget{ BACK_GAME_BTN_POS, BACK_GAME_BTN_SIZE, Texture::BackGamePlay, BACK_GAME_BTN_SPACE };
	back_btn->attach_click([=]() {ScreenManager::pop_screen(); });

	// タイトルに戻るボタンの設定
	ButtonWidget* Title_btn = new ButtonWidget{ BACK_OPTION_BTN_POS, BACK_OPTION_BTN_SIZE, Texture::BackTitle, BACK_OPTION_BTN_SPACE };
	Title_btn->attach_click([=]() { *is_title_ = true; });

	// リトライボタンの設定
	ButtonWidget* retry_btn = new ButtonWidget{ RETRY_BTN_POS, RETRY_BTN_SIZE, Texture::Retry, RETRY_BTN_SPACE };
	retry_btn->attach_click([=]() { *is_retry_ = true; });

	// ボタンの登録
	button_manager_.add_button(option_btn);
	button_manager_.add_button(status_btn);
	button_manager_.add_button(explanation_btn);
	button_manager_.add_button(back_btn);
	button_manager_.add_button(Title_btn);
	button_manager_.add_button(retry_btn);
}
// デストラクタ
PauseScreen::~PauseScreen() {

}

// 初期化
void PauseScreen::initialize() {

}
// 更新
void PauseScreen::update(float delta_time) {
	button_manager_.update(delta_time);

	if (gsGetKeyTrigger(PAUSE_KEY)) {

	}
}
// 描画
void PauseScreen::draw(void) const {

	// BG
	gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);

	button_manager_.draw();
	gsFontParameter(GS_FONT_NORMAL, 64, "MSゴシック");
	const GSvector2 Size{ 512 / 2, 512 / 2 };

	const GSvector2 pos = { 300, 200 };
	gsDrawSprite2D(Texture::PauseSetting, &pos, NULL, &Size, NULL, NULL, NULL);


}
