#ifndef CHARACTERS_MONSTERS_MID_BOSS_TABLE_H_
#define CHARACTERS_MONSTERS_MID_BOSS_TABLE_H_

#include "Defines/Assets.h"
#include "Utilities/ObjectTable.h"
#include "Characters/Monsters/MidBossCharacter.h"
#include "Characters/Monsters/MidBossTemplate.h"
#include "Characters/Monsters/MidBossDefaultBattleStrategy.h"

// リザードマンエフェクト
static constexpr MonsterEffectContext LIZARD_EFFECT{
	.attack{
		.is_play_effect = true,
		.position_offset_x = 0.6f,
		.position_offset_y = 1.5f,
		.position_offset_z = 1.2f,
		.rotate_offset_x = 105.0f,
		.is_play_se = true,
}, .critical_attack{
		.is_play_effect = true,
		.position_offset_x = 0.6f,
		.position_offset_y = 1.5f,
		.position_offset_z = 1.2f,
		.rotate_offset_x = 25.0f,
		.is_play_se = true,
} };

// リザードマンデータ
static constexpr MonsterData LIZARD_DATA{
	Mesh::LizardWarrior,
	MonsterParameters{
		.attack = 29,
		.defense = 50,
		.speed = 19,
		.luck = 23,
		.max_hp = 250
	},
	static_cast<experience_t>(200),
	LIZARD_EFFECT
};

// リザードマン
using LizardCharacter = MidBossTemplate<MidBossDefaultBattleStrategy, LIZARD_DATA>;

// ドッペルゲンガーエフェクト
static constexpr MonsterEffectContext DOPPELGANGER_EFFECT{
	.attack{
		.is_play_effect = true,
		.position_offset_x = 0.6f,
		.position_offset_y = 1.5f,
		.position_offset_z = 1.2f,
		.rotate_offset_x = 25.0f,
		.is_play_se = true,
}, .attack_2{
		.is_play_effect = true,
		.position_offset_x = 0.7f,
		.position_offset_y = 1.6f,
		.position_offset_z = 1.2f,
		.rotate_offset_x = 25.0f,
		.is_play_se = true,
}, .critical_attack{
		.is_play_effect = true,
		.position_offset_x = 0.0f,
		.position_offset_y = 1.5f,
		.position_offset_z = 1.2f,
		.rotate_offset_x = 45.0f,
		.is_play_se = true,
}, .critical_attack_2{
		.is_play_effect = true,
		.position_offset_x = 0.0f,
		.position_offset_y = 1.5f,
		.position_offset_z = 1.2f,
		.rotate_offset_x = 135.0f,
		.is_play_se = true,
} };

// ドッペルゲンガーデータ
static constexpr MonsterData DOPPELGANGER_DATA{
	Mesh::Doppelganger,
	MonsterParameters{
		.attack = 50,
		.defense = 22,
		.speed = 54,
		.luck = 30,
		.max_hp = 200
	},
	static_cast<experience_t>(200),
	DOPPELGANGER_EFFECT
};

// ドッペルゲンガー
using DoppelgangerCharacter = MidBossTemplate<MidBossDefaultBattleStrategy, DOPPELGANGER_DATA>;

// 強敵テーブル
using MidBossTable = ObjectTable<
	MidBossCharacter* (*)(),
	LizardCharacter,
	DoppelgangerCharacter
>;

#endif // CHARACTERS_MONSTERS_MID_BOSS_TABLE_H_
