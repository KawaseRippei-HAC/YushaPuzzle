#ifndef INTERFACES_IVIRTUAL_CAMERA_H_
#define INTERFACES_IVIRTUAL_CAMERA_H_

#include <GStransform.h>

// バーチャルカメラ抽象インターフェース
class IVirtualCamera {
public:
	virtual ~IVirtualCamera() = default;
	// 更新
	virtual void update(float delta_time) = 0;
	// トランスフォームの取得
	const GStransform& transform(void) const noexcept { return transform_; }

protected:
	// トランスフォーム
	GStransform transform_;
};

#endif // INTERFACES_IVIRTUAL_CAMERA_H_