#include "Behaviors/RepeaterNode.h"

/*定義*/
// リピートすべきか
#define SHOULD_REPEAT (repeat_count_ == NULL) || (current_repeat_count_ < repeat_count_)

/*実装*/
// コンストラクタ
RepeaterNode::RepeaterNode(unsigned int repeat, INode* node) :
	DecoratorNode{ node }, repeat_count_{ repeat }, current_repeat_count_{ 0 } {
}

// 実行
NodeStatus RepeaterNode::execute(float delta_time) {
	if (SHOULD_REPEAT) {
		// 子ノード実行
		const NodeStatus status = child_->execute(delta_time);

		// 実行中ならそのまま返す
		if (status == NodeStatus::Running) return NodeStatus::Running;

		// 子ノードが完了したらカウントを加算
		++current_repeat_count_;

		// まだリピートすべきなら実行中を返す
		if (SHOULD_REPEAT) return NodeStatus::Running;
	}

	// 完了したらリセットして成功を返す
	this->reset();
	return NodeStatus::Success;
}

// リセット
void RepeaterNode::reset() {
	current_repeat_count_ = 0;
	this->DecoratorNode::reset();
}