#ifndef BEHAVIORS_REPEATER_NODE_H_
#define BEHAVIORS_REPEATER_NODE_H_

#include "Behaviors/DecoratorNode.h"

// リピーターノードクラス
class RepeaterNode final :
	public DecoratorNode {
public:
	RepeaterNode(unsigned int repeat, INode* node);
	// 実行
	NodeStatus execute(float delta_time) override;
	// リセット
	void reset(void) override;

private:
	// リピート回数
	unsigned int repeat_count_{ 0 };
	// 現在のリピート回数
	unsigned int current_repeat_count_{ 0 };
};

#endif // BEHAVIORS_REPEATER_NODE_H_