#ifndef CHARACTERS_MONSTERS_DEMON_KING_DEFAULT_BATTLE_STRATEGY_H_
#define CHARACTERS_MONSTERS_DEMON_KING_DEFAULT_BATTLE_STRATEGY_H_

#include "Interfaces/IBattleActor.h"
#include "Skills/SkillActor.h"

class DemonKingCharacter;

// 魔王のデフォルト戦闘クラス
class DemonKingDefaultBattleStrategy final : public IBattleActor {
public:
	// コンテキスト
	explicit DemonKingDefaultBattleStrategy(DemonKingCharacter* demon_king);
	// デストラクタ
	~DemonKingDefaultBattleStrategy() = default;

	// IBattleActorの実装
	// スキルのコンテキストを設定（スキルを持つ時だけオーバーライド）
	void battle_set_skill_context(CharacterBase* target, int tile_count) override;

	// バトル開始時の初期化
	void battle_start_initialize() override;
	// デバフを追加
	void battle_set_debuff(Modifier debuff) override;
	// デバフ解除判定
	void battle_check_debuffs_removal() override;

	// ターン開始スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
	Modifier battle_turn_start_skill() override;
	// 攻撃スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
	Modifier battle_attack_skill() override;

	// バトルのターン開始時の処理
	void on_attack_turn_start(const bool is_critical) override;
	// バトルの防御ターン開始時の処理
	void on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) override;

	// ターン終了
	void battle_turn_end()override;
	// 戦闘終了
	void battle_end() override;

	// 戦闘アクションT
	void battle_action(float delta_time) override;
	// 戦闘リアクション開始処理
	void on_battle_reaction_start() override;
	// 戦闘リアクション
	void battle_reaction(float delta_time) override;

	// 素早さの取得
	speed_t agility(void) const noexcept override;

	// 戦闘コンテキストを取得
	AttackerBattleContext attacker_battle_context(void) const noexcept override;
	// 防御コンテキストを取得
	DefenderBattleContext defender_battle_context(void) const noexcept override;

	// 敗北フラグ
	bool is_defeated(void) const noexcept override;

	// スキルアクター取得
	SkillActor* skill_actor(void)noexcept override;

	// エフェクトの更新処理
	void battle_effect_update(float delta_time) override;

private:
	// 魔王キャラクター
	DemonKingCharacter* demon_king_;

	// スキルを所持
	SkillActor skill_actor_;
};

#endif // CHARACTERS_MONSTERS_DEMON_KING_DEFAULT_BATTLE_STRATEGY_H_
