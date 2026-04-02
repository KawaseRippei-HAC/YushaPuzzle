#ifndef CHARACTERS_HEROES_HERO_DEFINE_H_
#define CHARACTERS_HEROES_HERO_DEFINE_H_

#include "Utilities/StateMachine.h"
#include "Characters/Heroes/HeroContext.h"

/*定数*/
// 勇者デフォルト移動時間
inline constexpr float HERO_DEFAULT_MOVE_TIME{ 30.0f };
// 勇者デフォルト旋回速度
inline constexpr float HERO_DEFAULT_TURN_SPEED{ 5.0f };

// 戦闘時の盾の位置
inline constexpr float SHIELD_BATTLE_POSITION[3]{ 0.0f, 0.0f, 0.04f };
// 戦闘時の盾の回転
inline constexpr float SHIELD_BATTLE_ROTATE[3]{ 90.0f, -50.0f, 0.0f };
// 戦闘時の剣の位置
inline constexpr float SWORD_BATTLE_POSITION[3]{ -0.160f, 0.0f, 0.0f };
// 戦闘時の剣の回転
inline constexpr float SWORD_BATTLE_ROTATE[3]{ 90.0f, 60.0f, 0.0f };

// 非戦闘時の盾の位置
inline constexpr float SHIELD_NORMAL_POSITION[3]{ 0.0f, 0.0f, -0.1f };
// 非戦闘時の盾の回転
inline constexpr float SHIELD_NORMAL_ROTATE[3]{ 350.0f, 0.0f, 0.0f };
// 非戦闘時の剣の位置
inline constexpr float SWORD_NORMAL_POSITION[3]{ 0.1f, -0.40f, -0.12f };
// 非戦闘時の剣の回転
inline constexpr float SWORD_NORMAL_ROTATE[3]{ -11.0f, 0.0f, 200.0f };

// 左腕のボーン番号
inline constexpr int LEFT_ARM_BONE_NUMBER{ 19 };
// 右腕のボーン番号
inline constexpr int RIGHT_ARM_BONE_NUMBER{ 39 };
// 背中のボーン番号
inline constexpr int BACK_BONE_NUMBER{ 15 };

// 勇者状態抽象インターフェース
using IHeroState = IState<HeroContext>;
// 勇者ステートマシンクラス
using HeroStateMachine = StateMachine<HeroContext>;

// 勇者状態種別
namespace HeroState {
	enum HeroState : unsigned char {
		Idle,
		Think,
		Move,
		Event,
		Dead,
	};
}

// 勇者モーション
namespace HeroMotion {
	enum HeroMotion : GSuint {
		Idle = 0,
		Walk = 1,

		ThinkStart = 8,
		Thinking = 9,
		ThinkEnd = 10,

		BattleIdle = 21,
		Skill = 30,
		Attack = 17,
		CriticalAttack = 19,
		Die = 27,
		Damage = 25,
		Encounter = 22,
		Defense = 23,
		DefenseDamage = 24,
		Avoid = 26,
		Victory = 31,
	};
}

#endif // CHARACTERS_HEROES_HERO_DEFINE_H_