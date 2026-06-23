#ifndef CHARACTERS_MONSTERS_DEMON_KING_CHARACTER_H_
#define CHARACTERS_MONSTERS_DEMON_KING_CHARACTER_H_

#include "Defines/Assets.h"
#include "Animations/AnimatedMesh.h"
#include "Animations/EffectPlayer.h"
#include "Interfaces/IBattleActor.h"
#include "Characters/CharacterBase.h"
#include "Characters/Monsters/MonsterData.h"
#include "Characters/Monsters/DemonKingLevelUp.h"

// 魔王モーション
namespace DemonKingMotion {
	enum DemonKingMotion : GSuint {
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

// 魔王キャラクタークラス
class DemonKingCharacter :
	public CharacterBase {
public:
	explicit DemonKingCharacter(const MonsterData& data);

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
	// 死亡処理
	void die(float delta_time) override;
	// 死亡フラグ
	bool is_dead(void) const noexcept override;

	// レベルアップ実行
	void level_up_execute(int tile_count);

	// 現在の経験値量を取得
	int experience(void) const noexcept;
	// 必要経験値量を取得
	int required_experience(void) const noexcept;
	// 経験値の追加
	void add_experience(int amount) noexcept;
	// 必要経験値量を追加
	void add_required_experience(int amount) noexcept;
	// 経験値のリセット
	void reset_experience(void) noexcept;

	// バトル開始時の初期化
	// Strategyを動的に設定
	void set_battle_strategy(std::unique_ptr<IBattleActor> strategy);
	// 現在のStrategyオブジェクトを取得します
	IBattleActor& get_battle_actor() const noexcept;

private:
	// バトルストラテジー
	std::unique_ptr<IBattleActor> battle_strategy_;

	// オーラエフェクト
	EffectPlayer aura_effect_;

	int exp_{ 0 };				// 現在の経験値 (魔王の経験値はプレイヤーの置いたマス数）
	int required_exp_{ 10 };	// 必要経験値（レベルアップに必要なマス数）
	// レベルアップ
	DemonKingLevelUp demon_king_level_up_{ this };

	// 死亡フラグ
	bool is_dead_{ false };
	// タイマー
	float timer_{ 0.0f };
};

#endif // CHARACTERS_MONSTERS_DEMON_KING_CHARACTER_H_