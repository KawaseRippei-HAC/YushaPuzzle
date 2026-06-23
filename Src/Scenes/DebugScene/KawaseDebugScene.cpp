#include "Scenes/DebugScene/KawaseDebugScene.h"

#include <gslib.h>
#include <GSstandard_shader.h>

#include "Tweens/Tween.h"
#include "Systems/Mouse.h"
#include "Defines/Assets.h"
#include "UI/ButtonWidget.h"
#include "Systems/AssetLoader.h"
#include "Screens/PauseScreen.h"
#include "Screens/OptionAssets.h"
#include "Characters/CharacterNameTable.h"
#include "Characters/Heroes/HeroDefine.h"

/*実装*/
// 開始
void KawaseDebugScene::start() {
	// シェーダーの有効化
	gsInitDefaultShader();
	// 視錐台カリングの有効化
	gsEnable(GS_FRUSTUM_CULLING);

	// アセットの読み込み
	AssetLoader::load("Assets/Data/assets.csv");
	game_manager_.initialize("Assets/Data/map.csv");
	// シェーダーの読み込み
	gsLoadShader(Shader::TileInstanced,
		"Assets/Shader/instanced_tile_mesh.vert", "Assets/Shader/instanced_tile_mesh.frag");

	// カメラの初期化
	camera_ = std::make_unique<DraggableCamera>(GSvector3{ 0.0f, 20.0f, -15.0f });

	debug_character_ = std::make_unique<CharacterBase>(Mesh::Hero, 0, true);
	debug_character_->transform().position(DEBUG_POS_A);
	debug_character_->transform().localScale(GSvector3::one() * 1.0f);

	building_mover_ = std::make_unique<BuildingMover>();
	is_approaching_ = true;
	building_mover_->initialize(DEBUG_POS_A, DEBUG_POS_B);
}

// 更新
void KawaseDebugScene::update(float delta_time) {
	// ポーズ画面じゃない時
	if (ScreenManager::is_empty()) {
		// カメラの更新
		camera_->update(delta_time);

		if (gsGetKeyTrigger(GKEY_RETURN)) this->is_end_ = true;

		if (debug_character_ && building_mover_) {
			if (is_approaching_) {
				building_mover_->on_approach(delta_time, *debug_character_);
			} else {
				building_mover_->on_departure(delta_time, *debug_character_);
			}
			// キャラクターのアニメーション更新
			debug_character_->mesh().update(delta_time);
			debug_character_->mesh().transform(debug_character_->transform().localToWorldMatrix());

			// 完了チェック
			if (building_mover_->is_completed()) {
				// 状態を反転
				is_approaching_ = !is_approaching_;

				// 次の移動を初期化
				if (is_approaching_) {
					// BからAへ(接近)
					debug_character_->transform().position(DEBUG_POS_A);
					building_mover_->initialize(DEBUG_POS_A, DEBUG_POS_B);
				} else {
					// AからBへ(離脱)
					debug_character_->transform().position(DEBUG_POS_B);
					building_mover_->initialize(DEBUG_POS_A, DEBUG_POS_B);
				}
			}
		}
	}

}

// 描画
void KawaseDebugScene::draw() const {
	//camera_->draw();

	debug_character_->mesh().draw();
}

// 終了
void KawaseDebugScene::end() {
	debug_character_.reset(nullptr);
	building_mover_.reset(nullptr);

	// UIのクリア
	ScreenManager::clear();
	// ゲームのクリア
	camera_.reset(nullptr);
	//game_manager_.clear();

	// アセットの削除
	AssetLoader::clear();

	gsDisable(GS_FRUSTUM_CULLING);
	gsEndDefaultShader();

	this->is_end_ = false;
}