#include "Characters/Heroes/HeroCharacter.h"

#include "Characters/Heroes/HeroIdleState.h"
#include "Characters/Heroes/HeroThinkState.h"
#include "Characters/Heroes/HeroMoveState.h"
#include "Characters/Heroes/HeroEventState.h"
#include "Skills/SkillTable.h"

// 勇者のオフセット
static const GSvector3 HERO_OFFSET{ GSvector3::up() * 1.3f };

// 通常のモーション速度
static constexpr float DEFAULT_MOTION_SPEED{ 1.0f };
// 回避モーションの速度
static constexpr float AVOID_MOTION_SPEED{ 2.0f };
// 勝利モーションの速度
static constexpr float VICTORY_MOTION_SPEED{ 2.0f };

/*実装*/
//コンストラクタ
HeroCharacter::HeroCharacter(TileGraph& graph) :
	CharacterBase{ Mesh::Hero }, hud_{ this } {
	// コンテキストの初期化
	context_.graph = &graph;
	context_.current_tile = graph.start_tile();
	context_.character = this;

	// トランスフォームの初期化
	transform_.position(context_.current_tile->world_position());
	transform_.lookAt(transform_.position() + GSvector3::right());

	// 状態の登録
	state_.register_state<HeroIdleState>(HeroState::Idle);
	state_.register_state<HeroThinkState>(HeroState::Think);
	state_.register_state<HeroMoveState>(HeroState::Move);
	state_.register_state<HeroEventState>(HeroState::Event);

	// 状態の変更
	state_.change_state(HeroState::Idle);

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

	// 武器をしまっているか
	is_weapon_holstered_ = true;

	// アニメーションイベントの追加
	mesh_.add_event(HeroMotion::Attack, 20, [&]() { is_reaction_start_ = true; });
	mesh_.add_event(HeroMotion::Attack, 15,
		[&]() {
			attack_effect_.play(Effect::HeroSlashAttack).
				position(transform_.position() + HERO_OFFSET).
				rotation(GSvector3{ 25.0f, 0.0f, transform_.eulerAngles().y});
			gsPlaySE(SE::Slash);
		});
	mesh_.add_event(HeroMotion::CriticalAttack, 20, [&]() { is_reaction_start_ = true; });
	mesh_.add_event(HeroMotion::CriticalAttack, 15,
		[&]() {
			attack_effect_.play(Effect::HeroSlashAttack).
				position(transform_.position() + HERO_OFFSET).
				rotation(GSvector3{ 90.0f, 0.0f, transform_.eulerAngles().y });
			gsPlaySE(SE::Slash);
		});

	heal_effect_.stop();

	// モーション速度の設定
	mesh_.motion_speed(0, DEFAULT_MOTION_SPEED);
}

// 更新
void HeroCharacter::update(float delta_time) {
	// 状態の更新
	state_.update(delta_time);
	// メッシュの更新
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());

	acquisition_animation_.update();
}

// 描画
void HeroCharacter::draw() const {
	// メッシュの描画
	mesh_.draw();
	// 武器の描画
	draw_all_weapons();

	acquisition_animation_.draw();
}

// GUI描画
void HeroCharacter::draw_gui() const {
	hud_.draw_gui();
}

// 現在のタイルの取得
const TileNode* HeroCharacter::current_tile() const noexcept {
	return context_.current_tile;
}

// 攻撃力の取得
attack_t HeroCharacter::attack() const noexcept {
	return attack_/* + weapon_->attack*/;
}

// 防御力の取得
defense_t HeroCharacter::defense() const noexcept {
	return defense_/* + shield_->defense*/;
}

// お金の計算
gold_t HeroCharacter::calc_gold(gold_t amount) noexcept {
	gold_ = CLAMP(gold_ + amount, 0, MAX_GOLD);
	return amount;
}

// 死亡処理
void HeroCharacter::die(float delta_time) {
	is_game_over_ = true;
}

// 死亡フラグ
bool HeroCharacter::is_dead() const noexcept {
	return is_game_over_;
}

// 思考の開始
void HeroCharacter::think_start() noexcept {
	state_.change_state(HeroState::Think);
}

// 移動の開始
void HeroCharacter::move_start() noexcept {
	context_.is_actioning = true;
}

// 行動終了フラグ
bool HeroCharacter::is_action_ended() const noexcept {
	return !context_.is_actioning;
}

// スキルのコンテキストを設定（スキルを持つ時だけオーバーライド）
void HeroCharacter::battle_set_skill_context(CharacterBase* target, int tile_count) {
	skill_actor_.set_skill_context(this, target, tile_count);
}

// バトル開始時の初期化
void HeroCharacter::battle_start_initialize() {
	// バトル開始時に常時発動スキルを計算しておく
	skill_actor_.battle_start_skill();

	is_heal_completed_ = false;
	is_battle_end_completed_ = false;
}
// デバフを追加
void HeroCharacter::battle_set_debuff(Modifier debuff) {
	modifiers_.set_debuff(debuff);
}
// デバフ解除判定
void HeroCharacter::battle_check_debuffs_removal() {

	// デバフの解除判定
	modifiers_.check_debuffs_removal(this->luck());

#ifdef _TEST
	Modifier test_modifier = modifiers_.get_modifiers();
	std::cout << "<バフ・デバフ> hp:" << (int)test_modifier.hp << " attack:" << (int)test_modifier.attack << " defense:" <<
		(int)test_modifier.defense << " speed:" << (int)test_modifier.speed << " luck:" << (int)test_modifier.luck << std::endl;
#endif
}

// ターン開始スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
Modifier HeroCharacter::battle_turn_start_skill() {
	// ターン開始スキル実行
	SkillModifierContext context = skill_actor_.turn_start_skill();
	// スキルによるバフを設定
	modifiers_.set_buff(context.buffs);
	// 相手に付与するデバフを返す
	return context.target_debuffs;
}
// 攻撃スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
Modifier HeroCharacter::battle_attack_skill() {
	// 攻撃スキル実行
	SkillModifierContext context = skill_actor_.attack_skill();
	// スキルによるバフを追加 *攻撃時は追加
	modifiers_.add_buff(context.buffs);
	// 相手に付与するデバフを返す
	return context.target_debuffs;
}

// バトルの攻撃ターン開始時の処理
void HeroCharacter::on_attack_turn_start(const bool is_critical) {
	// リアクション開始フラグ
	is_reaction_start_ = false;
	// バトルターン完了フラグ
	is_turn_completed_ = false;

	// クリティカルならクリティカル攻撃モーション
	if (is_critical) mesh_.forced_change_motion(HeroMotion::CriticalAttack, false); 
	// クリティカルじゃなければ、攻撃モーションにする
	else mesh_.forced_change_motion(HeroMotion::Attack, false);
}

// バトルの防御ターン開始時の処理
void HeroCharacter::on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) {
	// ステータスを取得
	status_ = status;
	// ダメージを取得
	battle_damage_ = damage;
	// クリティカルフラグを取得
	is_critical_ = is_critical;

	// バトルターン完了フラグ
	is_turn_completed_ = false;

	if (status_ == DefenderStatus::Defense) {
		// 防御モーションにする
		mesh_.change_motion(HeroMotion::Defense);
	}
	else {
		// アイドルモーションにする
		mesh_.change_motion(HeroMotion::BattleIdle);
	}
}

// ターン終了
void HeroCharacter::battle_turn_end() {

}

// 戦闘終了
void HeroCharacter::battle_end() {
	mesh_.motion_speed(0, VICTORY_MOTION_SPEED);
	mesh_.change_motion(HeroMotion::Victory, false);
	if(!gsIsPlaySE(SE::BattleEnd)) gsPlaySE(SE::BattleEnd);
	if (mesh_.is_motion_finished()) {
		// バフ・デバフを初期化
		Modifier zero;
		modifiers_.set_buff(skill_actor_.non_battle_skill().buffs);
		modifiers_.set_debuff(zero);
		// スキルの条件に必要な情報を初期化（スキル所持のみ）
		skill_actor_.set_skill_context(this);
		// 通常のモーション速度に戻す
		mesh_.motion_speed(0, DEFAULT_MOTION_SPEED);

		is_battle_end_completed_ = true;
	}
}

// 戦闘アクション
void HeroCharacter::battle_action(float delta_time) {
	//// リアクション開始（アニメーションイベントンない時用）
	//is_reaction_start_ = true;

	// モーションが終了したら、ターン終了
	if (mesh_.is_motion_finished()) {
		// バトルターン完了フラグ
		is_turn_completed_ = true;

		// アイドル状態へ
		mesh_.change_motion(HeroMotion::BattleIdle);
	}
#ifdef _TEST
	std::cout << "\n< 勇者の攻撃 >" << std::endl;
#endif // _TEST
}

// 戦闘リアクション開始処理
void HeroCharacter::on_battle_reaction_start() {
	// ダメージ処理
	this->take_damage(battle_damage_);

	const GSvector3 pos = transform_.position() + HERO_OFFSET;
	if (this->hp() <= 0) {
		mesh_.forced_change_motion(HeroMotion::Die, false);
		return;
	}

	switch (status_)
	{
	case DefenderStatus::Attack:
		mesh_.forced_change_motion(HeroMotion::Damage, false);
		hit_effect_.play(Effect::HitBlow).position(pos);
		if (is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	case DefenderStatus::Avoid:
		mesh_.motion_speed(0, AVOID_MOTION_SPEED);
		mesh_.forced_change_motion(HeroMotion::Avoid, false);
		break;
	case DefenderStatus::Defense:
		mesh_.forced_change_motion(HeroMotion::DefenseDamage, false);
		hit_effect_.play(Effect::HitGuard).position(pos).rotation(GSvector3{ 0.0f, 0.0f, transform_.eulerAngles().y });
		if(is_critical_) gsPlaySE(SE::CriticalHit);
		gsPlaySE(SE::DamagePunch);
		break;
	}

#ifdef _DEBUG
	switch (status_)
	{
	case DefenderStatus::Attack: std::cout << "[ ダメージ ]" << std::endl; break;
	case DefenderStatus::Avoid: std::cout << "[ 回避 ]" << std::endl; break;
	case DefenderStatus::Defense: std::cout << "[ 防御 ]" << std::endl; break;
	}
	if (is_critical_) std::cout << "クリティカルダメージ" << std::endl;
	std::cout << "勇者は " << (int)battle_damage_ << "ダメージ 受けた hp:" << hp_ << std::endl;
#endif // _DEBUG
}

// 戦闘リアクション
void HeroCharacter::battle_reaction(float delta_time) {
	// ターンが終わったら実行しない
	if (is_turn_completed_) return;

	// モーションが終了したら、ターン終了
	if (mesh_.is_motion_finished()) {
		if (this->hp() <= 0 && !this->is_dead()) {
			this->die(delta_time);
			return;
		}

		// バトルターン完了フラグ
		is_turn_completed_ = true;

		mesh_.motion_speed(0, DEFAULT_MOTION_SPEED);

		// アイドル状態へ
		mesh_.change_motion(HeroMotion::BattleIdle);
	}
}

// 素早さの取得
speed_t HeroCharacter::agility() const noexcept {
	return (speed_t)(this->speed() + this->speed_modifier());
}

// 回復処理
void HeroCharacter::battle_heal() {
	// バフ・デバフのHPを取得
	const point_t amount = modifiers_.get_modifiers().hp;

	// 回復スキルがない時は実行せず終了
	if (amount <= 0) {
		is_heal_completed_ = true;
		return;
	}

	// スキルモーションにする
	mesh_.change_motion(HeroMotion::Skill, false);

	if (!is_play_heal_effect_ && !is_heal_completed_) {
		gsPlaySE(SE::SkillActivation);
		gsPlaySE(SE::Heal);
		heal_effect_.play(Effect::HeroHeal).position(transform_.position());
		is_play_heal_effect_ = true;
	}

	if (mesh_.is_motion_finished() && is_play_heal_effect_) {
		// 回復
		heal(amount);
		mesh_.change_motion(HeroMotion::Idle);
		is_heal_completed_ = true;
		is_play_heal_effect_ = false;
	}
}
// 回復処理完了フラグ
bool HeroCharacter::is_heal_completed() const noexcept {
	return is_heal_completed_;
}

// 戦闘コンテキストを取得
AttackerBattleContext HeroCharacter::attacker_battle_context() const noexcept {
	return AttackerBattleContext{ (attack_t)(this->attack() + this->attack_modifier()), (luck_t)(this->luck() + this->luck_modifier())};
}

// 防御コンテキストを取得
DefenderBattleContext HeroCharacter::defender_battle_context() const noexcept {
	return DefenderBattleContext{ (defense_t)(this->defense() + this->defense_modifier()), this->hp(), this->max_hp() };
}

// 敗北フラグ
bool HeroCharacter::is_defeated() const noexcept {
	return this->is_dead();
}

// スキルの追加
void HeroCharacter::add_skill(const SkillId& add_id, int level) {
	skill_actor_.add_skill(add_id, level);
	skill_actor_.battle_start_skill();
}
// スキルアクター取得
SkillActor* HeroCharacter::skill_actor(void) noexcept {
	return &skill_actor_;
}

// ヒーローのレベルアップ処理取得
HeroLevelUp& HeroCharacter::hero_levelup(void) noexcept {
	return hero_levelup_;
}

// 現在の経験値量を取得
int HeroCharacter::experience(void) const noexcept {
	return exp_;
}
// 必要経験値量を取得
int HeroCharacter::required_experience(void) const noexcept {
	return required_exp_;
}
// ステータスポイント取得
int HeroCharacter::status_point(void) const noexcept {
	return status_poitn_;
}
// 経験値の追加
void HeroCharacter::add_experience(int amount) noexcept {
	exp_ += amount;
}

// 必要経験値量を追加
void HeroCharacter::add_required_experience(int amount) noexcept {

	required_exp_ += amount;
}
// 経験値のリセット
void HeroCharacter::reset_experience() noexcept {
	exp_ = 0;
}

// ステータスポイント追加
void HeroCharacter::add_status_point(int amount) {
	status_poitn_ += amount;
}

// 武器をしまうかのフラグを設定
void HeroCharacter::set_is_weapon_holstered(bool is_weapon_holstered) {
	is_weapon_holstered_ = is_weapon_holstered;
}

// 全武器の描画
void HeroCharacter::draw_all_weapons(void) const {
	// 手のボーンの位置に武器のメッシュを描画
	// 左20 右39
	// 背中 ボーン15

	if (is_weapon_holstered_) {
		// 非戦闘時の武器描画
		// 盾
		draw_weapon(Mesh::Shield, BACK_BONE_NUMBER, SHIELD_NORMAL_POSITION, SHIELD_NORMAL_ROTATE);
		// 剣
		draw_weapon(Mesh::Sword, BACK_BONE_NUMBER, SWORD_NORMAL_POSITION, SWORD_NORMAL_ROTATE);
	}
	else {
		// 戦闘時の武器描画
		// 盾
		draw_weapon(Mesh::Shield, LEFT_ARM_BONE_NUMBER, SHIELD_BATTLE_POSITION, SHIELD_BATTLE_ROTATE);
		// 剣
		draw_weapon(Mesh::Sword, RIGHT_ARM_BONE_NUMBER, SWORD_BATTLE_POSITION, SWORD_BATTLE_ROTATE);
	}
}

// 武器の描画
void HeroCharacter::draw_weapon(const GSuint mesh_id, const int bone, const float position[3], const float rotate[3]) const {
	glPushMatrix();
	const GSmatrix4 matrix = mesh_.bone_matrix(bone) * transform_.localToWorldMatrix();
	glMultMatrixf(matrix);
	glRotated(rotate[0], 1, 0, 0);
	glRotated(rotate[1], 0, 1, 0);
	glRotated(rotate[2], 0, 0, 1);
	glTranslatef(position[0], position[1], position[2]);
	gsDrawMesh(mesh_id);
	glPopMatrix();
}

// エフェクトの更新処理
void HeroCharacter::battle_effect_update(float delta_time) {
	// 回復エフェクト
	heal_effect_.update(delta_time);

	// 攻撃エフェクト
	attack_effect_.update(delta_time);
	// ダメージエフェクト
	hit_effect_.update(delta_time);
	// 死亡エフェクト
	dead_effect_.update(delta_time);
	// デバフエフェクト
	debuff_effect_.update(delta_time);
}

// イベント
void HeroCharacter::set_event_type(GSuint event_id) {
	event_type_ = event_id;
}

GSuint HeroCharacter::event_type() {
	return event_type_;
}

// 獲得アニメーションの追加
void HeroCharacter::add_acquisition_animation(GSuint texture_id, GSvector2 ratio, float scale) {
	acquisition_animation_.add(texture_id, ratio, transform_.position() + (GSvector3::up() * 2.5f), scale);
}
