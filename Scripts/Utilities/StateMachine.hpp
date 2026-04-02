#ifndef UTILITIES_STATE_MACHINE_HPP_
#define UTILITIES_STATE_MACHINE_HPP_

#include "Utilities/StateMachine.h"

/*テンプレート関数実装*/
// コンストラクタ
template<class TContext>
StateMachine<TContext>::StateMachine(TContext& context) :
	context_{ context } {
}

// 更新
template<class TContext>
void StateMachine<TContext>::update(float delta_time) {
	if (!current_state_) return;
	state_timer_ += delta_time;
	current_state_->on_update(delta_time, context_);
	if (current_state_->is_finished()) change_state(current_state_->next());
}

// 状態の登録
template<class TContext>
template<class TState, typename ...Args>
	requires std::is_base_of_v<IState<TContext>, TState>
void StateMachine<TContext>::register_state(unsigned char id, Args ...args) {
	state_map_[id] = std::make_unique<TState>(std::forward<Args>(args)...);
}

// 状態の変更
template<class TContext>
void StateMachine<TContext>::change_state(unsigned char id) {
	const auto it = state_map_.find(id);
	if (it != state_map_.end()) {
		if (current_state_) current_state_->on_exit(context_);
		current_state_ = it->second.get();
		state_timer_ = 0.0f;
		if (!current_state_) return;
		current_state_->on_enter(context_);
		if (current_state_->is_finished()) change_state(current_state_->next());
	}
	else current_state_ = nullptr;
}

// 時間の取得
template<class TContext>
inline float StateMachine<TContext>::time() const noexcept {
	return state_timer_;
}

#endif // UTILITIES_STATE_MACHINE_HPP_