#include "Scenes/GamePlayScene/GamePlayScene.h"

#include <gslib.h>

#include "Tweens/Tween.h"
#include "Systems/Mouse.h"
#include "Defines/Assets.h"
#include "UI/ButtonWidget.h"
#include "Systems/AssetLoader.h"
#include "Systems/EffectSystem.h"
#include "Screens/PauseScreen.h"
#include "Screens/OptionScreen.h"
#include "Screens/StatusScreen.h"
#include "Screens/StatusAllocationScreen.h"
#include "Screens/SkillSelectScreen.h"
#include "Screens/BuildingExplanationScreen.h"
#include "Screens/RuleExplanationScreen.h"
#include "Screens/OptionAssets.h"
#include "Characters/CharacterNameTable.h"
#include "Data/ResultData.h"

/*実装*/
GamePlayScene::GamePlayScene() {
}

// 開始
void GamePlayScene::start() {
	// マップの読み込み
	game_manager_.initialize("Assets/Data/map.csv");
	// シェーダーの読み込み
	gsLoadShader(Shader::TileInstanced,
		"Assets/Shader/instanced_tile_mesh.vert", "Assets/Shader/instanced_tile_mesh.frag");

	// BGMの再生
	gsPlayBGM(BGM::HeroMapMovement);

	// UIの初期化
	hero_ui_ = std::make_unique<HeroUI>(game_manager_.get_hero());

	this->is_title_ = false;
	this->is_retry_ = false;
	// スクリーンの登録
	ScreenManager::register_screen(ScreenID::Pause, new PauseScreen{ &is_title_, &is_retry_ });
	ScreenManager::register_screen(ScreenID::SoundOption, new OptionScreen{});
	ScreenManager::register_screen(ScreenID::Status, new StatusScreen{ game_manager_.get_hero() });
	ScreenManager::register_screen(ScreenID::StatusAllocation, new StatusAllocationScreen{ game_manager_.get_hero() });
	ScreenManager::register_screen(ScreenID::SkillSelect, new SkillSelectScreen{ game_manager_.get_hero() });
	ScreenManager::register_screen(ScreenID::BuildingExplanation, new BuildingExplanationScreen{ game_manager_.get_hero() });
	ScreenManager::register_screen(ScreenID::RuleExplanation, new RuleExplanationScreen{});
	ScreenManager::push_screen(ScreenID::RuleExplanation);
}

// 更新
void GamePlayScene::update(float delta_time) {
	// マウスの更新
	Mouse::update(delta_time);

	// ポーズ画面じゃない時
	if (ScreenManager::is_empty()) {
		// ゲームマネージャーの更新
		game_manager_.update(delta_time);

		buttonManager_.update(delta_time);

		// TABキーでポーズ画面へ
		if (gsGetKeyTrigger(PAUSE_KEY)) {
			ScreenManager::push_screen(ScreenID::Pause);
		}
	}

	// Tweenの更新
	Tween::update(delta_time);
	// スクリーンの更新
	ScreenManager::update(delta_time);

	//タイトルに戻る リトライ
	if (is_title_ || is_retry_) this->is_end_ = true;
	// ゲーム終了
	else this->is_end_ = game_manager_.is_game_clear() || game_manager_.is_game_over();
}

// 描画
void GamePlayScene::draw() const {
	game_manager_.draw();

	// 画面上端の背景描画
	//gsDrawSprite2D(Texture::BackGround, &background_pos_, NULL, NULL, NULL, NULL, 0.0f);
	// 画面下端のバナー描画
	gsDrawSprite2D(Texture::Banner, &banner_pos_, &banner_rect_, NULL, NULL, NULL, 0.0f);

	// 勇者のUI表示
	game_manager_.draw_gui();

	buttonManager_.draw();

	// スクリーンの描画
	ScreenManager::draw();
	// マウスカーソルの描画
	Mouse::draw();
}

// 終了
void GamePlayScene::end() {
	// リザルト用のデータを保存
	HeroCharacter* hero = game_manager_.get_hero();
	ResultData::save_level(hero->level());
	ResultData::save_status(hero->hp(), hero->max_hp(), hero->attack(), hero->defense(), hero->speed(), hero->luck());
	ResultData::save_skills(hero->skill_actor()->skills());

	// UIのクリア
	ScreenManager::clear();
	buttonManager_.clear();

	// ゲームのクリア
	game_manager_.clear();
	Mouse::clear_drag_handlers();
	Tween::clear();

	if (EffectSystem::is_alive()) gsStopAllEffects();

	this->is_title_ = false;
	this->is_retry_ = false;
	this->is_end_ = false;
}