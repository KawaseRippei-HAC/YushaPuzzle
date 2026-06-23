#ifndef SCENES_LOAD_SECNE_LOAD_SCENE_H_
#define SCENES_LOAD_SECNE_LOAD_SCENE_H_

#include "Interfaces/IScene.h"

// ロードシーンクラス
class LoadScene final :
	public IScene {
public:
	LoadScene(const char* file_path, Scene next) noexcept;

	// 開始
	void start(void) override;

	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

	// 終了
	void end(void) override;
	// 終了フラグ
	bool is_end(void) const noexcept override;

	// 次シーンIDの取得
	Scene next(void) const noexcept override;

private:
	bool is_loaded_{ false };

	Scene next_{ Scene::Null };
	const char* file_path_{ nullptr };

	int sprite_index_{ 0 };
	float animation_timer_{ 0.0f };
};

#endif // !SCENES_LOAD_SECNE_LOAD_SCENE_H_