#include "Cameras/DraggableCamera.h"

#include "Systems/Mouse.h"

/*定数*/
// ドラッグ速度
static constexpr float DEFAULT_DRAG_SPEED{ 0.02f };
// ズーム速度
static constexpr float DEFAULT_ZOOM_SPEED{ 0.015f };

/*実装*/
// コンストラクタ
DraggableCamera::DraggableCamera(const GSvector3& position, const GSvector3& at, const GSvector3& up) {
	transform_.position(position);
	transform_.lookAt(at, up);
	Mouse::attach_drag_handler(GMOUSE_BUTTON_2, this);
}

// デストラクタ
DraggableCamera::~DraggableCamera() {
	Mouse::detach_drag_handler(this);
}

// 更新
void DraggableCamera::update(float delta_time) {
	transform_.translate(translation_ * delta_time, GStransform::Space::World);

	const GSvector3 zoom = Mouse::ray().direction * Mouse::wheel() * DEFAULT_ZOOM_SPEED;
	transform_.translate(zoom * delta_time, GStransform::Space::World);
}

// ドラッグ開始時処理
void DraggableCamera::on_drag_begin(const GSvector2& position) {
	translation_ = GSvector3::zero();
}

// ドラッグ中処理
void DraggableCamera::on_dragging(
	const GSvector2& start_position, const GSvector2& current_position, const GSvector2& moved_delta) {
	translation_ = GSvector3{ moved_delta.x, 0.0f, moved_delta.y } *DEFAULT_DRAG_SPEED;
}

// ドラッグ終了時処理
void DraggableCamera::on_drag_end(const GSvector2& position) {
	translation_ = GSvector3::zero();
}