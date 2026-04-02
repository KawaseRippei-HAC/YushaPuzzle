#include "Screens/ScreenManager.h"

// 更新
void ScreenManager::update(float delta_time) {
	// 最初のスクリーンの初期化
	if (!is_top_initialize_ && !screen_stack_.empty()) {
		screen_stack_.top()->initialize();
		is_top_initialize_ = true;
	}

	//スクリーンがあれば更新
	if (!screen_stack_.empty()) {
		screen_stack_.top()->update(delta_time);
	}

	// 次のスクリーンの初期化
	if (!is_top_initialize_ && !screen_stack_.empty()) {
		screen_stack_.top()->initialize();
		is_top_initialize_ = true;
	}
}
// 描画
void ScreenManager::draw(void) {
	//スクリーンがあれば描画
	if (!screen_stack_.empty()) {
		screen_stack_.top()->draw();
	}
}
// スクリーンの登録
void ScreenManager::register_screen(ScreenID id, IScreen* screen) {
	screen_map_[id] = screen;
}
// スクリーンの追加
void ScreenManager::push_screen(ScreenID id) {
	IScreen* screen = screen_map_[id];
	// 登録されているスクリーンなら追加
	if (screen != nullptr) {
		screen_stack_.push(screen);

		is_top_initialize_ = false;
	}
	// 登録されていない場合、何もしない
}
// 最後に追加したスクリーンの削除
void ScreenManager::pop_screen() {
	if (!screen_stack_.empty()) {
		screen_stack_.pop();
		is_top_initialize_ = false;
	}
}
// スタックの消去
void ScreenManager::stack_clear() {
	while (!screen_stack_.empty()) {
		screen_stack_.pop();
	}
}
// スクリーンマップ消去
void ScreenManager::map_clear() {
	for (auto& screen : screen_map_) {
		delete screen.second;
	}
	screen_map_.clear();
}
// 全て消去
void ScreenManager::clear() {
	// スタックの消去
	stack_clear();
	// マップの消去
	map_clear();
}
// スクリーンが空か
bool ScreenManager::is_empty() {
	return screen_stack_.empty();
}
