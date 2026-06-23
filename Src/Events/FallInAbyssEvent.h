#ifndef EVENTS_FALL_IN_ABYS_EVENT_H_
#define EVENTS_FALL_IN_ABYS_EVENT_H_

#include "Interfaces/ITileEvent.h"
#include "Interfaces/ICharacterMover.h"

// 奈落落下イベントクラス
class FallInAbyssEvent :
	public ITileEvent {
public:
	FallInAbyssEvent(struct EventContext&) {}

	// 実行
	void execute(float delta_time, CharacterBase& character) override;

	// 終了フラグ
	bool is_finished(void) const noexcept override;

	// 移動処理の取得
	ICharacterMover* mover(void) noexcept override;
	// 移動中有効フラグ
	bool is_activate_in_moving(CharacterBase& character) const noexcept override;

private:
	// 奈落落下移動処理クラス
	class FallInAbyssMover :
		public ICharacterMover {
	public:
		// 接近の初期化
		void init_approach(const GSvector3& from, const GSvector3& to) override;

		// 接近時処理
		void on_approach(float delta_time, CharacterBase& character) override;
		// 離脱時処理
		void on_departure(float delta_time, CharacterBase& character) override {}

		// 完了フラグ
		bool is_completed(void) const noexcept override;

	private:
		// 移動タイマー
		float timer_{ 0.0f };
		// 出発座標
		GSvector3 from_position_{ 0.0f, 0.0f, 0.0f };
		// 目標座標
		GSvector3 to_position_{ 0.0f, 0.0f, 0.0f };
		// 進行方向クォータニオン
		GSquaternion look_rotation_{ 0.0f, 0.0f, 0.0f, 0.0f };
		// 落下中フラグ
		bool is_falling_{ false };
	};

private:
	// 終了フラグ
	bool is_finished_{ false };
	// 奈落落下移動処理
	inline static FallInAbyssMover fall_in_abyss_mover_;
};

#endif // EVENTS_FALL_IN_ABYS_EVENT_H_