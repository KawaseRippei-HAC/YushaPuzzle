#ifndef BEHAVIORS_FUNCTION_NODE_HPP_
#define BEHAVIORS_FUNCTION_NODE_HPP_

#include "Behaviors/FunctionNode.h"

/*テンプレート関数実装*/
// コンストラクタ
template<typename TFunc>
	requires std::is_invocable_r_v<NodeStatus, TFunc>
FunctionNode<TFunc>::FunctionNode(TFunc&& callback) :
	callback_{ callback } {
}

// コンストラクタ
template<typename TFunc>
	requires std::is_invocable_r_v<NodeStatus, TFunc>
NodeStatus FunctionNode<TFunc>::execute(float delta_time) {
	return callback_();
}

#endif // BEHAVIORS_FUNCTION_NODE_HPP_