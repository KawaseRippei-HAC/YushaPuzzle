#include "Tweens/TweenUnit.h"

#include <gslib.h>
#include <GSeasing.h>

/*実装*/
// コンストラクタ
TweenUnit::TweenUnit(float duration) :
	duration_{ duration } {
}

// 更新
void TweenUnit::update(float delta_time) {
	if (delay_ > 0.0f) {
		delay_ -= delta_time;
		if (delay_ > 0.0f) return;
		else elapsed_ = -delay_;
	}

	// 各種値の計算
	elapsed_ += delta_time;
	const float progress = std::min(elapsed_ / duration_, 1.0f);
	const float eased = apply_ease(progress, ease_type_, overshoot_);

	// 更新時処理の実行
	this->on_update(eased);
	is_finished_ = (progress >= 1.0f);

	// 完了時コールバックの実行
	if (is_finished_ && complete_callback_) complete_callback_();
}

// イージングの適用
float TweenUnit::apply_ease(float value, Ease ease_type, float overshoot) {
	switch (ease_type) {
	default: return value;
	case Ease::Linear: return value;

	case Ease::InBack: return gsEasingInBack(value, overshoot);
	case Ease::InCirc: return gsEasingInCirc(value);
	case Ease::InCubic: return gsEasingInCubic(value);
	case Ease::InExpo: return gsEasingInExpo(value);
	case Ease::InQuad: return gsEasingInQuad(value);
	case Ease::InQuart: return gsEasingInQuart(value);
	case Ease::InQuint: return gsEasingInQuint(value);
	case Ease::InSine: return gsEasingInSine(value);

	case Ease::OutBack: return gsEasingOutBack(value, overshoot);
	case Ease::OutBounce: return gsEasingOutBounce(value);
	case Ease::OutCirc: return gsEasingOutCirc(value);
	case Ease::OutCubic: return gsEasingOutCubic(value);
	case Ease::OutElastic: {
		if (value <= 0.0f) return 0.0f;
		if (value >= 1.0f) return 1.0f;
		return 1.0f - std::cos(value * 10.0f * overshoot) * std::pow(1.0f - value, 3.0f);
	}
	case Ease::OutExpo: return gsEasingOutExpo(value);
	case Ease::OutQuad: return gsEasingOutQuad(value);
	case Ease::OutQuart: return gsEasingOutQuart(value);
	case Ease::OutQuint: return gsEasingOutQuint(value);
	case Ease::OutSine: return gsEasingOutSine(value);

	case Ease::InOutBack: return gsEasingInOutBack(value, overshoot);
	case Ease::InOutCirc: return gsEasingInOutCirc(value);
	case Ease::InOutCubic: return gsEasingInOutCubic(value);
	case Ease::InOutExpo: return gsEasingInOutExpo(value);
	case Ease::InOutQuad: return gsEasingInOutQuad(value);
	case Ease::InOutQuart: return gsEasingInOutQuart(value);
	case Ease::InOutQuint: return gsEasingInOutQuint(value);
	case Ease::InOutSine: return gsEasingInOutSine(value);

	case Ease::Punch: {
		if (value <= 0.0f || value >= 1.0f) return 0.0f;
		const float peak = 0.25f / overshoot;
		const float a = CLAMP((value - 1.0f) / (peak - 1.0f), 0.0f, 1.0f);
		const float b = a * a * (3 - 2 * a);
		return std::sin(value * 6.28319530f * overshoot) * b;
	}
	}
}