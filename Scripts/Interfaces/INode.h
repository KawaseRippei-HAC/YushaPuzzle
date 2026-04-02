#ifndef INTERFACES_INODE_H_
#define INTERFACES_INODE_H_

#include <iostream>

#include "Utilities/Noncopyable.h"

// ノード状態
enum class NodeStatus : unsigned char {
	// 成功
	Success,
	// 失敗
	Failure,
	// 実行中
	Running,
};

// ビヘイビアノード抽象インターフェース
class INode :
	private Noncopyable<INode> {
public:
	virtual ~INode(void) = default;

	// 実行
	virtual NodeStatus execute(float delta_time) = 0;

	// リセット
	virtual void reset(void) {}
};

#endif // INTERFACES_INODE_H_