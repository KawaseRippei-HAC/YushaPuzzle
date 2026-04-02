#ifndef UTILITIES_STATE_MACHINE_H_
#define UTILITIES_STATE_MACHINE_H_

#include <unordered_map>

#include "Interfaces/IState.h"
#include "Utilities/Noncopyable.h"

template<class TContext>
class StateMachine final:
	private Noncopyable<StateMachine<TContext>> {
public:
	explicit StateMachine(TContext& owner);

	// 更新
	void update(float delta_time);

	// 状態の登録
	template<class TState, typename ...Args>
		requires std::is_base_of_v<IState<TContext>, TState>
	void register_state(unsigned char id, Args ...args);
	// 状態の変更
	void change_state(unsigned char id);

	// 時間の取得
	float time(void) const noexcept;

private:
	// 状態タイマー
	float state_timer_{ 0.0f };
	// オーナー
	std::reference_wrapper<TContext> context_;
	// 現在の状態
	IState<TContext>* current_state_{ nullptr };
	// 状態マップ
	std::unordered_map<unsigned char, std::unique_ptr<IState<TContext>>> state_map_;
};

#include "Utilities/StateMachine.hpp"

#endif // UTILITIES_STATE_MACHINE_H_