#ifndef INLINES_MOUSE_INL_
#define INLINES_MOUSE_INL_

#include "Systems/Mouse.h"

/*インライン関数実装*/
// カーソル表示の設定
inline void Mouse::show_cursor(bool is_show) noexcept {
	(is_show) ? gsShowMouseCursor() : gsHideMouseCursor();
}

// ドラッグハンドラの接続
inline void Mouse::attach_drag_handler(GMOUSTATE button, IDragHandler* handler) noexcept {
	handlers_map_[button].insert(handler);
}

// ドラッグハンドラの解除
inline void Mouse::detach_drag_handler(IDragHandler* handler) noexcept {
	for (auto& [_, handlers] : handlers_map_) {
		if (handlers.erase(handler)) return;
	}
}

// ドラッグハンドラの消去
inline void Mouse::clear_drag_handlers(bool is_fullclear) noexcept {
	if (!is_fullclear) handlers_map_.clear();
	else handlers_map_ = std::unordered_map<GMOUSTATE, std::unordered_set<IDragHandler*>>{};
}

// ホイール回転量の取得
inline float Mouse::wheel() noexcept {
	return wheel_;
}

// カーソル位置の取得
inline GSvector2 Mouse::position() noexcept {
	return position_;
}

// カーソル移動量の取得
inline GSvector2 Mouse::velocity() noexcept {
	return velocity_;
}

// レイの取得
inline const Ray& Mouse::ray() noexcept {
	return ray_;
}

#endif // INLINES_MOUSE_INL_