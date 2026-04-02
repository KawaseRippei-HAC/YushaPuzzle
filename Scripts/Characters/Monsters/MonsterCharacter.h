#ifndef CHARACTERS_MONSTERS_MONSTER_CHARACTER_H_
#define CHARACTERS_MONSTERS_MONSTER_CHARACTER_H_

#include "Defines/Assets.h"
#include "Tiles/TileManager.h"
#include "Animations/AnimatedMesh.h"
#include "Interfaces/IBattleActor.h"
#include "Characters/CharacterBase.h"
#include "Characters/Monsters/MonsterData.h"

// 敵モーション
namespace MonsterMotion {
	enum MonsterMotion : GSuint {
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

// 非テンプレートの共通基底クラス
class MonsterBase : public CharacterBase {
public:
	// コンストラクタ
	using CharacterBase::CharacterBase;
	virtual ~MonsterBase() = default;

	// ポリモーフィズムが必要な関数を仮想関数として定義
	virtual IBattleActor& get_battle_actor() const noexcept = 0;
	virtual void set_battle_strategy(std::unique_ptr<IBattleActor> strategy) = 0;

	// 獲得経験値の取得
	virtual int drop_exp(void) const noexcept = 0;

	// EncounterEventから呼び出すために仮想関数化
	virtual void update(float delta_time) = 0;
	virtual void draw(void)const = 0;
};

// モンスターキャラクタークラス
template <class TBattleStrategy, MonsterData data, level_t min_level>
class MonsterCharacter final :
	public MonsterBase {
public:
	// 出現最小レベル
	static constexpr level_t MIN_LEVEL = min_level;

public:
	// コンストラクタ
	MonsterCharacter();
	// デストラクタ
	~MonsterCharacter() = default;

	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

	// 攻撃力の取得
	attack_t attack(void) const noexcept override;
	// 防御力の取得
	defense_t defense(void) const noexcept override;

	// Strategyを動的に設定
	void set_battle_strategy(std::unique_ptr<IBattleActor> strategy);
	// 現在のStrategyオブジェクトを取得します
	IBattleActor& get_battle_actor() const noexcept;

	// 獲得経験値の取得
	int drop_exp(void) const noexcept;

	// 死亡処理
	void die(float delta_time) override;
	// 死亡フラグ
	bool is_dead(void) const noexcept override;

private:
	// IBattleActorを包含
	std::unique_ptr<IBattleActor> battle_strategy_;

	// 死亡フラグ
	bool is_dead_{ false };
	// タイマー
	float timer_{ 0.0f };

	// 獲得経験値
	int drop_exp_{ 0 };
};

#include "Characters/Monsters/MonsterCharacter.hpp"

#endif // CHARACTERS_MONSTERS_MONSTER_CHARACTER_H_