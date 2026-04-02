#ifndef ANIMATIONS_ANIMATION_EVENT_H_
#define ANIMATIONS_ANIMATION_EVENT_H_

#include <functional>

#include <GStype.h>

// アニメーションイベント構造体
struct AnimationEvent final {
	// コンストラクタ
	AnimationEvent(GSuint target_motion, float trigger_time, const std::function<void(void)>& event_callback) :
		motion_{ target_motion }, time_{ trigger_time }, callback_{ event_callback } {
	}

	// 対象モーション
	GSuint motion_{ 0 };
	// 発生タイミング
	float time_{ 0.0f };
	// 発生時コールバック
	std::function<void(void)> callback_{ nullptr };
};

#endif // ANIMATIONS_ANIMATION_EVENT_H_