#ifndef CHARACTERS_MONSTERS_MID_BOSS_CHARACTER_H_
#define CHARACTERS_MONSTERS_MID_BOSS_CHARACTER_H_

#include "Defines/Assets.h"
#include "Animations/AnimatedMesh.h"
#include "Animations/EffectPlayer.h"
#include "Interfaces/IBattleActor.h"
#include "Characters/CharacterBase.h"
#include "Characters/Monsters/MonsterData.h"
#include "Characters/Monsters/DemonKingLevelUp.h"

// 強敵モーション
namespace MidBossMotion {
	enum MidBossMotion : GSuint {
		Idle = 0,
		Attack = 1,
		CriticalAttack = 2,
		Die = 4,
		Damage = 3,
		Encounter = 6,
		Defense = 8,
		DefenseDamage = 9,
		Avoid = 5,
	};
}

// 強敵キャラクタークラス
class MidBossCharacter :
	public CharacterBase {
public:
	explicit MidBossCharacter(const MonsterData& data);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;
	//// GUI描画
	//void draw_gui(void) const;

	// 攻撃力の取得
	attack_t attack(void) const noexcept override;
	// 防御力の取得
	defense_t defense(void) const noexcept override;

	// 獲得経験値の取得
	int drop_exp(void) const noexcept;

	// 死亡処理
	void die(float delta_time) override;
	// 死亡フラグ
	bool is_dead(void) const noexcept override;

	// バトル開始時の初期化
	// Strategyを動的に設定
	void set_battle_strategy(std::unique_ptr<IBattleActor> strategy);
	// 現在のStrategyオブジェクトを取得します
	IBattleActor& get_battle_actor() const noexcept;

	// モンスターエフェクトコンテキストの取得
	MonsterEffectContext effect_context() const;

private:
	// オーラエフェクト
	EffectPlayer aura_effect_;
	// バトルストラテジー
	std::unique_ptr<IBattleActor> battle_strategy_;

	// 死亡フラグ
	bool is_dead_{ false };
	// タイマー
	float timer_{ 0.0f };

	// 獲得経験値
	int drop_exp_{ 0 };

	// モンスターエフェクトコンテキスト
	MonsterEffectContext effect_context_;

};

#endif // CHARACTERS_MONSTERS_MID_BOSS_CHARACTER_H_
