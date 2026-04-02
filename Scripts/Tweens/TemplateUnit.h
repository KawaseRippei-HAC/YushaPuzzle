#ifndef TWEENS_TEMPLATE_UNIT_H_
#define TWEENS_TEMPLATE_UNIT_H_

#include <Defines/Define.h>
#include <Tweens/TweenUnit.h>

// テンプレートTweenユニットクラス
template<typename _Type = float>
class TemplateUnit :
	public TweenUnit {
public:
	TemplateUnit(
		optimal_param_t<_Type> from, optimal_param_t<_Type> to,
		float duration, const std::function<void(optimal_param_t<_Type>)>& update_callback
	);
	virtual ~TemplateUnit() = default;

protected:
	// 更新時処理
	virtual void on_update(float progress) override;

	// 開始値
	_Type from_;
	// 終了値
	_Type to_;
	// 更新時コールバック
	std::function<void(optimal_param_t<_Type>)> update_callback_{ nullptr };
};

#include "Tweens/TemplateUnit.hpp"

#endif // TWEENS_TEMPLATE_UNIT_H_