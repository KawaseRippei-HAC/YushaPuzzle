#ifndef BEHAVIORS_PARALLEL_NODE_H_
#define BEHAVIORS_PARALLEL_NODE_H_

#include "Behaviors/CompositeNode.h"

// パラレルノードクラス
class ParallelNode final :
	public CompositeNode {
public:
	ParallelNode(unsigned int success_threshold, unsigned int failure_threshold);
	// 実行
	NodeStatus execute(float delta_time) override;

private:
	// 成功しきい値
	unsigned int success_threshold_{ 0 };
	// 失敗しきい値
	unsigned int failure_threshold_{ 0 };
};

#endif // BEHAVIORS_PARALLEL_NODE_H_