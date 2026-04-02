#ifndef BEHAVIORS_COMPOSITE_NODE_H_
#define BEHAVIORS_COMPOSITE_NODE_H_

#include <vector>
#include <algorithm>

#include "Interfaces/INode.h"

// 制御ノード抽象クラス
class CompositeNode :
	public INode {
public:
	virtual ~CompositeNode(void);

	// リセット
	virtual void reset(void) override;

	// 子ノードの追加
	void add_child(INode* node);
	// 子ノード配列の取得
	const std::vector<INode*>& children(void) const noexcept { return children_; }

protected:
	// 子ノード配列
	std::vector<INode*> children_;
};

#endif // BEHAVIORS_COMPOSITE_NODE_H_