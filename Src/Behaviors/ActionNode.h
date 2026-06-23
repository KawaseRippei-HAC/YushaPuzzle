#ifndef BEHAVIORS_ACTION_NODE_H_
#define BEHAVIORS_ACTION_NODE_H_

#include "Interfaces/INode.h"
#include "Interfaces/IAction.h"

// アクションノードクラス
class ActionNode final :
	public INode {
public:
	ActionNode(IAction* action);
	~ActionNode(void);

	// 実行
	NodeStatus execute(float delta_time) override;
	// リセット
	void reset(void) override;

private:
	// アクション
	IAction* action_{ nullptr };
	// ノード状態
	NodeStatus status_{ NodeStatus::Failure };
};

#endif // BEHAVIORS_ACTION_NODE_H_