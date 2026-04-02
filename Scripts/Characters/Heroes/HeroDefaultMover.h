#ifndef CHARACTERS_HEROES_HERO_DEFAULT_MOVER_H_
#define CHARACTERS_HEROES_HERO_DEFAULT_MOVER_H_

#include <GSquaternion.h>

#include "Interfaces/ICharacterMover.h"

// 勇者通常移動処理クラス
class HeroDefaultMover :
	public ICharacterMover {
public:
	// 初期化
	void initialize(const GSvector3& from, const GSvector3& to) override;

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

#endif // CHARACTERS_HEROES_HERO_DEFAULT_MOVER_H_