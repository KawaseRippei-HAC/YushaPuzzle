#ifndef TWEENS_TEMPLATE_UNIT_HPP_
#define TWEENS_TEMPLATE_UNIT_HPP_

#include "Tweens/TemplateUnit.h"

/*実装*/
// コンストラクタ
template<typename _Type>
TemplateUnit<_Type>::TemplateUnit(
	optimal_param_t<_Type> from, optimal_param_t<_Type> to,
	float duration, const std::function<void(optimal_param_t<_Type>)>& update_callback) :
	TweenUnit{ duration },
	from_{ from }, to_{ to },
	update_callback_{ update_callback } {
}

// 更新時処理
template<typename _Type>
void TemplateUnit<_Type>::on_update(float progress) {
	update_callback_(LERP(progress, from_, to_));
}

#endif // TWEENS_TEMPLATE_UNIT_HPP_