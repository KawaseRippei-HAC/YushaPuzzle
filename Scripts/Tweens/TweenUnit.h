#ifndef TWEENS_TWEEN_UNIT_H_
#define TWEENS_TWEEN_UNIT_H_

#include <string>
#include <functional>

// イージング種別
enum class Ease : unsigned char {
	// 線形補間
	Linear = 0,

	InBack,
	InCirc,
	InCubic,
	InExpo,
	InQuad,
	InQuart,
	InQuint,
	InSine,

	OutBack,
	OutBounce,
	OutCirc,
	OutCubic,
	OutElastic,
	OutExpo,
	OutQuad,
	OutQuart,
	OutQuint,
	OutSine,

	InOutBack,
	InOutCirc,
	InOutCubic,
	InOutExpo,
	InOutQuad,
	InOutQuart,
	InOutQuint,
	InOutSine,

	Punch,
};

// Tweenユニットクラス
class TweenUnit {
public:
	TweenUnit(float duration);
	virtual ~TweenUnit(void) = default;

	// 更新
	void update(float delta_time);

	// 遅延時間
	TweenUnit& delay(float delay) noexcept { delay_ = delay; return *this; }
	// イージングの設定
	TweenUnit& ease(Ease ease_type) noexcept { ease_type_ = ease_type; return *this; }
	// オーバーシュートの設定
	TweenUnit& overshoot(float overshoot) noexcept { overshoot_ = overshoot; return *this; }
	// 完了時コールバックの設定
	TweenUnit& on_complate(const std::function<void(void)>& complate_callback) noexcept {
		complete_callback_ = complate_callback; return *this;
	}

	// 名前の取得
	const std::string& name(void) const noexcept { return name_; }
	// 名前の設定
	TweenUnit& name(const std::string& name) noexcept { name_ = name; return *this; }

	// 終了フラグ
	bool is_finished(void) const noexcept { return is_finished_; }

protected:
	// 更新時処理
	virtual void on_update(float progress) = 0;

private:
	// イージングの適用
	static float apply_ease(float value, Ease ease_type, float overshoot);

private:
	// 名前
	std::string name_{ "" };

	// 遅延時間
	float delay_{ 0.0f };

	// 持続時間
	float duration_{ 0.0f };
	// 経過時間
	float elapsed_{ 0.0f };

	// イージング種別
	Ease ease_type_{ Ease::Linear };
	// オーバーシュート
	float overshoot_{ 1.70158f };

	// 完了時コールバック
	std::function<void(void)> complete_callback_{ nullptr };

	// 終了フラグ
	bool is_finished_{ false };
};

#endif // TWEENS_TWEEN_UNIT_H_