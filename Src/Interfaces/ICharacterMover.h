#ifndef INTERFACES_IMOVE_OVERRIDER_H_
#define INTERFACES_IMOVE_OVERRIDER_H_

#include <GStransform.h>

#include "Characters/CharacterBase.h"

// キャラクター移動処理抽象インターフェース
class ICharacterMover {
public:
	virtual ~ICharacterMover() = default;

	// 初期化
	virtual void initialize(const GSvector3& from, const GSvector3& to) {}

	// 接近の初期化
	virtual void init_approach(const GSvector3& from, const GSvector3& to) { this->initialize(from, to); }
	// 離脱の初期化
	virtual void init_departure(const GSvector3& from, const GSvector3& to) { this->initialize(from, to); }

	// 接近時処理
	virtual void on_approach(float delta_time, CharacterBase& character) = 0;
	// 離脱時処理
	virtual void on_departure(float delta_time, CharacterBase& character) = 0;

	// 完了フラグ
	virtual bool is_completed(void) const noexcept = 0;
};

#endif // INTERFACES_IMOVE_OVERRIDER_H_