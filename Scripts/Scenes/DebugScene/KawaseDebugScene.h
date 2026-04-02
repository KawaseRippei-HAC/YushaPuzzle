#ifndef SCENES_DEBUG_SCENE_KAWASE_DEBUG_SCENE_H_
#define SCENES_DEBUG_SCENE_KAWASE_DEBUG_SCENE_H_

#include "Interfaces/IScene.h"

#include <memory>

#include "Systems/GameManager.h"
#include "Screens/ScreenManager.h"
#include "Cameras/DraggableCamera.h"
#include "Events/BuildingMover.h"
#include "Characters/CharacterBase.h"

class KawaseDebugScene :
	public IScene {
public:
	// 開始
	void start(void) override;

	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

	// 終了
	void end(void) override;
	// 終了フラグ
	bool is_end(void) const noexcept override { return is_end_; }

	// 次シーンIDの取得
	Scene next(void) const noexcept override { return Scene::Debug; }

private:

	GameManager game_manager_;
	std::unique_ptr<DraggableCamera> camera_{ nullptr };

	// 終了フラグ
	bool is_end_{ false };

	std::unique_ptr<CharacterBase> debug_character_{ nullptr };
	std::unique_ptr<BuildingMover> building_mover_{ nullptr };
	bool is_approaching_{ true };

	// デバック用の移動開始/終了座標
	const GSvector3 DEBUG_POS_A{ 0.0f,0.0f,0.0f };
	const GSvector3 DEBUG_POS_B{ -5.0f,0.0f,0.0f };
};

#endif // SCENES_DEBUG_SCENE_KAWASE_DEBUG_SCENE_H_