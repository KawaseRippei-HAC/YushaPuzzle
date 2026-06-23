#include "Scenes/TitleScene/TitleScene.h"

#include "UI/ButtonWidget.h"
#include "Screens/OptionAssets.h"
#include "Screens/OptionScreen.h"
#include "Systems/AssetLoader.h"
#include "Systems/Mouse.h"

/* --- 背景 --- */
static const GSvector2 UI_BANNER_SIZE{ 1625, 1119 };

/* --- タイトルイラスト --- */
static const GSvector2 ILLUSTRATION_CENTER{ 1024 * 0.5f, 512 * 0.5f };
static const GSvector2 ILLUSTRATION_POS{ 1490, 845 };

/* --- タイトルロゴ --- */
static const GSvector2 LOGO_CENTER{ 2048 * 0.5f, 1024 * 0.5f };
static const GSvector2 LOGO_POS{ 1920 * 0.5f + 20, 1080 * 0.5f - 150 };


// ボタンの基準位置（中央）
static const GSvector2 BTN_POS{ 420, 680 };
static constexpr int BTN_SPACE{ 140 };

/*実装*/
// 開始
void TitleScene::start() {
	//	スタートボタンの設定
	GSvector2 btn_size{ BTN_FONT_SIZE_57 * 7, BTN_FONT_SIZE_57 };	// ボタンサイズ
	GSvector2 btn_pos{ BTN_POS - btn_size * 0.5f };			// ボタン位置
	GSvector2 btn_space{ (BTN_TEXTURE_SIZE_512 - btn_size.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - btn_size.y) * 0.5f };	// テクスチャ内のボタンまでのスペース
	ButtonWidget* start_btn = new ButtonWidget{ btn_pos, btn_size, Texture::GameStart, btn_space };
	start_btn->attach_click([=]() { this->is_end_ = true; });

	// 設定ボタンの設定
	btn_size = { BTN_FONT_SIZE_57 * 2, BTN_FONT_SIZE_57 };
	btn_pos = { BTN_POS - btn_size * 0.5f };
	btn_pos.y += BTN_SPACE;
	btn_space = { (BTN_TEXTURE_SIZE_512 - btn_size.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - btn_size.y) * 0.5f };
	ButtonWidget* option_btn = new ButtonWidget{ btn_pos, btn_size, Texture::Setting, btn_space };
	option_btn->attach_click([=]() {ScreenManager::push_screen(ScreenID::SoundOption); });

	// 終了設定ボタンの設定
	btn_size = { BTN_FONT_SIZE_57 * 5, BTN_FONT_SIZE_57 };
	btn_pos = { BTN_POS - btn_size * 0.5f };
	btn_pos.y += BTN_SPACE * 2;
	btn_space = { (BTN_TEXTURE_SIZE_512 - btn_size.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - btn_size.y) * 0.5f };
	ButtonWidget* end_btn = new ButtonWidget{ btn_pos, btn_size, Texture::GameEnd, btn_space };
	end_btn->attach_click([=]() { is_game_end_ = true; });

	// ボタンの登録（コンストラクタと観測者を設定してから）
	button_manager_.add_button(start_btn);
	button_manager_.add_button(option_btn);
	button_manager_.add_button(end_btn);

	ScreenManager::register_screen(ScreenID::SoundOption, new OptionScreen{});

	gsPlayBGM(BGM::Title);
}

// 更新
void TitleScene::update(float delta_time) {
	// マウスの更新
	Mouse::update(delta_time);

	// ポーズ画面じゃない時
	if (ScreenManager::is_empty()) {
		button_manager_.update(delta_time);
	}

	if (gsGetKeyTrigger(GKEY_SPACE)) {
		this->is_end_ = true;
	}

	// スクリーンの更新
	ScreenManager::update(delta_time);
}

// 描画
void TitleScene::draw() const {
	// BG
	GSvector2 scale{ WINDOW_SIZE.x / UI_BANNER_SIZE.x, WINDOW_SIZE.y / UI_BANNER_SIZE.y };
	gsDrawSprite2D(Texture::Banner, NULL, NULL, NULL, NULL, &scale, NULL);

	// イラスト
	gsDrawSprite2D(Texture::TitleIllustration, &ILLUSTRATION_POS, NULL, &ILLUSTRATION_CENTER, NULL, NULL, NULL);

	// ロゴ
	gsDrawSprite2D(Texture::TitleLogo, &LOGO_POS, NULL, &LOGO_CENTER, NULL, NULL, NULL);

	button_manager_.draw();

	// スクリーンの描画
	ScreenManager::draw();
	// マウスカーソルの描画
	Mouse::draw();
}

// 終了
void TitleScene::end() {
	button_manager_.clear();
	ScreenManager::clear();

	this->is_end_ = false;
}