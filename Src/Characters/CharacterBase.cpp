#include "Characters/CharacterBase.h"

#include <algorithm>

/*定数*/
// 最大移動速度倍率
static constexpr float MAX_MOVE_SPEED_MULTIPLIER{ 4.0f };

/*実装*/
// コンストラクタ
CharacterBase::CharacterBase(GSuint mesh, GSuint motion, bool loop) :
	CharacterBase{ mesh, mesh, mesh, motion, loop } {
}

// コンストラクタ
CharacterBase::CharacterBase(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop) :
	mesh_{ mesh, skeleton, animation, motion, loop } {
}

// 名前の取得
const std::string& CharacterBase::name() const noexcept {
	return name_;
}

// レベルの取得
level_t CharacterBase::level() const noexcept {
	return level_;
}

// HPの取得
point_t CharacterBase::hp() const noexcept {
	return hp_;
}

// 最大HPの取得
point_t CharacterBase::max_hp() const noexcept {
	return max_hp_;
}

// 攻撃力の取得
attack_t CharacterBase::attack() const noexcept {
	return attack_;
}

// 攻撃力の修正値（バフとデバフ）取得
attack_t CharacterBase::attack_modifier(void) const noexcept {
	return modifiers_.get_modifiers().attack;
}

// 防御力の取得
defense_t CharacterBase::defense() const noexcept {
	return defense_;
}

// 防御力の修正値を取得（バフとデバフ）
defense_t CharacterBase::defense_modifier(void) const noexcept {
	return modifiers_.get_modifiers().defense;
}

// 素早さの取得
speed_t CharacterBase::speed() const noexcept {
	return speed_;
}

// 素早さの修正値を取得（バフとデバフ）
speed_t CharacterBase::speed_modifier(void) const noexcept {
	return modifiers_.get_modifiers().speed;
}

// 現在の移動速度倍率の取得
float CharacterBase::current_move_speed_multiplier() const noexcept {
	constexpr float max_speed{ MAX_SPEED - INITIAL_SPEED };
	const speed_t speed = this->speed() - INITIAL_SPEED;
	const float ratio = static_cast<float>(speed) / max_speed;
	return 1.0f + MAX_MOVE_SPEED_MULTIPLIER * ratio;
}

// 運の取得
luck_t CharacterBase::luck() const noexcept {
	return luck_;
}

// 運の修正値を取得（バフとデバフ）
luck_t CharacterBase::luck_modifier(void) const noexcept {
	return modifiers_.get_modifiers().luck;
}

// レベルアップ処理
void CharacterBase::level_up(level_t amount) noexcept {
	if (level_ >= MAX_LEVEL) return;
	level_ = std::min(static_cast<level_t>(level_ + amount), MAX_LEVEL);
}

// 回復処理
point_t CharacterBase::heal(point_t amount) noexcept {
	if (amount <= 0 || hp_ >= max_hp_) return 0;
	hp_ = std::min(static_cast<point_t>(hp_ + amount), max_hp_);
	return amount;
}

// 被ダメージ処理
point_t CharacterBase::take_damage(point_t amount) noexcept {
	if (amount <= 0 || hp_ <= 0) return 0;
	hp_ = std::max(hp_ - amount, 0);
	return amount;
}

// 最大HPの追加
void CharacterBase::add_max_hp(point_t amount) noexcept {
	max_hp_ = std::min(static_cast<point_t>(max_hp_ + amount), MAX_HP);
}

// 攻撃力の追加
void CharacterBase::add_attack(attack_t amount) noexcept {
	attack_ = std::min(static_cast<attack_t>(attack_ + amount), MAX_ATTACK);
}
// 防御力の追加
void CharacterBase::add_defense(defense_t amount) noexcept {
	defense_ = std::min(static_cast<defense_t>(defense_ + amount), MAX_DEFENSE);
}

// 素早さの追加
void CharacterBase::add_speed(speed_t amount) noexcept {
	speed_ = std::min(static_cast<speed_t>(speed_ + amount), MAX_SPEED);
}
// 運の追加
void CharacterBase::add_luck(luck_t amount) noexcept {
	luck_ = std::min(static_cast<luck_t>(luck_ + amount), MAX_LUCK);
}

// 死亡処理
void CharacterBase::die(float delta_time) {

}

// 死亡フラグ
bool CharacterBase::is_dead() const noexcept {
	return hp_ <= 0;
}

// お金の取得
gold_t CharacterBase::gold(void) const noexcept {
	return gold_;
}

// お金の計算
gold_t CharacterBase::calc_gold(gold_t amount) noexcept {
	return amount;
}

// メッシュの取得
AnimatedMesh& CharacterBase::mesh() noexcept {
	return mesh_;
}

// メッシュの取得
const AnimatedMesh& CharacterBase::mesh() const noexcept {
	return mesh_;
}

// トランスフォームの取得
GStransform& CharacterBase::transform() noexcept {
	return transform_;
}

// トランスフォームの取得
const GStransform& CharacterBase::transform() const noexcept {
	return transform_;
}

// バフ・デバフの取得
StatusModifiers& CharacterBase::modifiers(void) noexcept {
	return modifiers_;
}
// バフ・デバフの取得
const StatusModifiers& CharacterBase::modifiers(void) const noexcept {
	return modifiers_;
}