#ifndef TWEENS_TWEEN_H_
#define TWEENS_TWEEN_H_

#include <list>

#include "Tweens/TweenUnit.h"
#include "Tweens/TemplateUnit.h"

// Tweenクラス
class Tween final {
public:
	// 更新
	static void update(float delta_time);
	// 消去
	static void clear(void);

	// Tweenの開始
	template<typename _Type = float>
	static TweenUnit& start(
		optimal_param_t<_Type> from, optimal_param_t<_Type> to, float duration,
		const std::function<void(optimal_param_t<_Type>)>& update_callback);

	// キャンセル
	static void cancel(const TweenUnit& unit);
	// キャンセル
	static void cancel(const std::string& name);

private:
	/*インスタンス生成禁止*/
	Tween() = delete;
	// ユニットの追加
	static void add_unit(TweenUnit* unit) { unit_list_.push_back(unit); }

private:
	// Tweenユニットリスト
	static std::list<TweenUnit*> unit_list_;
};

#pragma region /*テンプレート関数実装*/

// Tweenの開始
template<typename _Type>
TweenUnit& Tween::start(
	optimal_param_t<_Type> from, optimal_param_t<_Type> to, float duration,
	const std::function<void(optimal_param_t<_Type>)>& update_callback) {
	TweenUnit* unit = new TemplateUnit<_Type>{ from, to, duration, update_callback };
	Tween::add_unit(unit);
	return *unit;
}

#pragma endregion

#endif // TWEENS_TWEEN_H_