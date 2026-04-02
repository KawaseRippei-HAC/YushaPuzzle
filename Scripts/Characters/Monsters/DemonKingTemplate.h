#ifndef CHARACTERS_MONSTERS_DEMON_KING_TEMPLATE_H_
#define CHARACTERS_MONSTERS_DEMON_KING_TEMPLATE_H_

#include <type_traits>

#include "Characters/Monsters/DemonKingCharacter.h"

template<class TStrategy, MonsterData data>
	requires std::is_base_of_v<IBattleActor, TStrategy>
class DemonKingTemplate :
	public DemonKingCharacter {
public:
	DemonKingTemplate() :
		DemonKingCharacter(data) {
		set_battle_strategy(std::make_unique<TStrategy>(this));
	}
};

#endif // CHARACTERS_MONSTERS_DEMON_KING_TEMPLATE_H_