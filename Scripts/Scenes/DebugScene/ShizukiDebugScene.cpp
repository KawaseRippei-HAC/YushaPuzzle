#include "Scenes/DebugScene/ShizukiDebugScene.h"

#include <gslib.h>
#include <GSstandard_shader.h>

#include "Defines/Assets.h"
#include "Systems/AssetLoader.h"

/*実装*/
// 開始
void ShizukiDebugScene::start() {
	gsInitDefaultShader();
	gsEnable(GS_FRUSTUM_CULLING);

	AssetLoader::load("Assets/Data/assets.csv");
	game_manager_ = std::make_unique<GameManager>();
	game_manager_->initialize("Assets/Data/map.csv");
	gsLoadShader(
		Shader::TileInstanced,
		"Assets/Shader/instanced_tile_mesh.vert",
		"Assets/Shader/instanced_tile_mesh.frag"
	);
}

// 更新
void ShizukiDebugScene::update(float delta_time) {
	if (gsGetKeyTrigger(GKEY_R)) {
		game_manager_ = std::make_unique<GameManager>();
		game_manager_->initialize("Assets/Data/map.csv");
	}

	game_manager_->update(delta_time);
}

// 描画
void ShizukiDebugScene::draw() const {
	game_manager_->draw();
	game_manager_->draw_gui();
}

// 終了
void ShizukiDebugScene::end() {
	game_manager_->clear();
	AssetLoader::clear();

	gsDisable(GS_FRUSTUM_CULLING);
	gsEndDefaultShader();

	this->is_end_ = false;
}