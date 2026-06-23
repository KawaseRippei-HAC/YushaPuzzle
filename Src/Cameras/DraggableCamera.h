#ifndef CAMERAS_DRAGGABLE_CAMERA_H_
#define CAMERAS_DRAGGABLE_CAMERA_H_

#include <GStype.h>
#include <GStransform.h>

#include "Interfaces/IDragHandler.h"
#include "Interfaces/IVirtualCamera.h"

// ドラッグ可能カメラ
class DraggableCamera :
	public IDragHandler, public IVirtualCamera {
public:
	DraggableCamera(
		const GSvector3& position,
		const GSvector3& at = { 0.0f, 0.0f, 0.0f },
		const GSvector3& up = GSvector3::up()
	);
	~DraggableCamera();

	// 更新
	void update(float delta_time) override;

	// ドラッグ開始時処理
	void on_drag_begin(const GSvector2& position) override;
	// ドラッグ中処理
	void on_dragging(
		const GSvector2& start_position, const GSvector2& current_position, const GSvector2& moved_delta) override;
	// ドラッグ終了時処理
	void on_drag_end(const GSvector2& position) override;

private:
	// 平行移動量
	GSvector3 translation_{ 0.0f, 0.0f, 0.0f };
};

#endif // CAMERAS_DRAGGABLE_CAMERA_H_