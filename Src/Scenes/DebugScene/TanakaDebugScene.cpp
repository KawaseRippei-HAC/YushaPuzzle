#include "Scenes/DebugScene/TanakaDebugScene.h"

#include "Screens/PauseScreen.h"
#include"UI/ButtonWidget.h"
#include "Screens/OptionAssets.h"
#include "Defines/Assets.h"
#include "Systems/AssetLoader.h"

/*実装*/
// 開始
void TanakaDebugScene::start() {
	// アセットの読み込み
	// メッシュの読み込み
	AssetLoader::load("Assets/Data/assets.csv");

	gsPlayBGM(BGM::Title);

	is_title_ = false;


	// バトルシステムの動作確認
	tile_ = std::make_unique<TileManager>("Assets/Data/map.csv");
	tile_->graph();
	hero_ = new HeroCharacter{ tile_->graph() };
	/*enemy_ = new MonsterCharacter{};*/
	//enemy_->speed_ = 20;
	//BattleSystem::initialize(*hero_, enemy_->get_battle_actor());
}

// 更新
void TanakaDebugScene::update(float delta_time) {

	// バトルシステムの動作確認
	if (gsGetKeyTrigger(GKEY_F)) {
		//BattleSystem::initialize(*hero_, enemy_->get_battle_actor());
	}

	// 戦闘終了してない時実行
	if (!BattleSystem::is_finished()) {
		//BattleSystem::execute(delta_time, *hero_, enemy_->get_battle_actor());
	}

	//// ポーズ画面じゃない時
	//if (screenManager_.is_empty()) {
	//	buttonManager_.update(delta_time);

	//	// TABキーでポーズ画面へ
	//	if (gsGetKeyTrigger(PAUSE_KEY)) {
	//		screenManager_.push_screen(new PauseScreen(&screenManager_, &is_title_, &hero_parameter_));
	//	}
	//}
	//screenManager_.update(delta_time);

	////タイトルに戻る
	//if (is_title_ == true) {
	//	this->is_end_ = true;
	//}
	if (gsGetKeyTrigger(GKEY_SPACE)) {
		this->is_end_ = true;
	}
}

// 描画
void TanakaDebugScene::draw() const {

	buttonManager_.draw();
}

// 終了
void TanakaDebugScene::end() {
	this->is_end_ = false;

	buttonManager_.clear();

	// テクスチャの削除
	AssetLoader::clear();

	tile_.reset(nullptr);
	delete hero_;
	//delete enemy_;
}