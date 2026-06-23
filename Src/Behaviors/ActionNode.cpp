#include "Behaviors/ActionNode.h"

/*実装*/
// コンストラクタ
ActionNode::ActionNode(IAction* action) :
	action_{ action } {
}

// デストラクタ
ActionNode::~ActionNode() {
	if (action_) delete action_;
	action_ = nullptr;
}

// 実行
NodeStatus ActionNode::execute(float delta_time) {
	if (!action_) return NodeStatus::Failure;

	// ノード状態が実行中でないなら、実行時処理を実行
	if (status_ != NodeStatus::Running) action_->on_enter();

	status_ = action_->run(delta_time);

	// アクション実行後、ノード状態が実行中でないなら、終了時処理を実行
	if (status_ != NodeStatus::Running) action_->on_exit();

	return status_;
}

// リセット
void ActionNode::reset() {
	status_ = NodeStatus::Failure;
}