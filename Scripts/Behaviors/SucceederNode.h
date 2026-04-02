#ifndef BEHAVIORS_SUCCEEDER_NODE_H_
#define BEHAVIORS_SUCCEEDER_NODE_H_

#include "Behaviors/DecoratorNode.h"

// サクシードノードクラス
class SucceederNode final :
	public DecoratorNode {
public:
	SucceederNode(INode* node);
	// 実行
	NodeStatus execute(float delta_time) override;
};

#endif // BEHAVIORS_SUCCEEDER_NODE_H_