#ifndef INTERFACES_IBATTLE_ACTION_H_
#define INTERFACES_IBATTLE_ACTION_H_

#include "Characters/CharacterParameter.h"
#include "Modifiers/StatusModifiers.h"
#include "Skills/SkillActor.h"
#include "Animations/EffectPlayer.h"

class CharacterBase;

// 防御ステータス
enum class DefenderStatus {
	None,
	Attack,	// 攻撃
	Avoid,	// 回避
	Defense	// 防御
};

// 攻撃側の戦闘コンテキスト
struct AttackerBattleContext {
	attack_t power{ 0 };	// 攻撃力
	luck_t luck{ 0 };		// 運
};

// 防御側の戦闘コンテキスト
struct DefenderBattleContext {
	defense_t defense{ 0 };	// 防御力
	point_t hp{ 0 };		// HP
	point_t max_hp{ 0 };	// 最大HP
};

// 戦闘アクター抽象インターフェース
class IBattleActor {
public:
	virtual ~IBattleActor() = default;

	// スキルのコンテキストを設定（スキルを持つ時だけオーバーライド）
	virtual void battle_set_skill_context(CharacterBase* target, int tile_count) {};

	// バトル開始時の初期化
	virtual void battle_start_initialize() = 0;

	// デバフを追加
	virtual void battle_set_debuff(Modifier debuff) = 0;
	// デバフ解除判定
	virtual void battle_check_debuffs_removal() = 0;

	// ターン開始スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
	virtual Modifier battle_turn_start_skill() { return Modifier{}; };
	// 攻撃スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
	virtual Modifier battle_attack_skill() { return Modifier{}; };

	// バトルの攻撃ターン開始時の処理(素早さ対抗後に呼ばれる)
	virtual void on_attack_turn_start(const bool is_critical) = 0;
	// バトルの防御ターン開始時の処理(素早さ対抗後に呼ばれる)
	virtual void on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) = 0;

	// ターン終了
	virtual void battle_turn_end() = 0;
	// 戦闘終了
	virtual void battle_end() = 0;
	// 戦闘終了処理完了フラグ
	virtual bool is_battle_end_completed() const noexcept { return is_battle_end_completed_; };

	// 戦闘アクション
	virtual void battle_action(float delta_time) = 0;
	// 戦闘リアクション開始処理
	virtual void on_battle_reaction_start() = 0;
	// 戦闘リアクション
	virtual void battle_reaction(float delta_time) = 0;

	// 素早さの取得
	virtual speed_t agility(void) const noexcept = 0;
	// 回復処理（ターン終了時に実行するので勇者だけ実行）
	virtual void battle_heal() {};
	// 回復処理完了フラグ
	virtual bool is_heal_completed() const noexcept { return true; };

	// 攻撃側の戦闘コンテキストを取得
	virtual AttackerBattleContext attacker_battle_context(void) const noexcept = 0;
	// 防御側の戦闘コンテキストを取得
	virtual DefenderBattleContext defender_battle_context(void) const noexcept = 0;

	// リアクション開始フラグ
	virtual bool is_reaction_start(void) const noexcept { return is_reaction_start_; }
	// ターン完了フラグ
	virtual bool is_turn_completed(void) const noexcept { return is_turn_completed_; }
	// 敗北フラグ
	virtual bool is_defeated(void) const noexcept = 0;
	// スキルアクター取得（スキルを持つ時だけオーバーライド）
	virtual SkillActor* skill_actor(void) noexcept { return nullptr; };
	virtual void battle_effect_update(float delta_time) = 0;

protected:
	// ステータス
	DefenderStatus status_{ 0 };
	// ダメージ
	attack_t battle_damage_{ 0 };
	// クリティカルフラグ
	bool is_critical_{ false };

	// リアクション開始フラグ
	bool is_reaction_start_{ false };

	// 戦闘のターン完了フラグ
	bool is_turn_completed_{ false };

	// 回復処理完了フラグ
	bool is_battle_end_completed_{ false };

	/* --- エフェクト --- */
	EffectPlayer attack_effect_;			// 攻撃エフェクト
	EffectPlayer attack_2_effect_;			// 攻撃2エフェクト
	EffectPlayer hit_effect_;				// ダメージエフェクト
	EffectPlayer dead_effect_;				// 死亡エフェクト
	EffectPlayer debuff_effect_;			// デバフエフェクト

};

#endif // INTERFACES_IBATTLE_ACTION_H_