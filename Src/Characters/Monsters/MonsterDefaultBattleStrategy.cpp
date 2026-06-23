#include "Characters/Monsters/MonsterDefaultBattleStrategy.h"

#include "Characters/Monsters/MonsterCharacter.h"

// 強敵のダメージオフセット
static const GSvector3 MONSTER_HIT_OFFSET{ GSvector3::up() * 1.1f };
// 強敵の正面オフセット
static constexpr float MONSTER_FORWARD_OFFSET{ 0.3f };

/*実装*/
// コンストラクタ
MonsterDefaultBattleStrategy::MonsterDefaultBattleStrategy(CharacterBase* monster) :
	monster_{ monster } {

	// ステータスの初期化
	status_ = DefenderStatus::None;
	// 戦闘ダメージの初期化
	battle_damage_ = 0;
	// クリティカルフラグの初期化
	is_critical_ = false;

	// バトルの行動完了フラグ
	is_turn_completed_ = false;

	// リアクション開始フラグ
	is_reaction_start_ = false;

	// アニメーションイベントの追加
	monster_->mesh().add_event(MonsterMotion::Attack, 30, [&]() { is_reaction_start_ = true; });
	monster_->mesh().add_event(MonsterMotion::CriticalAttack, 30, [&]() { is_reaction_start_ = true; });

	// アイドル状態へ
	monster_->mesh().change_motion(MonsterMotion::Idle);
}

// デバフを追加
void MonsterDefaultBattleStrategy::battle_set_debuff(Modifier debuff) {
	monster_->modifiers().set_debuff(debuff);
}
// デバフ解除判定
void MonsterDefaultBattleStrategy::battle_check_debuffs_removal() {
	// デバフの解除判定
	monster_->modifiers().check_debuffs_removal(monster_->luck());
}

// バトル開始時の初期化
void MonsterDefaultBattleStrategy::battle_start_initialize() {
	is_battle_end_completed_ = false;
}

// バトルの攻撃ターン開始時の処理
void MonsterDefaultBattleStrategy::on_attack_turn_start(const bool is_critical) {

	// リアクション開始フラグ
	is_reaction_start_ = false;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// クリティカルならクリティカル攻撃モーション
	if (is_critical) monster_->mesh().forced_change_motion(MonsterMotion::CriticalAttack, false);
	// クリティカルじゃなければ、攻撃モーションにする
	else monster_->mesh().forced_change_motion(MonsterMotion::Attack, false);
}

// バトルの防御ターン開始時の処理
void MonsterDefaultBattleStrategy::on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) {
	// ステータスを取得
	status_ = status;
	// ダメージを取得
	battle_damage_ = damage;
	// クリティカルフラグを取得
	is_critical_ = is_critical;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// アイドルモーションにする
	if (status_ != DefenderStatus::Defense) monster_->mesh().change_motion(MonsterMotion::Idle);
	// 防御モーションにする
	else monster_->mesh().change_motion(MonsterMotion::Defense);
}

// ターン終了
void MonsterDefaultBattleStrategy::battle_turn_end() {

}

// 戦闘終了
void MonsterDefaultBattleStrategy::battle_end() {
	// バフ・デバフを初期化
	Modifier zero;
	monster_->modifiers().set_buff(zero);
	monster_->modifiers().set_debuff(zero);
	dead_effect_.play(Effect::MonsterDead).position(monster_->transform().position());
	is_battle_end_completed_ = true;
}

// 戦闘アクション
void MonsterDefaultBattleStrategy::battle_action(float delta_time) {
	// モーションが終了したら、ターン終了
	if (monster_->mesh().is_motion_finished()) {
		// バトルターン完了フラグ
		is_turn_completed_ = true;
		// アイドル状態へ
		monster_->mesh().change_motion(MonsterMotion::Idle);
	}
}

// 戦闘リアクション開始処理
void MonsterDefaultBattleStrategy::on_battle_reaction_start() {
	// ダメージ処理
	monster_->take_damage(battle_damage_);

	const GSvector3 pos = monster_->transform().position() + MONSTER_HIT_OFFSET +
		(monster_->transform().forward() * MONSTER_FORWARD_OFFSET);

	switch (status_)
	{
	case DefenderStatus::Attack:
		monster_->mesh().forced_change_motion(MonsterMotion::Damage, false);
		hit_effect_.play(Effect::HitBlow).position(pos);
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	case DefenderStatus::Avoid: monster_->mesh().forced_change_motion(MonsterMotion::Avoid, false); break;
	case DefenderStatus::Defense:
		monster_->mesh().forced_change_motion(MonsterMotion::DefenseDamage, false);
		hit_effect_.play(Effect::HitGuard).position(pos).rotation(GSvector3{ 0.0f, 0.0f, monster_->transform().eulerAngles().y });
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	}

	// 死亡していたら死亡モーション
	if (monster_->hp() <= 0) {
		monster_->mesh().forced_change_motion(MonsterMotion::Die, false);
	}

#ifdef _DEBUG
	switch (status_)
	{
	case DefenderStatus::Attack: std::cout << "[ ダメージ ]" << std::endl; break;
	case DefenderStatus::Avoid: std::cout << "[ 回避 ]" << std::endl; break;
	case DefenderStatus::Defense: std::cout << "[ 防御 ]" << std::endl; break;
	}
	if (is_critical_) std::cout << "クリティカルダメージ" << std::endl;
	std::cout << "敵は " << (int)battle_damage_ << "ダメージ 受けた hp:" << monster_->hp() << std::endl;
#endif // _DEBUG
}

// 戦闘リアクション
void MonsterDefaultBattleStrategy::battle_reaction(float delta_time) {
	// モーションが終了したら、ターン終了
	if (monster_->mesh().is_motion_finished()) {

		// HPが0かつ死亡フラグが立ってない時
		if (monster_->hp() <= 0 && !monster_->is_dead()) {
			// 死亡処理を実行
			monster_->die(delta_time);
			return;
		}

		// バトルターン完了フラグ
		is_turn_completed_ = true;
		// アイドル状態へ
		monster_->mesh().change_motion(MonsterMotion::Idle);
	}
}

// 素早さの取得
speed_t MonsterDefaultBattleStrategy::agility(void) const noexcept {
	return (speed_t)(monster_->speed() + monster_->speed_modifier());
}

// 戦闘コンテキストを取得
AttackerBattleContext MonsterDefaultBattleStrategy::attacker_battle_context(void) const noexcept {
	return AttackerBattleContext{ (attack_t)(monster_->attack() + monster_->attack_modifier()), (luck_t)(monster_->luck() + monster_->luck_modifier()) };
}

// 防御コンテキストを取得
DefenderBattleContext MonsterDefaultBattleStrategy::defender_battle_context(void) const noexcept {
	return DefenderBattleContext{ (defense_t)(monster_->defense() + monster_->defense_modifier()), monster_->hp(), monster_->max_hp() };
}

// 敗北フラグ
bool MonsterDefaultBattleStrategy::is_defeated(void) const noexcept {
	return monster_->is_dead();
}

// エフェクトの更新処理
void MonsterDefaultBattleStrategy::battle_effect_update(float delta_time) {
	// 攻撃エフェクト
	attack_effect_.update(delta_time);
	// ダメージエフェクト
	hit_effect_.update(delta_time);
	// 死亡エフェクト
	dead_effect_.update(delta_time);
	// デバフエフェクト
	debuff_effect_.update(delta_time);
}
