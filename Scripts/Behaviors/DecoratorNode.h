#ifndef BEHAVIORS_DECORATOR_NODE_H_
#define BEHAVIORS_DECORATOR_NODE_H_

#include "Interfaces/INode.h"

// 装飾ノード抽象クラス
class DecoratorNode :
	public INode {
public:
	DecoratorNode(INode* node);
	virtual ~DecoratorNode(void);

	// リセット
	virtual void reset(void) override { child_->reset(); }
	// 子ノードの取得
	INode* child(void) const noexcept { return child_; }

protected:
	// 子ノード
	INode* child_{ nullptr };
};

#endif // BEHAVIORS_DECORATOR_NODE_H_