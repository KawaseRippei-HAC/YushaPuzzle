#ifndef MONSTER_DATA_H_
#define MONSTER_DATA_H_

#include "Interfaces/IBattleActor.h"

// 敵、強敵、魔王の初期パラメータ
struct MonsterParameters {
	attack_t	attack{ 0 };
	defense_t	defense{ 0 };
	speed_t		speed{ 0 };
	luck_t		luck{ 0 };
	point_t		max_hp{ 0 };
};

// エフェクトのパラメータ
struct EffectParameters {
	// 攻撃エフェクトを実行フラグ
	bool is_play_effect{ false };
	// エフェクトのID
	GSuint effect_id{ Effect::MonsterSlashAttack };
	// 位置のオフセット
	float position_offset_x{ 0.0f };
	float position_offset_y{ 0.0f };
	float position_offset_z{ 0.0f };
	// 回転
	float rotate_offset_x{ 0.0f };
	float rotate_offset_y{ 0.0f };
	float rotate_offset_z{ 0.0f };
	// サイズ
	float size{ 1.0f };

	// SE実行フラグ
	bool is_play_se{ false };
	// SEのID
	GSuint se_id{ SE::Slash };
};

// モンスターのエフェクトコンテキスト
struct MonsterEffectContext {
	EffectParameters attack;		// 攻撃エフェクト
	EffectParameters attack_2;		// 攻撃エフェクト
	EffectParameters critical_attack;	// クリティカル攻撃エフェクト
	EffectParameters critical_attack_2;	// クリティカル攻撃エフェクト
};

// モンスター単位のデータを保持する構造体
struct MonsterData {
	GSuint mesh_id{ 0 };
	MonsterParameters params;
	int drop_exp{ 0 };	// 獲得経験値
	MonsterEffectContext effect;
};

// スライムデータ
inline constexpr MonsterData SLIME_DATA{
	.mesh_id = Mesh::Slime,
	.params = {
		.attack = 8,
		.defense = 6,
		.speed = 7,
		.luck = 0,
		.max_hp = 40 },
	.drop_exp = 5
};
// スライム出現レベル
inline constexpr level_t SLIME_SPAWN_REQUIRE_LEVEL{ 0 };

// ファントムデータ
inline constexpr MonsterData PHANTOM_DATA{
	.mesh_id = Mesh::Phantom,
	.params = {
		.attack = 34,
		.defense = 14,
		.speed = 22,
		.luck = 30,
		.max_hp = 110 },
		.drop_exp = 30
};
// ファントム出現レベル
inline constexpr level_t PHANTOM_SPAWN_REQUIRE_LEVEL{ 6 };

// ゴーレムデータ
inline constexpr MonsterData GOLEM_DATA{
	.mesh_id = Mesh::Golem,
	.params = {
		.attack = 34,
		.defense = 42,
		.speed = 8,
		.luck = 9,
		.max_hp = 180 },
		.drop_exp = 50
};
// ゴーレム出現レベル
inline constexpr level_t GOLEM_SPAWN_REQUIRE_LEVEL{ 9 };

// 焼き鳥データ
inline constexpr MonsterData GRILLED_CHICKEN_DATA{
	.mesh_id = Mesh::GrilledChicken,
	.params = {
		.attack = 13,
		.defense = 8,
		.speed = 30,
		.luck = 14,
		.max_hp = 50 },
		.drop_exp = 15
};
// 焼き鳥出現レベル
inline constexpr level_t GRILLED_CHICKEN_SPAWN_REQUIRE_LEVEL{ 4 };

// オークデータ
inline constexpr MonsterData ORC_DATA{
	.mesh_id = Mesh::Orc,
	.params = {
		.attack = 18,
		.defense = 26,
		.speed = 12,
		.luck = 11,
		.max_hp = 140 },
		.drop_exp = 20
};
// オーク出現レベル
inline constexpr level_t ORC_SPAWN_REQUIRE_LEVEL{ 5 };

// スケルトンデータ
inline constexpr MonsterData SKELETON_DATA{
	.mesh_id = Mesh::Skeleton,
	.params = {
		.attack = 11,
		.defense = 7,
		.speed = 15,
		.luck = 17,
		.max_hp = 60 },
		.drop_exp = 10
};
// スケルトン出現レベル
inline constexpr level_t SKELETON_SPAWN_REQUIRE_LEVEL{ 2 };

#endif // MONSTER_DATA_H_
