#ifndef INTERFACES_ISTATE_H_
#define INTERFACES_ISTATE_H_

#include "Utilities/Noncopyable.h"

template<class TContext>
class IState :
	private Noncopyable<IState<TContext>> {
public:
	virtual ~IState() = default;

	// 更新時処理
	virtual void on_update(float delta_time, TContext& context) = 0;

	// 遷移時処理
	virtual void on_enter(TContext& context) {}
	// 終了時処理
	virtual void on_exit(TContext& context) { is_finished_ = false; }

	// 状態の変更
	void change_state(unsigned char next) { next_ = next; is_finished_ = true; }

	// 次の状態の取得
	unsigned char next(void) const noexcept { return next_; }
	// 終了フラグの取得
	bool is_finished(void) const noexcept { return is_finished_; }

protected:
	// 次の状態ID
	unsigned char next_{ 0 };
	// 終了フラグ
	bool is_finished_{ false };
};

#endif // INTERFACES_ISTATE_H_