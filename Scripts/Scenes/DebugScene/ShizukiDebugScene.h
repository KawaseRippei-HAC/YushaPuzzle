#ifndef SCENES_DEBUG_SCENE_SHIZUKI_DEBUG_SCENE_H_
#define SCENES_DEBUG_SCENE_SHIZUKI_DEBUG_SCENE_H_

#include "Interfaces/IScene.h"

#include "Systems/GameManager.h"
#include "Cameras/DraggableCamera.h"

class ShizukiDebugScene :
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
	std::unique_ptr<GameManager> game_manager_{ nullptr };

	// 終了フラグ
	bool is_end_{ false };
};

#endif // SCENES_DEBUG_SCENE_SHIZUKI_DEBUG_SCENE_H_