#ifndef BEHAVIORS_SELECTOR_NODE_H_
#define BEHAVIORS_SELECTOR_NODE_H_

#include "Behaviors/CompositeNode.h"

// セレクターノードクラス
class SelectorNode final :
	public CompositeNode {
public:
	// 実行
	NodeStatus execute(float delta_time) override;
	// リセット
	void reset(void) override;

private:
	// 現在実行中の子ノードインデックス
	unsigned int current_child_index_{ 0 };
};

#endif // BEHAVIORS_SELECTOR_NODE_H_