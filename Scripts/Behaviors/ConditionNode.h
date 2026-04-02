#ifndef BEHAVIORS_CONDITION_NODE_H_
#define BEHAVIORS_CONDITION_NODE_H_

#include <functional>

#include "Behaviors/DecoratorNode.h"

// コンディションノードクラス
class ConditionNode final :
	public DecoratorNode {
public:
	ConditionNode(const std::function<bool(void)>& condition_callback, INode* node);
	// 実行
	NodeStatus execute(float delta_time) override;

private:
	// 条件コールバック
	std::function<bool(void)> condition_callback_{ nullptr };
};

#endif // BEHAVIORS_CONDITION_NODE_H_