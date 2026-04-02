#ifndef BEHAVIORS_FUNCTION_NODE_H_
#define BEHAVIORS_FUNCTION_NODE_H_

#include <type_traits>

#include "Interfaces/INode.h"

// ファンクションノード
template<typename TFunc>
	requires std::is_invocable_r_v<NodeStatus, TFunc>
class FunctionNode :
	public INode {
public:
	FunctionNode(TFunc&& callback);

	// 実行
	NodeStatus execute(float delta_time) override;

private:
	TFunc callback_;
};

#include "Behaviors/FunctionNode.hpp"

#endif // BEHAVIORS_FUNCTION_NODE_H_