#include "Behaviors/ConditionNode.h"

/*実装*/
// コンストラクタ
ConditionNode::ConditionNode(const std::function<bool()>& condition_callback, INode* node) :
	condition_callback_{ condition_callback }, DecoratorNode{ node } {
}

// 実行
NodeStatus ConditionNode::execute(float delta_time) {
	// コールバックの実行
	if (condition_callback_()) {
		// trueなら子ノードを実行
		return child_->execute(delta_time);
	}
	// falseなら失敗を返す
	return NodeStatus::Failure;
}