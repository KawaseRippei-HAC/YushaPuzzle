#ifndef PHYSICS_RAY_H_
#define PHYSICS_RAY_H_

#include <GSvector3.h>

// レイクラス
class Ray final {
public:
	Ray(const GSvector3& origin, const GSvector3& direction) :
		origin{ origin }, direction{ direction } {
	}

public:
	// 原点
	GSvector3 origin{ 0.0f, 0.0f, 0.0f };
	// 方向
	GSvector3 direction{ 0.0f, 0.0f, 0.0f };
};

#endif // PHYSICS_RAY_H_