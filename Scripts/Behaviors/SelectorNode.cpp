#include "Behaviors/SelectorNode.h"

/*実装*/
// 実行
NodeStatus SelectorNode::execute(float delta_time) {
	while (current_child_index_ < this->children_.size()) {
		// 子ノードを実行
		INode* child = children_.at(current_child_index_);
		const NodeStatus status = child->execute(delta_time);

		// 成功したらリセットして返す
		if (status == NodeStatus::Success) {
			this->reset();
			return NodeStatus::Success;
		}
		// 実行中ならそのまま返す
		else if (status == NodeStatus::Running) return NodeStatus::Running;

		// インデックスを加算
		++current_child_index_;
	}

	// 全て失敗した時のみ失敗を返す
	this->reset();
	return NodeStatus::Failure;
}

// リセット
void SelectorNode::reset() {
	current_child_index_ = 0;
	this->CompositeNode::reset();
}