#include "Data/ResultData.h"

// レベルを保存
void ResultData::save_level(level_t level) {
	level_ = level;
}

// ステータスを保存
void ResultData::save_status(point_t hp, point_t max_hp, attack_t attack, defense_t defense, speed_t speed, luck_t luck) {
	hp_ = hp;
	max_hp_ = max_hp;
	attack_ = attack;
	defense_ = defense;
	speed_ = speed;
	luck_ = luck;
}
// スキルを保存
void ResultData::save_skills(std::vector<SkillState> skills) {
	skills_ = skills;
}

// レベルを取得
level_t ResultData::level() {
	return level_;
}

// HPを取得
point_t ResultData::hp() {
	return hp_;
}
// 最大HP取得
point_t ResultData::max_hp() {
	return max_hp_;
}
// 攻撃力を取得
attack_t ResultData::attack() {
	return attack_;
}
// 防御力を取得
defense_t ResultData::defense() {
	return defense_;
}
// 素早さを取得
speed_t ResultData::speed() {
	return speed_;
}
// 運を取得
luck_t ResultData::luck() {
	return luck_;
}

// スキルを取得
std::vector<SkillState> ResultData::skills() {
	return skills_;
}
