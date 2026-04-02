#ifndef SCENES_NULL_SCENE_H_
#define SCENES_NULL_SCENE_H_

#include "Interfaces/IScene.h"

// Nullシーンクラス
class NullScene final:
	public IScene {
public:
	// 開始
	void start(void) override {}

	// 更新
	void update(float delta_time) override {}
	// 描画
	void draw(void) const override {}

	// 終了
	void end(void) override {}
	// 終了フラグ
	bool is_end(void) const noexcept override { return false; }

	// 次シーンIDの取得
	Scene next(void) const noexcept override { return Scene::Null; }
};

#endif // SCENES_NULL_SCENE_H_