#ifndef CHARACTERS_HEROES_MONSTER_PARAMETER_H_
#define CHARACTERS_HEROES_MONSTER_PARAMETER_H_

#include <vector>

#include "Characters/CharacterParameter.h"

/*定数*/
// 落とす経験値の基本値
static constexpr experience_t DEFAULT_EXPERIENCE_YIELD{ 50 };

// モンスターが落とすお金
static constexpr gold_t MONSTER_DROP_GOLD{ 100 };

// モンスターパラメータークラス
class MonsterParameter :
	public CharacterParameter {
public:
	// IDを受け取るコンストラクタ
	MonsterParameter(int id) :CharacterParameter(id){}
	MonsterParameter() = default;

	// モンスターが倒されたときに得られる経験値を取得する
	experience_t drop_exp(void) const noexcept;

	// お金を取得する
	gold_t monster_gold(void) noexcept;

private:
	// モンスターのお金
	gold_t monster_drop_gold_{ MONSTER_DROP_GOLD };
	// モンスターの落とす経験値
	experience_t experience_yield_{ DEFAULT_EXPERIENCE_YIELD };
};

#include "Inlines/MonsterParameter.inl"

#endif // CHARACTERS_HEROES_HERO_PARAMETER_H_