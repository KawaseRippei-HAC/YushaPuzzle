#ifndef SCENES_SCENE_MANAGER_H_
#define SCENES_SCENE_MANAGER_H_

#include <unordered_map>

#include "Scenes/NullScene.h"
#include "Utilities/Noncopyable.h"

// シーンマネージャークラス
class SceneManager final :
	private Noncopyable<SceneManager> {
public:
	SceneManager(void);
	~SceneManager(void);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// 終了
	void end(void);

	// シーンの登録
	void register_scene(Scene id, IScene* scene);
	// シーンの変更
	void change_scene(Scene id);

	// 消去
	void clear(void);

	// ゲーム終了フラグ
	bool is_game_end(void) const noexcept { return current_scene_->is_game_end(); }

private:
	// Nullシーン
	static NullScene null_scene_;

	// シーンタイマー
	float scene_timer_{ 0.0f };
	// 現在のシーン
	IScene* current_scene_{ &null_scene_ };
	// シーンマップ
	std::unordered_map<Scene, IScene*> scene_map_;
};

#endif // SCENES_SCENE_MANAGER_H_