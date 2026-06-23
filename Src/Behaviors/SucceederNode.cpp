#include "Behaviors/SucceederNode.h"

/*実装*/
// コンストラクタ
SucceederNode::SucceederNode(INode* node) :
	DecoratorNode{ node } {
}

// 実行
NodeStatus SucceederNode::execute(float delta_time) {
	// 子ノード実行
	child_->execute(delta_time);
	// 結果に関わらず成功
	return NodeStatus::Success;
}