#ifndef BEHAVIORS_BEHAVIOR_TREE_H_
#define BEHAVIORS_BEHAVIOR_TREE_H_

#include <unordered_map>

#include "Interfaces/INode.h"
#include "Utilities/Noncopyable.h"
#include "Behaviors/SequenceNode.h"
#include "Behaviors/SelectorNode.h"
#include "Behaviors/ParallelNode.h"
#include "Behaviors/InverterNode.h"
#include "Behaviors/SucceederNode.h"
#include "Behaviors/RepeaterNode.h"
#include "Behaviors/ConditionNode.h"
#include "Behaviors/ActionNode.h"
#include "Behaviors/FunctionNode.h"

// ビヘイビアツリークラス
class BehaviorTree final :
	private Noncopyable<BehaviorTree> {
public:
	BehaviorTree(void) = default;
	BehaviorTree(INode* root);
	~BehaviorTree(void);

	// 更新
	void update(float delta_time);

	// リセット
	void reset(void);

	// 消去
	void clear(void);

	// ルートノードの登録
	void register_root(INode* node);
	// ノードの追加
	bool add_node(unsigned char id, INode* node);
	// ノードの追加
	bool add_node(unsigned char parent_id, unsigned char id, INode* node);

	// ステータスの取得
	NodeStatus status(void) const noexcept { return status_; }

private:
	// ノード追加の適用
	bool apply_add_node(INode* parent, unsigned char id, INode* node);

private:
	// ルートノード
	INode* root_node_{ nullptr };
	// ノードマップ
	std::unordered_map<unsigned char, INode*> node_map_;
	// ステータス
	NodeStatus status_{ NodeStatus::Running };
};

#endif // BEHAVIORS_BEHAVIOR_TREE_H_