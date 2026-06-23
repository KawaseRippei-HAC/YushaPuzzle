#ifndef INLINES_SLIDER_INL_
#define INLINES_SLIDER_INL_

#include "UI/SliderWidget.h"

/*インライン関数実装*/
// オブザーバーの接続
inline void SliderWidget::attach(const std::function<void(float)>& observer) {
	observer(value_);
	observers_.push_back(observer);
}

// 通知処理
inline void SliderWidget::notify(float progress) const {
	for (const std::function<void(float)>& observer : observers_) {
		observer(progress);
	}
}

// 範囲内フラグ
inline bool SliderWidget::is_inside(const GSvector2& point, const GSrect& rectangle) {
	if (point.x < rectangle.left) return false;
	if (point.x > rectangle.right) return false;
	if (point.y < rectangle.top) return false;
	if (point.y > rectangle.bottom) return false;
	return true;
}

// 操作部x座標の適用
inline float SliderWidget::apply_thumb_x(const GSvector2& mouse_point) const {
	return CLAMP(mouse_point.x, position_.x, position_.x + track_size_.x);
}

// 値の適用
inline float SliderWidget::apply_value() const {
	return (thumb_position_.x - position_.x) / track_size_.x;
}

#endif // INLINES_SLIDER_INL_