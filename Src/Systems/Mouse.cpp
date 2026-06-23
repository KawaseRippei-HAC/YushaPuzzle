#include "Systems/Mouse.h"

#include "Interfaces/IDragHandler.h"
#include "Defines/Assets.h"

/*実装*/
// 更新
void Mouse::update(float delta_time) {
	// マウスカーソル位置の取得
	int mx{ 0 }, my{ 0 };
	gsGetMouseCursorPosition(&mx, &my);
	position_ = { static_cast<float>(mx), static_cast<float>(my) };

	// マウスカーソル移動量の取得
	int vx{ 0 }, vy{ 0 }, vz{ 0 };
	gsGetMouseVelocity(&vx, &vy, &vz);
	wheel_ = static_cast<float>(vz);
	velocity_ = { static_cast<float>(vx), static_cast<float>(vy) };

	// レイの計算
	gsCalculateRay(
		position_.x, position_.y,
		&ray_.origin.x, &ray_.origin.y, &ray_.origin.z,
		&ray_.direction.x, &ray_.direction.y, &ray_.direction.z
	);

	// ドラッグの更新
	for (auto& [button, handlers] : handlers_map_) {
		update_drag(button, handlers);
	}
}

// 描画
void Mouse::draw() noexcept {
	gsDrawSprite2D(Texture::MouseCursor, &position_, NULL, NULL, NULL, NULL, 0.0f);
}

// ドラッグの更新
void Mouse::update_drag(GMOUSTATE button, std::unordered_set<IDragHandler*>& handlers) noexcept {
	static bool is_dragging = false;
	static GSvector2 start_position = { 0.0f, 0.0f };
	static GSvector2 last_position = { 0.0f, 0.0f };

	// ドラッグ開始処理
	if (!is_dragging && gsGetMouseButtonTrigger(button)) {
		// ドラッグ開始
		is_dragging = true;
		// 開始位置と最終位置を設定
		start_position = last_position = position_;
		// ハンドラへ通知
		for (IDragHandler* handler : handlers) {
			handler->on_drag_begin(start_position);
		}
	}

	// ドラッグ中処理
	if (is_dragging && gsGetMouseButtonState(button)) {
		// 移動差分を求める
		const GSvector2 moved_delta{ position_ - last_position };
		// ハンドラへ通知
		for (IDragHandler* handler : handlers) {
			handler->on_dragging(start_position, position_, moved_delta);
		}
		// 最終位置を更新
		last_position = position_;
	}

	// ドラッグ終了処理
	if (is_dragging && gsGetMouseButtonDetach(button)) {
		// ドラッグ終了
		is_dragging = false;
		// ハンドラへ通知
		for (IDragHandler* handler : handlers) {
			handler->on_drag_end(position_);
		}
	}
}