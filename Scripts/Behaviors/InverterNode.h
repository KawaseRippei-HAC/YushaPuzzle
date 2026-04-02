#ifndef BEHAVIORS_INVERTER_NODE_H_
#define BEHAVIORS_INVERTER_NODE_H_

#include "Behaviors/DecoratorNode.h"

// インバーターノードクラス
class InverterNode final :
	public DecoratorNode {
public:
	InverterNode(INode* node);
	// 実行
	NodeStatus execute(float delta_time) override;
};

#endif // BEHAVIORS_INVERTER_NODE_H_