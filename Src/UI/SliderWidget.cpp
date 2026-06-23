#include "UI/SliderWidget.h"
#include "Defines/Assets.h"
#include <gslib.h>

/*実装*/
// コンストラクタ
SliderWidget::SliderWidget(
	float initial_value,
	const GSvector2& position,
	const GSvector2& thumb_size, const GSvector2& bar_size,
	GSuint thumb_texture, GSuint bar_texture,
	bool is_test_se,
	const GSvector2& bar_texure_space) :
	value_{ initial_value }, position_{ position },
	thumb_size_{ thumb_size }, track_size_{ bar_size },
	thumb_position_{ position_.x + (track_size_.x * value_), position_.y + (track_size_.y * 0.5f) },
	thumb_texture_{ thumb_texture }, track_texture_{ bar_texture },
	is_test_se_{ is_test_se },
	track_draw_pos_(position - bar_texure_space){
}

// 更新
void SliderWidget::update(float delta_time) {
	// マウス座標の取得
	int mx{ 0 }, my{ 0 };
	gsGetMouseCursorPosition(&mx, &my);
	const GSvector2 mouse_point{ static_cast<float>(mx), static_cast<float>(my) };

	// クリックされたとき
	if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
		// マウスが操作部分または背景の範囲内ならフラグを立てる
		if (is_thumb_inside(mouse_point) || is_bar_inside(mouse_point)) is_holding_ = true;

		//// マウスが操作部分の範囲内ならフラグを立てる
		//if (is_thumb_inside(mouse_point)) is_holding_ = true;
		//// 背景の範囲内なら、値をその場に設定する
		//else if (is_bar_inside(mouse_point)) this->change_value(mouse_point);
	}
	// クリックされた位置が操作部分かつ、長押し中なら、毎フレーム値を更新
	else if (is_holding_ && gsGetMouseButtonState(GMOUSE_BUTTON_1)) {
		this->change_value(mouse_point);
	}
	// それ以外ならフラグを下ろす
	else is_holding_ = false;

	// SE再生
	if (is_test_se_ && gsGetMouseButtonDetach(GMOUSE_BUTTON_1) && is_se_playback_) {
		gsPlaySE(SE::Click);
		is_se_playback_ = false;
	}
}

// 描画
void SliderWidget::draw() const {
	gsDrawSprite2D(track_texture_, &track_draw_pos_, NULL, NULL, NULL, NULL, 0.0f);
	const GSvector2 thumb_center{ thumb_size_.x * 0.5f, thumb_size_.y * 0.5f };
	gsDrawSprite2D(thumb_texture_, &thumb_position_, NULL, &thumb_center, NULL, NULL, 0.0f);
}

// 外部から値を強制的にセット
void SliderWidget::set_value(float value) {
	// 0.0 ~ 1.0の範囲に収める
	if (value < 0.0f) value = 0.0f;
	if (value > 1.0f) value = 1.0f;

	// 内部変数を更新
	this->value_ = value;

	// つまみの描画座標の更新
	thumb_position_.x = position_.x + (track_size_.x * value);
}

// 操作部範囲内フラグ
bool SliderWidget::is_thumb_inside(const GSvector2& mouse_point) const {
	const GSrect thumb_rect{
		thumb_position_.x - (thumb_size_.x * 0.5f),
		thumb_position_.y - (thumb_size_.y * 0.5f),
		thumb_position_.x + (thumb_size_.x * 0.5f),
		thumb_position_.y + (thumb_size_.y * 0.5f)
	};
	return is_inside(mouse_point, thumb_rect);
}

// 背景範囲内フラグ
bool SliderWidget::is_bar_inside(const GSvector2& mouse_point) const {
	const GSrect bar_rect{
		position_.x, position_.y,
		position_.x + track_size_.x, position_.y + track_size_.y
	};
	return is_inside(mouse_point, bar_rect);
}

// 値の変更
void SliderWidget::change_value(const GSvector2& mouse_position) {
	const float prev_x = thumb_position_.x;
	thumb_position_.x = apply_thumb_x(mouse_position);
	if (thumb_position_.x == prev_x) return;
	value_ = apply_value();
	this->notify(value_);
	is_se_playback_ = true;
}