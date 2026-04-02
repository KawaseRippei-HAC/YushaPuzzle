#include "Behaviors/CompositeNode.h"

/*実装*/
// デストラクタ
CompositeNode::~CompositeNode() {
	for (INode* child : children_) {
		delete child;
		child = nullptr;
	}
}

// リセット
void CompositeNode::reset() {
	for (INode* child : children_) {
		child->reset();
	}
}

// 子ノードの追加
void CompositeNode::add_child(INode* node) {
	children_.push_back(node);
}