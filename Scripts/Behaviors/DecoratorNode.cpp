#include "Behaviors/DecoratorNode.h"

/*実装*/
// コンストラクタ
DecoratorNode::DecoratorNode(INode* node) :
	child_{ node } {
}

// デストラクタ
DecoratorNode::~DecoratorNode() {
	if (child_) delete child_;
	child_ = nullptr;
}