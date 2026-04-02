#ifndef CHARACTERS_MONSTERS_MID_BOSS_TEMPLATE_H_
#define CHARACTERS_MONSTERS_MID_BOSS_TEMPLATE_H_

#include <type_traits>

#include "Characters/Monsters/MidBossCharacter.h"

template<class TStrategy, MonsterData data>
	requires std::is_base_of_v<IBattleActor, TStrategy>
class MidBossTemplate :
	public MidBossCharacter {
public:
	MidBossTemplate() :
		MidBossCharacter(data) {
		set_battle_strategy(std::make_unique<TStrategy>(this));
	}
};

#endif // CHARACTERS_MONSTERS_MID_BOSS_TEMPLATE_H_
