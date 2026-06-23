#include "Behaviors/SequenceNode.h"

/*実装*/
// 実行
NodeStatus SequenceNode::execute(float delta_time) {
	while (current_child_index_ < this->children_.size()) {
		// 子ノードを実行
		INode* child = children_.at(current_child_index_);
		const NodeStatus status = child->execute(delta_time);

		// 失敗したらリセットして返す
		if (status == NodeStatus::Failure) {
			this->reset();
			return NodeStatus::Failure;
		}
		// 実行中ならそのまま返す
		else if (status == NodeStatus::Running) return NodeStatus::Running;

		// インデックスを加算
		++current_child_index_;
	}

	// 全て成功した時のみ成功を返す
	this->reset();
	return NodeStatus::Success;
}

// リセット
void SequenceNode::reset() {
	current_child_index_ = 0;
	this->CompositeNode::reset();
}