#include "Behaviors/BehaviorTree.h"

/*実装*/
// コンストラクタ
BehaviorTree::BehaviorTree(INode* root) :
	root_node_{ root } {
}

// デストラクタ
BehaviorTree::~BehaviorTree() {
	this->clear();
}

// 更新
void BehaviorTree::update(float delta_time) {
	if (!root_node_) return;
	status_ = root_node_->execute(delta_time);
}

// リセット
void BehaviorTree::reset() {
	if (root_node_) root_node_->reset();
	status_ = NodeStatus::Running;
}

// 消去
void BehaviorTree::clear() {
	node_map_.clear();
	this->register_root(nullptr);
	this->reset();
}

// ルートノードの登録
void BehaviorTree::register_root(INode* node) {
	if (node == root_node_) return;
	if (root_node_) delete root_node_;
	root_node_ = node;
}

// ノードの追加
bool BehaviorTree::add_node(unsigned char id, INode* node) {
	return this->apply_add_node(root_node_, id, node);
}

// ノードの追加
bool BehaviorTree::add_node(unsigned char parent_id, unsigned char id, INode* node) {
	auto node_it = node_map_.find(parent_id);
	if (node_it == node_map_.end()) { delete node; node = nullptr; return false; }
	return this->apply_add_node(node_it->second, id, node);
}

// ノード追加の適用
bool BehaviorTree::apply_add_node(INode* parent, unsigned char id, INode* node) {
	if (!node) return false;
	if (!parent) { delete node; node = nullptr; return false; }
	if (node_map_.find(id) != node_map_.end()) { delete node; node = nullptr; return false; }

	// 親ノードが制御ノードなら、子にノードを追加
	if (CompositeNode* composite = dynamic_cast<CompositeNode*>(parent)) {
		node_map_[id] = node; composite->add_child(node); return true;
	}
	// 親ノードが装飾ノードなら、装飾ノードの子にノード追加を試みる
	else if (DecoratorNode* decorator = dynamic_cast<DecoratorNode*>(parent)) {
		return this->apply_add_node(decorator->child(), id, node);
	}
	// 親ノードが子を持てないなら、deleteする
	else { delete node; node = nullptr; }

	return false;
}