#ifndef CHARACTERS_MONSTERS_DEMON_KING_TABLE_H_
#define CHARACTERS_MONSTERS_DEMON_KING_TABLE_H_

#include "Defines/Assets.h"
#include "Utilities/ObjectTable.h"
#include "Characters/Monsters/DemonKingCharacter.h"
#include "Characters/Monsters/DemonKingTemplate.h"
#include "Characters/Monsters/DemonKingDefaultBattleStrategy.h"

// 魔王データ
static constexpr MonsterData DEMON_KING_DATA{
	.mesh_id = Mesh::DemonKing,
	.params = {
		.attack = 74,
		.defense = 69,
		.speed = 34,
		.luck = 40,
		.max_hp = 410 },
};

// 魔王テーブル
using DemonKingTable = ObjectTable<
	DemonKingCharacter* (*)(),
	DemonKingTemplate<DemonKingDefaultBattleStrategy, DEMON_KING_DATA>
>;

#endif // CHARACTERS_MONSTERS_DEMON_KING_TABLE_H_