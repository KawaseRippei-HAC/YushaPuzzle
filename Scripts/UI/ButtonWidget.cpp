#include "UI/ButtonWidget.h"
#include "Defines/Assets.h"

ButtonWidget::ButtonWidget(GSvector2 position, GSvector2 button_range, GSuint button_texture, GSvector2 space, const char* text, int text_size):
position_(position - space), button_texture_(button_texture), text_(text), text_size_(text_size)
{
	rectangle_ = { position.x, position.y, position.x + button_range.x, position.y + button_range.y };
	text_pos_ = { position.x, position.y + text_size * 0.5f};
	is_hover_ = false;
}

// 更新
void ButtonWidget::update(float delta_time) {
	prev_is_hover_ = is_hover_;
	// マウス座標の取得
	int mx{ 0 }, my{ 0 };
	gsGetMouseCursorPosition(&mx, &my);
	const GSvector2 mouse_point{ static_cast<float>(mx), static_cast<float>(my) };

	// ボタン範囲内にマウスカーソルがあるか
	if (is_inside(mouse_point)) {
		// クリックされたら通知
		if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
			click_notify();
			gsPlaySE(SE::Click);
		}

		is_hover_ = true;
	}
	else {
		is_hover_ = false;
	}

	// ホバー中の処理
	if (is_hover_) {
		// ホバー中の通知処理
		hover_notify();
		if (!prev_is_hover_) {
			// 前のフレームで重なっていなかったらSE再生
			gsPlaySE(SE::Cursor);
		}
	}
}
// 描画
void ButtonWidget::draw(void) const {

	GScolor buttonColor = { 1.0f, 1.0f, 1.0f, 0.7f };
	// マウスカーソルが重なっていたら半透明解除
	if (is_hover_) {
		buttonColor.a = 1.0f;
		buttonColor.g = 0.0f;
		buttonColor.b = 0.0f;
	}

	//ボタンの描画
	gsDrawSprite2D(button_texture_, &position_, NULL, NULL, &buttonColor, NULL, 0.0f);

	gsFontParameter(GS_FONT_NORMAL, text_size_, "MSゴシック");
	gsTextPos(text_pos_.x, text_pos_.y);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	gsDrawText(text_.c_str());
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

// オブザーバーの接続
void ButtonWidget::attach_click(const std::function<void()>& observer) {
	click_observers_.push_back(observer);
}
// オブザーバーの接続（ホバー中の処理）
void ButtonWidget::attach_hover(const std::function<void()>& observer) {
	hover_observers_.push_back(observer);
}

// 通知処理
void ButtonWidget::click_notify() const {
	// 全ての観測者に通知
	for (const auto& observer : click_observers_) {
		observer();
	}
}
void ButtonWidget::hover_notify() const {
	// 全ての観測者に通知
	for (const auto& observer : hover_observers_) {
		observer();
	}
}

// ボタン範囲内か
bool ButtonWidget::is_inside(const GSvector2& point) {
	
	if (point.x < rectangle_.left)	return false;
	if (point.y < rectangle_.top)	return false;
	if (point.x > rectangle_.right)	return false;
	if (point.y > rectangle_.bottom)return false;
	return true;
}


