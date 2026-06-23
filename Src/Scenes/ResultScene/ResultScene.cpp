#include "Scenes/ResultScene/ResultScene.h"
#include "Defines/Assets.h"
#include "Systems/Mouse.h"
#include "Systems/AssetLoader.h"
#include "Systems/GameManager.h"
#include "Screens/OptionAssets.h"
#include "Screens/StatusScreen.h"
#include "Data/ResultData.h"

static const GSvector2 BG_SIZE{ 1625, 1119 };

/* --- ステータスボタン --- */
// ボタンの大きさ
static const GSvector2 STATUS_BTN_SIZE{ BTN_FONT_SIZE_90 * 7, BTN_FONT_SIZE_90 };
// 位置
static const GSvector2 STATUS_BTN_POS{ 270, 1080 * 0.5f + 50 };
// テクスチャ内のボタンまでのスペース
static const GSvector2 STATUS_BTN_SPACE{ (BTN_TEXTURE_SIZE_1024 - STATUS_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_1024 - STATUS_BTN_SIZE.y) * 0.5f };


// 下のボタンの位置（真ん中）
static const GSvector2 BACK_BTN_CENTER_POS{ 960, 900 };
static constexpr int SPACE{ 500 };
static constexpr int FONSTSIZE{ 57 };
static const GSvector2 TITLE_BTN_SIZE{ FONSTSIZE * 7, FONSTSIZE };
static const GSvector2 RETRY_BTN_SIZE{ FONSTSIZE * 4, FONSTSIZE };
static const GSvector2 STAGESELECT_BTN_SIZE{ FONSTSIZE * 8, FONSTSIZE };

// 画像サイズ
static constexpr int TEXTURE_SIZE{ 2048 };

static const GSvector2 GAME_POS{ 960, 250 };

// ヒーローの勝利画像の位置
static const GSvector2 HERO_VICTORY_POS{ 1310, 590 };
// ヒーローの勝利画像の中心
static const GSvector2 HERO_VICTORY_CENTER{ 1024 * 0.5f, 1024 * 0.5f };
// ヒーローの勝利画像のサイズ
static const GSvector2 HERO_VICTORY_SIZE{ 0.7f, 0.7f };

/*実装*/
// 開始
void ResultScene::start() {
	// スクリーンの登録
	ScreenManager::register_screen(ScreenID::Status, new StatusScreen{});

	// 勇者ステータスボタンの設定
	ButtonWidget* status_btn = new ButtonWidget{ STATUS_BTN_POS, STATUS_BTN_SIZE, Texture::HeroStatus, STATUS_BTN_SPACE };
	status_btn->attach_click([=]() {ScreenManager::push_screen(ScreenID::Status); });
	// タイトルボタンの設定
	ButtonWidget* Title_btn = new ButtonWidget{ GSvector2{ BACK_BTN_CENTER_POS.x - TITLE_BTN_SIZE.x * 0.5f - SPACE, BACK_BTN_CENTER_POS.y },
		TITLE_BTN_SIZE, Texture::BackTitle, GSvector2{ (512 - TITLE_BTN_SIZE.x) * 0.5f, (512 - TITLE_BTN_SIZE.y) * 0.5f } };
	Title_btn->attach_click([=]() { next_scene_ = NextScene::Title; });
	// リトライボタンの設定
	ButtonWidget* retry_btn = new ButtonWidget{ GSvector2{ BACK_BTN_CENTER_POS.x - RETRY_BTN_SIZE.x * 0.5f, BACK_BTN_CENTER_POS.y },
		RETRY_BTN_SIZE, Texture::Retry, GSvector2{ (512 - RETRY_BTN_SIZE.x) * 0.5f, (512 - RETRY_BTN_SIZE.y) * 0.5f } };
	retry_btn->attach_click([=]() { next_scene_ = NextScene::GamePlay; });
	// ステージ選択へボタンの設定
	ButtonWidget* stage_select_btn = new ButtonWidget{ GSvector2{ BACK_BTN_CENTER_POS.x - STAGESELECT_BTN_SIZE.x * 0.5f + SPACE, BACK_BTN_CENTER_POS.y },
		STAGESELECT_BTN_SIZE, Texture::BackStageSelect, GSvector2{ (512 - STAGESELECT_BTN_SIZE.x) * 0.5f, (512 - STAGESELECT_BTN_SIZE.y) * 0.5f } };
	stage_select_btn->attach_click([=]() { next_scene_ = NextScene::StageSelect; });

	// ボタンの登録
	button_manager_.add_button(status_btn);
	button_manager_.add_button(Title_btn);
	button_manager_.add_button(retry_btn);
	button_manager_.add_button(stage_select_btn);

	// 次のシーン
	next_scene_ = NextScene::None;
	// ゲームクリアフラグ
	is_gameclear_ = GameManager::is_game_clear();
	// BGMフラグ
	is_bgm_ = false;
	// BGM停止
	gsStopBGM();

	// SE再生
	if (is_gameclear_) gsPlaySE(SE::GameClear);
	else gsPlaySE(SE::GameOver);
}

// 更新
void ResultScene::update(float delta_time) {
	// マウスの更新
	Mouse::update(delta_time);

	// ポーズ画面じゃない時
	if (ScreenManager::is_empty()) {
		button_manager_.update(delta_time);
	}
	// SEが終わったらBGM再生
	if (!is_bgm_ && !(gsIsPlaySE(SE::GameClear) || gsIsPlaySE(SE::GameOver))) {
		if (is_gameclear_) gsPlayBGM(BGM::GameClear);
		else gsPlayBGM(BGM::GameOver);
		is_bgm_ = true;
	}

	// スクリーンの更新
	ScreenManager::update(delta_time);

#ifdef _DEBUG
	if (gsGetKeyTrigger(GKEY_SPACE)) this->is_end_ = true;
#endif

	if (next_scene_ != NextScene::None) this->is_end_ = true;
}

// 描画
void ResultScene::draw() const {

	GSvector2 center{ TEXTURE_SIZE * 0.5f, TEXTURE_SIZE * 0.5f };
	GSuint textureID = 0;
	// ゲームクリアの描画
	if (is_gameclear_) {
		// BG
		GSvector2 scale{ 1920 / BG_SIZE.x, 1080 / BG_SIZE.y };
		gsDrawSprite2D(Texture::Banner, NULL, NULL, NULL, NULL, &scale, NULL);
		// ヒーローの勝利ポーズ
		gsDrawSprite2D(Texture::HeroVictoryGameClear, &HERO_VICTORY_POS, NULL, &HERO_VICTORY_CENTER, NULL, &HERO_VICTORY_SIZE, NULL);

		textureID = Texture::GameClearLogo;
	}
	// ゲームオーバーの描画
	else {
		gsDrawSprite2D(Texture::HeroDieGameOver, NULL, NULL, NULL, NULL, NULL, NULL);
		textureID = Texture::GameOverLogo;
	}
	gsDrawSprite2D(textureID, &GAME_POS, NULL, &center, NULL, NULL, NULL);

	button_manager_.draw();

	// スクリーンの描画
	ScreenManager::draw();
	// マウスカーソルの描画
	Mouse::draw();
}

// 終了
void ResultScene::end() {
	// BGM停止
	gsStopBGM();

	// スクリーン削除
	ScreenManager::clear();
	// ボタンの削除
	button_manager_.clear();

	this->is_end_ = false;
}