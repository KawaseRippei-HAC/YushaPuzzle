#ifndef SYSTEMS_BATTLE_SYSTEM_H_
#define SYSTEMS_BATTLE_SYSTEM_H_

#include "Interfaces/IBattleActor.h"

// バトルシステムクラス
class BattleSystem {
public:
	// 初期化
	static void initialize(IBattleActor& hero, IBattleActor& enemy);

	// 実行
	static void execute(float delta_time, IBattleActor& hero, IBattleActor& enemy);

	// 終了フラグ
	static bool is_finished(void) noexcept;

private:
	/*インスタンス生成禁止*/
	BattleSystem() = delete;

	// 勇者が速いか
	static bool is_hero_speed_fast();

	// スピードの倍率を計算
	static float calc_speed_multiplier();

	// ターン開始時
	static void on_turn_start(IBattleActor& hero, IBattleActor& enemy);

	// ターンの実行
	static void execute_turn(float delta_time, IBattleActor& attacker, IBattleActor& defender);

	// ターン終了
	static void turn_end(IBattleActor& attacker, IBattleActor& defender);

	// 戦闘終了
	static void battle_end(IBattleActor& attacker, IBattleActor& defender);

	// クリティカル判定
	static bool is_critical(const float luck) noexcept;

	// 回避判定
	static bool is_avoid(const float agility)noexcept;

	// 防御判定
	static bool is_defense(const float hp, const float max_hp) noexcept;

	// 防御力の計算
	static int defense_calculation(const DefenderBattleContext& defender_context, DefenderStatus& status);

private:
	// 終了
	inline static bool is_finished_{ false };

	// 同じキャラクターの連続ターン数
	inline static int same_char_turn_streak_{ 0 };

	// 勇者の素早さ
	inline static int hero_speed_{ 0 };
	// 敵の素早さ
	inline static int enemy_speed_{ 0 };

	// 次のターンを開始するか
	inline static bool should_start_next_turn_{ false };

	// 攻撃側
	inline static IBattleActor* attacker_{ nullptr };
	// 防御側
	inline static IBattleActor* defender_{ nullptr };

	// リアクション開始処理の完了フラグ
	inline static bool is_reaction_started_{ false };

	// 戦闘開始フラグ
	inline static bool is_battle_start_{ false };
};

#endif // SYSTEMS_BATTLE_SYSTEM_H_