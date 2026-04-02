#include "Scenes/SceneManager.h"

/*静的メンバ変数*/
// Nullシーン
NullScene SceneManager::null_scene_;

/*実装*/
// コンストラクタ
SceneManager::SceneManager() :
	scene_timer_{ 0.0f },
	current_scene_{ &null_scene_ } {
}

// デストラクタ
SceneManager::~SceneManager() {
	this->clear();
}

// 更新
void SceneManager::update(float delta_time) {
	current_scene_->update(delta_time);
	scene_timer_ += delta_time;
	if (current_scene_->is_end()) {
		change_scene(current_scene_->next());
	}
}

// 描画
void SceneManager::draw() const {
	current_scene_->draw();
}

// 終了
void SceneManager::end() {
	current_scene_->end();
	current_scene_ = &null_scene_;
}

// シーンの登録
void SceneManager::register_scene(Scene id, IScene* scene) {
	if (!scene) return;
	scene_map_[id] = scene;
}

// シーンの変更
void SceneManager::change_scene(Scene id) {
	this->end();
	auto scene_it = scene_map_.find(id);
	if (scene_it != scene_map_.end()) {
		current_scene_ = scene_map_[id];
		current_scene_->start();
		scene_timer_ = 0.0f;
	}
}

// 消去
void SceneManager::clear() {
	this->end();
	current_scene_ = nullptr;
	for (auto& pair : scene_map_) {
		delete pair.second;
		pair.second = nullptr;
	}
	scene_map_.clear();
}