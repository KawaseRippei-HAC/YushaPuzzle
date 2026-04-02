#include "Characters/Monsters/DemonKingDefaultBattleStrategy.h"

#include "Characters/Monsters/DemonKingCharacter.h"

// 強敵の攻撃オフセット
static const GSvector3 DEMON_KING_ATTACK_OFFSET{ GSvector3::up() * 1.5f };
// 強敵のダメージオフセット
static const GSvector3 DEMON_KING_HIT_OFFSET{ GSvector3::up() * 1.1f };
// 強敵の正面オフセット
static constexpr float DEMON_KING_FORWARD_OFFSET{ 1.2f };
// 強敵の右手オフセット（通常攻撃用）
static constexpr float DEMON_KING_RIGHT_HAND_OFFSET{ 0.6f };

// 通常攻撃の回転
static constexpr float ATTACK_ROTATION{ 105.0f };
// クリティカル攻撃の回転
static constexpr float CRITICAL_ATTACK_ROTATION{ 135 };

/*実装*/
// コンストラクタ
DemonKingDefaultBattleStrategy::DemonKingDefaultBattleStrategy(DemonKingCharacter* demon_king) :
	demon_king_{ demon_king } {

	// ステータスの初期化
	status_ = DefenderStatus::None;
	// 戦闘ダメージの初期化
	battle_damage_ = 0;
	// クリティカルフラグの初期化
	is_critical_ = false;

	// バトルの行動完了フラグ
	is_turn_completed_ = false;

	// リアクション開始フラグ
	is_reaction_start_ = true;

	// アニメーションイベントの追加
	demon_king_->mesh().add_event(DemonKingMotion::Attack, 45, [&]() { is_reaction_start_ = true; });
	demon_king_->mesh().add_event(DemonKingMotion::Attack, 35,
		[&]() {
			const GSvector3 pos = demon_king_->transform().position() + DEMON_KING_ATTACK_OFFSET +
				(demon_king_->transform().forward() * DEMON_KING_FORWARD_OFFSET) +
				(demon_king_->transform().right() * DEMON_KING_RIGHT_HAND_OFFSET);
			attack_effect_.
				play(Effect::MonsterSlashAttack).
				position(pos).
				rotation(GSvector3{ ATTACK_ROTATION, 0.0f, demon_king_->transform().eulerAngles().y });
			gsPlaySE(SE::Slash);
		});
	demon_king_->mesh().add_event(DemonKingMotion::CriticalAttack, 30, [&]() { is_reaction_start_ = true; });
	demon_king_->mesh().add_event(DemonKingMotion::CriticalAttack, 30,
		[&]() {
			const GSvector3 pos = demon_king_->transform().position() + DEMON_KING_ATTACK_OFFSET +
				(demon_king_->transform().forward() * DEMON_KING_FORWARD_OFFSET);
			attack_effect_.
				play(Effect::MonsterSlashAttack).
				position(pos).
				rotation(GSvector3{ CRITICAL_ATTACK_ROTATION, 0.0f, demon_king_->transform().eulerAngles().y });
			gsPlaySE(SE::Slash);
		});
}

// スキルのコンテキストを設定（スキルを持つ時だけオーバーライド）
void DemonKingDefaultBattleStrategy::battle_set_skill_context(CharacterBase* target, int tile_count) {
	skill_actor_.set_skill_context(demon_king_, target, tile_count);
}

// バトル開始時の初期化
void DemonKingDefaultBattleStrategy::battle_start_initialize() {
	// バトル開始時に常時発動スキルを計算しておく
	skill_actor_.battle_start_skill();

	is_battle_end_completed_ = false;
}
// デバフを追加
void DemonKingDefaultBattleStrategy::battle_set_debuff(Modifier debuff) {
	demon_king_->modifiers().set_debuff(debuff);
}
// デバフ解除判定
void DemonKingDefaultBattleStrategy::battle_check_debuffs_removal() {
	// デバフの解除判定
	demon_king_->modifiers().check_debuffs_removal(demon_king_->luck());
}

// ターン開始スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
Modifier DemonKingDefaultBattleStrategy::battle_turn_start_skill() {
	// ターン開始スキル実行
	SkillModifierContext context = skill_actor_.turn_start_skill();
	// スキルによるバフを設定
	demon_king_->modifiers().set_buff(context.buffs);
	// 相手に付与するデバフを返す
	return context.target_debuffs;
}
// 攻撃スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
Modifier DemonKingDefaultBattleStrategy::battle_attack_skill() {
	// 攻撃スキル実行
	SkillModifierContext context = skill_actor_.attack_skill();
	// スキルによるバフを追加 *攻撃時は追加
	demon_king_->modifiers().add_buff(context.buffs);
	// 相手に付与するデバフを返す
	return context.target_debuffs;
}

// バトルの攻撃ターン開始時の処理
void DemonKingDefaultBattleStrategy::on_attack_turn_start(const bool is_critical) {
	// リアクション開始フラグ
	is_reaction_start_ = false;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// クリティカルならクリティカル攻撃モーション
	if (is_critical) demon_king_->mesh().forced_change_motion(DemonKingMotion::CriticalAttack, false);
	// クリティカルじゃなければ、攻撃モーションにする
	else demon_king_->mesh().forced_change_motion(DemonKingMotion::Attack, false);
}

// バトルの防御ターン開始時の処理
void DemonKingDefaultBattleStrategy::on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) {
	// ステータスを取得
	status_ = status;
	// ダメージを取得
	battle_damage_ = damage;
	// クリティカルフラグを取得
	is_critical_ = is_critical;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// アイドルモーションにする
	if (status_ != DefenderStatus::Defense) demon_king_->mesh().change_motion(DemonKingMotion::Idle);
	// 防御モーションにする
	else demon_king_->mesh().change_motion(DemonKingMotion::Defense);
}

// ターン終了
void DemonKingDefaultBattleStrategy::battle_turn_end() {

}

// 戦闘終了
void DemonKingDefaultBattleStrategy::battle_end() {
	// バフ・デバフを初期化
	Modifier zero;
	demon_king_->modifiers().set_buff(zero);
	demon_king_->modifiers().set_debuff(zero);
	dead_effect_.play(Effect::MonsterDead).position(demon_king_->transform().position());
	is_battle_end_completed_ = true;
}

// 戦闘アクション
void DemonKingDefaultBattleStrategy::battle_action(float delta_time) {
	// モーションが終了したら、ターン終了
	if (demon_king_->mesh().is_motion_finished()) {
		// バトルターン完了フラグ
		is_turn_completed_ = true;
		// アイドル状態へ
		demon_king_->mesh().change_motion(DemonKingMotion::Idle);
	}
}

// 戦闘リアクション開始処理
void DemonKingDefaultBattleStrategy::on_battle_reaction_start() {
	// ダメージ処理
	demon_king_->take_damage(battle_damage_);

	const GSvector3 pos = demon_king_->transform().position() + DEMON_KING_HIT_OFFSET +
		(demon_king_->transform().forward() * DEMON_KING_FORWARD_OFFSET);
	switch (status_)
	{
	case DefenderStatus::Attack: demon_king_->mesh().forced_change_motion(DemonKingMotion::Damage, false);
		hit_effect_.play(Effect::HitBlow).position(pos);
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	case DefenderStatus::Avoid: demon_king_->mesh().forced_change_motion(DemonKingMotion::Avoid, false); break;
	case DefenderStatus::Defense: demon_king_->mesh().forced_change_motion(DemonKingMotion::DefenseDamage, false);
		hit_effect_.play(Effect::HitGuard).position(pos).rotation(GSvector3{ 0.0f, 0.0f, demon_king_->transform().eulerAngles().y });
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	}

	// 死亡していたら死亡モーション
	if (demon_king_->hp() <= 0) {
		demon_king_->mesh().forced_change_motion(DemonKingMotion::Die, false);
		return;
	}

#ifdef _DEBUG
	switch (status_)
	{
	case DefenderStatus::Attack: std::cout << "[ ダメージ ]" << std::endl; break;
	case DefenderStatus::Avoid: std::cout << "[ 回避 ]" << std::endl; break;
	case DefenderStatus::Defense: std::cout << "[ 防御 ]" << std::endl; break;
	}
	if (is_critical_) std::cout << "クリティカルダメージ" << std::endl;
	std::cout << "敵は " << (int)battle_damage_ << "ダメージ 受けた hp:" << demon_king_->hp() << std::endl;
#endif // _DEBUG
}

// 戦闘リアクション
void DemonKingDefaultBattleStrategy::battle_reaction(float delta_time) {
	// モーションが終了したら、ターン終了
	if (demon_king_->mesh().is_motion_finished()) {

		// HPが0かつ死亡フラグが立ってない時
		if (demon_king_->hp() <= 0 && !demon_king_->is_dead()) {
			// 死亡処理を実行
			demon_king_->die(delta_time);
			return;
		}

		// バトルターン完了フラグ
		is_turn_completed_ = true;
		// アイドル状態へ
		demon_king_->mesh().change_motion(DemonKingMotion::Idle);
	}
}

// 素早さの取得
speed_t DemonKingDefaultBattleStrategy::agility(void) const noexcept {
	return (speed_t)(demon_king_->speed() + demon_king_->speed_modifier());
}

// 戦闘コンテキストを取得
AttackerBattleContext DemonKingDefaultBattleStrategy::attacker_battle_context(void) const noexcept {
	return AttackerBattleContext{ (attack_t)(demon_king_->attack() + demon_king_->attack_modifier()), (luck_t)(demon_king_->luck() + demon_king_->luck_modifier()) };
}

// 防御コンテキストを取得
DefenderBattleContext DemonKingDefaultBattleStrategy::defender_battle_context(void) const noexcept {
	return DefenderBattleContext{ (defense_t)(demon_king_->defense() + demon_king_->defense_modifier()), demon_king_->hp(), demon_king_->max_hp() };
}

// 敗北フラグ
bool DemonKingDefaultBattleStrategy::is_defeated(void) const noexcept {
	return demon_king_->is_dead();
}

// スキルアクター取得
SkillActor* DemonKingDefaultBattleStrategy::skill_actor(void) noexcept {
	return &skill_actor_;
}

// エフェクトの更新処理
void DemonKingDefaultBattleStrategy::battle_effect_update(float delta_time) {
	// 攻撃エフェクト
	attack_effect_.update(delta_time);
	// ダメージエフェクト
	hit_effect_.update(delta_time);
	// 死亡エフェクト
	dead_effect_.update(delta_time);
	// デバフエフェクト
	debuff_effect_.update(delta_time);
}
