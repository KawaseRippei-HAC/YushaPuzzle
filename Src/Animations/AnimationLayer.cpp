#include "Animations/AnimationLayer.h"

#include <cmath>
#include <algorithm>

/*定義*/
// モーション最終フレーム
#define LAST_FRAME(end_time) (end_time - 1.0f)

/*実装*/
// コンストラクタ
AnimationLayer::AnimationLayer(GSuint animation, GSuint motion, bool loop) :
	animation_{ animation },
	motion_{ motion }, prev_motion_{ motion },
	is_looping_{ loop } {
}

// 更新
void AnimationLayer::update(float delta_time) {
	const float last_time = motion_timer_;
	motion_timer_ += delta_time * motion_speed_;

	// ループ
	if (is_looping_) motion_timer_ = std::fmod(motion_timer_, motion_end_time());
	// クランプ
	else motion_timer_ = std::min(motion_timer_, LAST_FRAME(motion_end_time()));

	lerp_timer_ = std::min(lerp_timer_ + delta_time, lerp_time_);

	// イベントの実行
	for (const auto& event : events_) {
		if (event->motion_ != motion_) continue;

		if (motion_timer_ < last_time) {
			// ループ時
			if (event->time_ > last_time || event->time_ <= motion_timer_) event->callback_();
		}
		else {
			// 通常時
			if (event->time_ > last_time && event->time_ <= motion_timer_) event->callback_();
		}
	}
}

// モーションの強制変更
void AnimationLayer::forced_change_motion(GSuint motion, bool loop, float lerp_time) {
	// 補間の計算
	lerp_time_ = lerp_time;
	if (lerp_timer_ > (lerp_time_ * 0.5f)) {
		prev_motion_ = motion_;
		prev_motion_timer_ = motion_timer_;
		lerp_timer_ = 0.0f;
	}

	// モーションの変更
	motion_ = motion;
	motion_timer_ = 0.0f;
	is_looping_ = loop;
}

// ローカル用ボーン変換行列の取得
GSmatrix4 AnimationLayer::local_bone_matrix(GSuint bone_no) const {
	GSmatrix4 result{ GS_MATRIX4_IDENTITY };
	// 補間アニメーションの計算
	gsCalculateBoneAnimationLerp(
		animation_, prev_motion_, prev_motion_timer_,
		animation_, motion_, motion_timer_,
		bone_no, lerp_timer_ / lerp_time_, &result
	);
	return result;
}