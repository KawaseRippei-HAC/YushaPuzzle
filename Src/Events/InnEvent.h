#ifndef EVENTS_INN_EVENT_H_
#define EVENTS_INN_EVENT_H_

#include "Interfaces/ITileEvent.h"
#include "Interfaces/ICharacterMover.h"

/*定数*/
// 宿泊料金
inline constexpr int ACCOMMODATION_FEE{ 100 };

// 奈落落下イベントクラス
class InnEvent :
	public ITileEvent {
public:
	InnEvent(struct EventContext&) {}

	// 開始時処理
	void on_enter(CharacterBase& character) override;
	// 実行
	void execute(float delta_time, CharacterBase& context) override;
	// 終了時処理
	void on_exit(CharacterBase& character) override;

	// オブジェクトの描画
	void draw_object(const GSmatrix4& matrix) const override;

	// イベント種別の取得
	EventType type(void) const noexcept override;
	// アイコンの取得
	GSuint icon(void) const noexcept override;

	// 終了フラグ
	bool is_finished(void) const noexcept override;
	// 移動中有効フラグ
	bool is_activate_in_moving(CharacterBase& character) const noexcept override;

	// 移動処理の取得
	ICharacterMover* mover(void) noexcept override;

private:

	void hp_max_execute(float delta_time, CharacterBase& character);
	void low_on_gold_execute(float delta_time, CharacterBase& character);
	void can_pay_execute(float delta_time, CharacterBase& character);

	enum {
		HpMax,		// HPが満タン
		LowOnGold,	// ゴールドが足りない
		CanPay,		// ゴールドが払える
	};

	// 条件ステート
	int state_{ 0 };

	// 終了フラグ
	bool is_finished_{ false };

	// タイマ
	float timer_{ 0 };
};

#endif // EVENTS_INN_EVENT_H_