#ifndef INLINES_ANIMATION_LAYER_INL_
#define INLINES_ANIMATION_LAYER_INL_

#include <deque>

#include "Animations/AnimationLayer.h"

/*定義*/
// モーション最終フレーム
#define LAST_FRAME(end_time) (end_time - 1.0f)

/*インライン関数実装*/
// イベントの追加
inline void AnimationLayer::add_event(
	GSuint target_motion, float trigger_time, const std::function<void()>& event_callback) {
	events_.push_back(std::make_unique<Event>(target_motion, trigger_time, event_callback));
}

// イベントの消去
inline void AnimationLayer::clear_event(GSuint target_motion) {
	std::erase_if(events_,
		[target_motion](std::unique_ptr<Event>& event) { return event->motion_ == target_motion; }
	);
}

// モーションの変更
inline void AnimationLayer::change_motion(GSuint motion, bool loop, float lerp_time) {
	if (motion == motion_) return;
	forced_change_motion(motion, loop, lerp_time);
}

// 現在のモーションの取得
inline GSuint AnimationLayer::current_motion() const noexcept {
	return motion_;
}

// モーション再生時間の取得
inline float AnimationLayer::motion_time() const noexcept {
	return motion_timer_;
}

// モーション再生時間の設定
inline void AnimationLayer::motion_time(float time) noexcept {
	motion_timer_ = time;
}

// モーション速度の設定
inline void AnimationLayer::motion_speed(float scale) noexcept {
	motion_speed_ = scale;
}

// モーション終了時間の取得
inline float AnimationLayer::motion_end_time() const noexcept {
	return gsGetEndAnimationTime(animation_, motion_);
}

// モーション終了フラグ
inline bool AnimationLayer::is_motion_finished() const noexcept {
	return (is_looping_) ? false : (motion_timer_ >= LAST_FRAME(motion_end_time()));
}

// ボーン数の取得
inline int AnimationLayer::count_bone() const noexcept {
	return (int)gsGetAnimationNumBones(animation_, motion_);
}

#endif // INLINES_ANIMATION_LAYER_INL_