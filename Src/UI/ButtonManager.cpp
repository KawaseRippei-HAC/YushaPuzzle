#include "UI/ButtonManager.h"

// コンストラクタ
ButtonManager::ButtonManager() {

}
// デストラクタ
ButtonManager::~ButtonManager() {
	clear();
}

// 更新
void ButtonManager::update(float delta_time) {
	for (auto button : buttons_) {
		button->update(delta_time);
	}
}
// 描画
void ButtonManager::draw(void) const {
	for (auto button : buttons_) {
		button->draw();
	}
}
// ボタンの追加
void ButtonManager::add_button(ButtonWidget* button) {
	buttons_.push_back(button);
}

// ボタンの削除
void ButtonManager::clear() {
	for ( auto button : buttons_) {
		delete button;
	}
	buttons_.clear();
}
