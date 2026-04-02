#include "Characters/Monsters/MidBossDefaultBattleStrategy.h"
#include "Characters/Monsters/MidBossCharacter.h"

// 強敵のダメージオフセット
static const GSvector3 MID_BOSS_HIT_OFFSET{ GSvector3::up() * 1.1f };
// 強敵の正面オフセット
static constexpr float MID_BOSS_FORWARD_OFFSET{  1.2f };

/*実装*/
// コンストラクタ
MidBossDefaultBattleStrategy::MidBossDefaultBattleStrategy(MidBossCharacter* mid_boss) :
	mid_boss_{ mid_boss } {

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

	effect_context_ = mid_boss_->effect_context();

	// アニメーションイベントの追加
	mid_boss_->mesh().add_event(MidBossMotion::Attack, 35, [&]() { is_reaction_start_ = true; });
	mid_boss_->mesh().add_event(MidBossMotion::Attack, 30,
		[&]() {
			
			if (effect_context_.attack.is_play_effect) {
				const GSvector3 pos = mid_boss_->transform().position() +
					(mid_boss_->transform().right() * effect_context_.attack.position_offset_x) +
					mid_boss_->transform().up() * effect_context_.attack.position_offset_y +
					(mid_boss_->transform().forward() * effect_context_.attack.position_offset_z);

				attack_effect_.
					play(effect_context_.attack.effect_id).
					position(pos).
					rotation(GSvector3{ effect_context_.attack.rotate_offset_x, effect_context_.attack.rotate_offset_y, mid_boss_->transform().eulerAngles().y + effect_context_.attack.rotate_offset_z });
			}
			if (effect_context_.attack.is_play_se) {
				gsPlaySE(effect_context_.attack.se_id);
			}
			if (effect_context_.attack_2.is_play_effect) {
				const GSvector3 pos = mid_boss_->transform().position() +
					(mid_boss_->transform().right() * effect_context_.attack_2.position_offset_x) +
					mid_boss_->transform().up() * effect_context_.attack_2.position_offset_y +
					(mid_boss_->transform().forward() * effect_context_.attack_2.position_offset_z);

				attack_effect_.
					play(effect_context_.attack_2.effect_id).
					position(pos).
					rotation(GSvector3{ effect_context_.attack_2.rotate_offset_x, effect_context_.attack_2.rotate_offset_y, mid_boss_->transform().eulerAngles().y + effect_context_.attack_2.rotate_offset_z });
			}
			if (effect_context_.attack.is_play_se) {
				gsPlaySE(effect_context_.attack_2.se_id);
			}
		});
	mid_boss_->mesh().add_event(MidBossMotion::CriticalAttack, 20, [&]() { is_reaction_start_ = true; });
	mid_boss_->mesh().add_event(MidBossMotion::CriticalAttack, 15,
		[&]() {
			/*const GSvector3 pos = mid_boss_->transform().position() + MID_BOSS_ATTACK_OFFSET +
				(mid_boss_->transform().forward() * MID_BOSS_FORWARD_OFFSET);
			attack_effect_.
				play(Effect::MonsterSlashAttack).
				position(pos).
				rotation(GSvector3{ CRITICAL_ATTACK_ROTATION, 0.0f, mid_boss_->transform().eulerAngles().y });
			gsPlaySE(SE::Slash);*/
			if (effect_context_.critical_attack.is_play_effect) {
				const GSvector3 pos = mid_boss_->transform().position() +
					(mid_boss_->transform().right() * effect_context_.critical_attack.position_offset_x) +
					mid_boss_->transform().up() * effect_context_.critical_attack.position_offset_y +
					(mid_boss_->transform().forward() * effect_context_.critical_attack.position_offset_z);

				attack_effect_.
					play(effect_context_.critical_attack.effect_id).
					position(pos).
					rotation(GSvector3{ effect_context_.critical_attack.rotate_offset_x, effect_context_.critical_attack.rotate_offset_y, mid_boss_->transform().eulerAngles().y + effect_context_.critical_attack.rotate_offset_z });
			}
			if (effect_context_.attack.is_play_se) {
				gsPlaySE(effect_context_.critical_attack.se_id);
			}
			if (effect_context_.critical_attack_2.is_play_effect) {
				const GSvector3 pos = mid_boss_->transform().position() +
					(mid_boss_->transform().right() * effect_context_.critical_attack_2.position_offset_x) +
					mid_boss_->transform().up() * effect_context_.critical_attack_2.position_offset_y +
					(mid_boss_->transform().forward() * effect_context_.critical_attack_2.position_offset_z);

				attack_effect_.
					play(effect_context_.critical_attack_2.effect_id).
					position(pos).
					rotation(GSvector3{ effect_context_.critical_attack_2.rotate_offset_x, effect_context_.critical_attack_2.rotate_offset_y, mid_boss_->transform().eulerAngles().y + effect_context_.critical_attack_2.rotate_offset_z });
			}
			if (effect_context_.attack.is_play_se) {
				gsPlaySE(effect_context_.critical_attack_2.se_id);
			}
		});

	// アイドル状態へ
	mid_boss_->mesh().change_motion(MidBossMotion::Idle);
}

// バトル開始時の初期化
void MidBossDefaultBattleStrategy::battle_start_initialize() {
	is_battle_end_completed_ = false;
}
// デバフを追加
void MidBossDefaultBattleStrategy::battle_set_debuff(Modifier debuff) {
	mid_boss_->modifiers().set_debuff(debuff);
}
// デバフ解除判定
void MidBossDefaultBattleStrategy::battle_check_debuffs_removal() {
	// デバフの解除判定
	mid_boss_->modifiers().check_debuffs_removal(mid_boss_->luck());
}

// バトルの攻撃ターン開始時の処理
void MidBossDefaultBattleStrategy::on_attack_turn_start(const bool is_critical) {
	// リアクション開始フラグ
	is_reaction_start_ = false;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// クリティカルならクリティカル攻撃モーション
	if (is_critical) mid_boss_->mesh().forced_change_motion(MidBossMotion::CriticalAttack, false);
	// クリティカルじゃなければ、攻撃モーションにする
	else mid_boss_->mesh().forced_change_motion(MidBossMotion::Attack, false);
}

// バトルの防御ターン開始時の処理
void MidBossDefaultBattleStrategy::on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) {
	// ステータスを取得
	status_ = status;
	// ダメージを取得
	battle_damage_ = damage;
	// クリティカルフラグを取得
	is_critical_ = is_critical;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// アイドルモーションにする
	if (status_ != DefenderStatus::Defense) mid_boss_->mesh().change_motion(MidBossMotion::Idle);
	// 防御モーションにする
	else mid_boss_->mesh().change_motion(MidBossMotion::Defense);
}

// ターン終了
void MidBossDefaultBattleStrategy::battle_turn_end() {

}

// 戦闘終了
void MidBossDefaultBattleStrategy::battle_end() {
	// バフ・デバフを初期化
	Modifier zero;
	mid_boss_->modifiers().set_buff(zero);
	mid_boss_->modifiers().set_debuff(zero);
	dead_effect_.play(Effect::MonsterDead).position(mid_boss_->transform().position());
	is_battle_end_completed_ = true;
}

// 戦闘アクション
void MidBossDefaultBattleStrategy::battle_action(float delta_time) {
	// モーションが終了したら、ターン終了
	if (mid_boss_->mesh().is_motion_finished()) {
		// バトルターン完了フラグ
		is_turn_completed_ = true;
		// アイドル状態へ
		mid_boss_->mesh().change_motion(MidBossMotion::Idle);
	}
}

// 戦闘リアクション開始処理
void MidBossDefaultBattleStrategy::on_battle_reaction_start() {
	// ダメージ処理
	mid_boss_->take_damage(battle_damage_);

	const GSvector3 pos = mid_boss_->transform().position() + MID_BOSS_HIT_OFFSET +
		(mid_boss_->transform().forward() * MID_BOSS_FORWARD_OFFSET);

	switch (status_)
	{
	case DefenderStatus::Attack: mid_boss_->mesh().forced_change_motion(MidBossMotion::Damage, false);
		hit_effect_.play(Effect::HitBlow).position(pos);
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	case DefenderStatus::Avoid: mid_boss_->mesh().forced_change_motion(MidBossMotion::Avoid, false); break;
	case DefenderStatus::Defense: mid_boss_->mesh().forced_change_motion(MidBossMotion::DefenseDamage, false);
		hit_effect_.play(Effect::HitGuard).position(pos).rotation(GSvector3{ 0.0f, 0.0f, mid_boss_->transform().eulerAngles().y});
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	}

	// 死亡していたら死亡モーション
	if (mid_boss_->hp() <= 0) {
		mid_boss_->mesh().forced_change_motion(MidBossMotion::Die, false);
	}
#ifdef _DEBUG
	switch (status_)
	{
	case DefenderStatus::Attack: std::cout << "[ ダメージ ]" << std::endl; break;
	case DefenderStatus::Avoid: std::cout << "[ 回避 ]" << std::endl; break;
	case DefenderStatus::Defense: std::cout << "[ 防御 ]" << std::endl; break;
	}
	if (is_critical_) std::cout << "クリティカルダメージ" << std::endl;
	std::cout << "敵は " << (int)battle_damage_ << "ダメージ 受けた hp:" << mid_boss_->hp() << std::endl;
#endif // _DEBUG
}

// 戦闘リアクション
void MidBossDefaultBattleStrategy::battle_reaction(float delta_time) {
	// モーションが終了したら、ターン終了
	if (mid_boss_->mesh().is_motion_finished()) {

		// HPが0かつ死亡フラグが立ってない時
		if (mid_boss_->hp() <= 0 && !mid_boss_->is_dead()) {
			// 死亡処理を実行
			mid_boss_->die(delta_time);
			return;
		}

		// バトルターン完了フラグ
		is_turn_completed_ = true;
		// アイドル状態へ
		mid_boss_->mesh().change_motion(MidBossMotion::Idle);
	}
}

// 素早さの取得
speed_t MidBossDefaultBattleStrategy::agility(void) const noexcept {
	return (speed_t)(mid_boss_->speed() + mid_boss_->speed_modifier());
}

// 戦闘コンテキストを取得
AttackerBattleContext MidBossDefaultBattleStrategy::attacker_battle_context(void) const noexcept {
	return AttackerBattleContext{ (attack_t)(mid_boss_->attack() + mid_boss_->attack_modifier()), (luck_t)(mid_boss_->luck() + mid_boss_->luck_modifier()) };
}

// 防御コンテキストを取得
DefenderBattleContext MidBossDefaultBattleStrategy::defender_battle_context(void) const noexcept {
	return DefenderBattleContext{ (defense_t)(mid_boss_->defense() + mid_boss_->defense_modifier()), mid_boss_->hp(), mid_boss_->max_hp() };
}

// 敗北フラグ
bool MidBossDefaultBattleStrategy::is_defeated(void) const noexcept {
	return mid_boss_->is_dead();
}

// エフェクトの更新処理
void MidBossDefaultBattleStrategy::battle_effect_update(float delta_time) {
	// 攻撃エフェクト
	attack_effect_.update(delta_time);
	// ダメージエフェクト
	hit_effect_.update(delta_time);
	// 死亡エフェクト
	dead_effect_.update(delta_time);
	// デバフエフェクト
	debuff_effect_.update(delta_time);
}
