#ifndef EVENTS_ENCOUNT_MOVER_H_
#define EVENTS_ENCOUNT_MOVER_H_

#include <GSquaternion.h>

#include "Interfaces/ICharacterMover.h"

// 遭遇時移動処理クラス
class EncountMover :
	public ICharacterMover {
public:
	// 接近の初期化
	void init_approach(const GSvector3& from, const GSvector3& to) override;
	// 離脱の初期化
	void init_departure(const GSvector3& from, const GSvector3& to) override;

	// 接近時処理
	void on_approach(float delta_time, CharacterBase& character) override;
	// 離脱時処理
	void on_departure(float delta_time, CharacterBase& character) override;

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
};

// 建物の入退出移動処理
static EncountMover encount_mover;

#endif // EVENTS_BUILDING_MOVER_H_