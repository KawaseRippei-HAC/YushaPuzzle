#include "Systems/BattleSystem.h"

#include <iostream>

#include "Characters/CharacterBase.h"
#include "Interfaces/IBattleActor.h"
#include "LoggableRandomizer.h"

// クリティカル率
static const float CRITICAL_RATE{ 0.002f };
// 最大クリティカル率
static const float CRITICAL_RATE_MAX{ 0.4f };

// 回避率
static const float AVOID_RATE{ 0.005f };
// 最大回避率
static const float AVOID_RATE_MAX{ 0.5f };

// HPが低い時のHP（20%）
static const float LOW_HP_RATE{ 0.2f };
// HPが低い時の防御確率
static const float LOW_HP_DEFENSE_RATE{ 0.2f };
// HPが半分時のHP（50%）
static const float HALF_HP_RATE{ 0.5f };
// HPが半分の時の防御確率
static const float HALF_HP_DEFENSE_RATE{ 0.1f };
// 通常時の防御確率
static const float NORMAL_HP_DEFENSE_RATE{ 0.05f };

// クリティカルダメージ倍率
static const float CRITICAL_DAMAGE_RATE{ 2.0f };

// 通常時の防御補正率
static const float NORMAL_DEFENSE_CORRECTION_RATE{ 0.2f };
// 防御時の防御補正率
static const float DEFENSE_CORRECTION_RATE{ 0.4f };

// スピードの増加率
static constexpr float SPEED_INCREASE_RATE{ 0.1f };

// 初期化
void BattleSystem::initialize(IBattleActor& hero, IBattleActor& enemy) {
	is_finished_ = false;
	same_char_turn_streak_ = 0;
	hero_speed_ = hero.agility();
	enemy_speed_ = enemy.agility();
	should_start_next_turn_ = true;
	attacker_ = nullptr;
	defender_ = nullptr;
	is_battle_start_ = true;
	gsPlaySE(SE::BattleStart);
}

/*実装*/
// 実行
void BattleSystem::execute(float delta_time, IBattleActor& hero, IBattleActor& enemy) {
	// 戦闘開始の処理
	if (is_battle_start_) {
		// 戦闘開始時のスキルを実行
		hero.battle_start_initialize();
		enemy.battle_start_initialize();

		is_battle_start_ = false;
	}

	// ターン開始時
	if (should_start_next_turn_) on_turn_start(hero, enemy);

	// ターンの実行
	execute_turn(delta_time, *attacker_, *defender_);

	hero.battle_effect_update(delta_time);
	enemy.battle_effect_update(delta_time);
}

// 終了フラグ
bool BattleSystem::is_finished() noexcept {
	return is_finished_;
}

// 勇者の方が速いか
bool BattleSystem::is_hero_speed_fast() {

	const bool is_heroes_turn = hero_speed_ >= enemy_speed_;
	if (is_heroes_turn) enemy_speed_ *= calc_speed_multiplier();
	else hero_speed_ *= calc_speed_multiplier();

	if ((hero_speed_ >= enemy_speed_) == is_heroes_turn) ++same_char_turn_streak_;
	else same_char_turn_streak_ = 0;

	return is_heroes_turn;
}

// スピードの倍率を計算
float BattleSystem::calc_speed_multiplier() {
	return (1 + SPEED_INCREASE_RATE * (same_char_turn_streak_ + 1));
}

// ターン開始時
void BattleSystem::on_turn_start(IBattleActor& hero, IBattleActor& enemy) {
	LoggableRandomizer::log("<turn start>");

	// スキル実行とスキルによるバフの追加
	enemy.battle_set_debuff(hero.battle_turn_start_skill());
	hero.battle_set_debuff(enemy.battle_turn_start_skill());

	// デバフ解除判定
	hero.battle_check_debuffs_removal();
	enemy.battle_check_debuffs_removal();

	// 勇者のターンか決める
	// 勇者が速ければ、勇者のターンとする
	const bool is_heroes_turn = is_hero_speed_fast();
	// 攻撃側と防御側を決める
	attacker_ = (is_heroes_turn) ? &hero : &enemy;
	defender_ = (is_heroes_turn) ? &enemy : &hero;

	// 攻撃側の攻撃スキル実行し、防御側にデバフを追加
	defender_->battle_set_debuff(attacker_->battle_attack_skill());

	// 攻撃側と防御側の戦闘コンテキストを取得
	const AttackerBattleContext attacker_context = attacker_->attacker_battle_context();
	const DefenderBattleContext defender_context_ = defender_->defender_battle_context();

	// 防御側のステータス
	DefenderStatus status{ DefenderStatus::None };
	// ダメージ
	attack_t damage_{ 0 };
	// クリティカルフラグ
	bool is_critical_{ false };

	// 防御側の回避判定
	if (is_avoid(defender_->agility())) {
		status = DefenderStatus::Avoid;
		damage_ = 0;
		is_critical_ = false;
	}
	else {
		// 攻撃側の攻撃力を代入
		damage_ = attacker_context.power;

		// クリティカル判定
		is_critical_ = is_critical(attacker_context.luck);
		// クリティカルならダメージを倍にする
		if(is_critical_) damage_ *= CRITICAL_DAMAGE_RATE;

		// 攻撃力から防御力を引いて、実際のダメージを計算
		damage_ -= defense_calculation(defender_context_, status);
	}

	// 攻撃側と防御側のターン開始処理を呼ぶ
	attacker_->on_attack_turn_start(is_critical_);
	// 防御側のターン開始処理
	defender_->on_defence_turn_start(status, damage_, is_critical_);

	// リアクション開始処理の完了フラグ
	is_reaction_started_ = false;

	// 次のターンを開始するか
	should_start_next_turn_ = false;
	LoggableRandomizer::log("<turn start completed>");
}

// ターンの実行
void BattleSystem::execute_turn(float delta_time, IBattleActor& attacker, IBattleActor& defender) {

	// 攻撃側のアクションが完了していなかったら実行
	if(!attacker.is_turn_completed())attacker.battle_action(delta_time);

	// リアクション開始処理の実行
	// リアクション開始処理が終わっていないかつ、リアクション開始フラグが立っているか
	if (!is_reaction_started_ && attacker.is_reaction_start()) {
		// リアクション開始処理を実行
		defender.on_battle_reaction_start();

		// リアクション開始処理の完了フラグ
		is_reaction_started_ = true;
	}
	// リアクション開始処理が終了しているかつ、
	// 防御側のリアクションが完了していなかったら、防御側のリアクションを実行
	if (is_reaction_started_ && !defender.is_turn_completed()) defender.battle_reaction(delta_time);

	// 防御側の完了フラグ
	const bool is_defender_completed = (is_reaction_started_) ? defender.is_turn_completed() : true;
	// 攻撃・防御側の処理が終わっていれば、次のターンを開始する
	if (attacker.is_turn_completed() && is_defender_completed) {
		// ターン終了処理
		turn_end(attacker, defender);

		// 防御側の敗北判定 ture:戦闘終了処理実行 false:次のターンへ
		if (defender.is_defeated()) battle_end(attacker, defender);
		else should_start_next_turn_ = true;
	}
}

// ターン終了
void BattleSystem::turn_end(IBattleActor& attacker, IBattleActor& defender) {
	attacker.battle_turn_end();
	defender.battle_turn_end();
}

// 戦闘終了
void BattleSystem::battle_end(IBattleActor& attacker, IBattleActor& defender) {
	// 攻撃側(勝利した側)だけ回復を実行
	const bool is_heal = attacker.is_heal_completed();
	if (!is_heal) {
		attacker.battle_heal();
		return;
	}

	const bool atk_is_end_comp = attacker.is_battle_end_completed();
	const bool def_is_end_comp = defender.is_battle_end_completed();
	// キャラクターのバトル終了関数呼び出し
	if (!atk_is_end_comp) attacker.battle_end();
	if (!def_is_end_comp) defender.battle_end();

	// 戦闘終了処理が全て完了したら戦闘終了
	if (is_heal && atk_is_end_comp && def_is_end_comp) {
		is_finished_ = true;
	}
}

// クリティカル判定
bool BattleSystem::is_critical(float luck) noexcept {
	// 運1に付きクリティカル率0.2%（最大40%）
	float critical_rate = MIN(luck * CRITICAL_RATE, CRITICAL_RATE_MAX);
	return LoggableRandomizer::generate(0.0f, 1.0f) <= critical_rate;
}

// 回避判定
bool BattleSystem::is_avoid(float agility) noexcept {
	// 素早さ1につき回避率0.5%（最大50%）
	float avoid_rate = MIN(agility * AVOID_RATE, AVOID_RATE_MAX);
	return LoggableRandomizer::generate(0.0f, 1.0f) <= avoid_rate;
}

// 防御判定
bool BattleSystem::is_defense(float hp, float max_hp) noexcept {
	// 最大HPの何%か
	const float hp_rate = hp / max_hp;
	float defense_rate;
	// hpに応じて防御する確率を設定
	// HPが低い時（20%）: 確率20%
	if (hp_rate <= LOW_HP_RATE) defense_rate = LOW_HP_DEFENSE_RATE;
	// HPが半分時（50%）: 確率10%
	else if (hp_rate <= HALF_HP_RATE) defense_rate = HALF_HP_DEFENSE_RATE;
	// 通常時 : 確率5%
	else defense_rate = NORMAL_HP_DEFENSE_RATE;

	return LoggableRandomizer::generate(0.0f, 1.0f) <= defense_rate;
}

// 防御力の計算
int BattleSystem::defense_calculation(const DefenderBattleContext& defender_context, DefenderStatus& status) {

	// 防御フラグ
	const bool is_defense_flg = is_defense(defender_context.hp, defender_context.max_hp);

	// 防御ステータスを決める
	status = (is_defense_flg) ? DefenderStatus::Defense : DefenderStatus::Attack;

	// 防御の補正倍率
	const float defense_correction_rate = (is_defense_flg) ? DEFENSE_CORRECTION_RATE : NORMAL_DEFENSE_CORRECTION_RATE;

	// 防御力の計算( 防御 * 防御の補正倍率 )
	return defender_context.defense * defense_correction_rate;
}
