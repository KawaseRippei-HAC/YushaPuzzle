#include "Behaviors/ParallelNode.h"

/*実装*/
// コンストラクタ
ParallelNode::ParallelNode(unsigned int success_threshold, unsigned int failure_threshold) :
	success_threshold_{ success_threshold }, failure_threshold_{ failure_threshold } {
}

// 実行
NodeStatus ParallelNode::execute(float delta_time) {
	unsigned int success_count{ 0 };
	unsigned int failure_count{ 0 };

	for (INode* child : children_) {
		// 子ノード実行
		const NodeStatus status = child->execute(delta_time);

		// 成功カウント加算
		if (status == NodeStatus::Success) { ++success_count; continue; }
		// 失敗カウント加算
		if (status == NodeStatus::Failure) ++failure_count;
	}

	// カウントがしきい値以上なら結果を返す
	if (success_count >= success_threshold_) return NodeStatus::Success;
	if (failure_count >= failure_threshold_) return NodeStatus::Failure;

	// しきい値に達しない場合は実行中を返す
	return NodeStatus::Running;
}