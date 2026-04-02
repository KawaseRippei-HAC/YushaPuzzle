#ifndef INTERFACES_IACTION_H_
#define INTERFACES_IACTION_H_

#include "Utilities/Noncopyable.h"

enum class NodeStatus : unsigned char;

// アクション抽象インターフェース
class IAction :
	private Noncopyable<IAction> {
public:
	virtual ~IAction(void) = default;

	// 実行
	virtual NodeStatus run(float delta_time) = 0;
	// 実行時処理
	virtual void on_enter(void) {}
	// 終了時処理
	virtual void on_exit(void) {}
};

#endif // INTERFACES_IACTION_H_