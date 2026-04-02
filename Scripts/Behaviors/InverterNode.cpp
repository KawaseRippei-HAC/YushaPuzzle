#include "Behaviors/InverterNode.h"

/*実装*/
// コンストラクタ
InverterNode::InverterNode(INode* node) :
	DecoratorNode{ node } {
}

// 実行
NodeStatus InverterNode::execute(float delta_time) {
	// 子ノード実行
	const NodeStatus status = child_->execute(delta_time);

	// 結果が成功なら失敗にして返す
	if (status == NodeStatus::Success) return NodeStatus::Failure;
	// 結果が失敗なら成功にして返す
	if (status == NodeStatus::Failure) return NodeStatus::Success;

	// 実行中はそのまま返す
	return NodeStatus::Running;
}