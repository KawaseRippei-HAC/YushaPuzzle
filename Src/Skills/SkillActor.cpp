#include "Skills/SkillActor.h"
#include "SkillTable.h"
#include "Systems/SkillSystem.h"

// デストラクタ
SkillActor::~SkillActor() {
	// スキルの削除
	clear();
}

// スキルに使う条件を設定
void SkillActor::set_skill_context(CharacterBase* owner, CharacterBase* target, int tile_count) {
	// コンテキスト初期化
	skill_context_.owner = owner;
	// 敵を設定
	skill_context_.target = target;
	// マップ数
	skill_context_.tile_count = tile_count;
}

// 所持スキルの取得
std::vector<SkillState> SkillActor::skills() {

	std::vector<SkillState> skills;
	// スキルの結合
	skills.insert(skills.end(), passive_skills_.begin(), passive_skills_.end());
	skills.insert(skills.end(), conditional_skills_.begin(), conditional_skills_.end());
	skills.insert(skills.end(), attack_skills_.begin(), attack_skills_.end());
	// 結合したスキルを返す
	return skills;
}

// 追加可能なスキルか
bool SkillActor::is_addable_skill(const SkillId& add_skill_id) {
	if (add_skill_id == SkillId::None) return false;	// 失敗

	std::vector<SkillState>* skills = nullptr;
	// 発動するタイミングに応じて振り分ける 該当しない場合は、失敗とする
	switch (SkillTable::get_skill_data(add_skill_id)->timing) {
	case ActivationTiming::Passive: skills = &passive_skills_; break;
	case ActivationTiming::Conditional: skills = &conditional_skills_; break;
	case ActivationTiming::Attack: skills = &attack_skills_; break;
	default: return false; break;	// 失敗
	}
	// キャラクターがスキルを持っているか検索
	auto it = std::find(skills->begin(), skills->end(), add_skill_id);
	// 持っている場合はレベルが最大じゃないか
	if (it != skills->end()) {
		// 追加するスキルのレベルが最大値だったら、失敗
		if (it->level >= SKILL_LVEVL_MAX) return false;
	}
	// 成功
	return true;
}

// スキルの追加
bool SkillActor::add_skill(const SkillId& add_skill_id, int level) {
	if (add_skill_id == SkillId::None) return false;	// 失敗

	std::vector<SkillState>* skills = nullptr;
	// 発動するタイミングに応じて振り分ける 該当しない場合は、失敗とする
	switch (SkillTable::get_skill_data(add_skill_id)->timing) {
	case ActivationTiming::Passive: skills = &passive_skills_; break;
	case ActivationTiming::Conditional: skills = &conditional_skills_; break;
	case ActivationTiming::Attack: skills = &attack_skills_; break;
	default: return false; break;	// 失敗
	}
	auto it = std::find(skills->begin(), skills->end(), add_skill_id);
	// 所持スキル
	if (it != skills->end()) {
		// 所持スキルの場合、スキルのレベルアップ
		it->level += level;
		it->level = MIN(it->level, SKILL_LVEVL_MAX);
	}
	// 未所持のスキル
	else {
		// 追加
		skills->push_back(SkillState{ .id = add_skill_id, .level = level });
	}
	// 成功
	return true;
}

// スキルの追加（配列）
void SkillActor::add_skills(const std::vector<SkillState>& add_skills) {
	for (auto& skill : add_skills) {
		add_skill(skill.id, skill.level);
	}
}

// スキル追加時の実行
SkillModifierContext SkillActor::non_battle_skill() {
	SkillModifierContext skill_modifiers =
		SkillSystem::skill_execute(passive_skills_, skill_context_) +
		SkillSystem::skill_execute(conditional_skills_, skill_context_);
	return skill_modifiers;
}

// 戦闘開始時のスキル
void SkillActor::battle_start_skill() {
	passive_result_context_ = SkillSystem::skill_execute(passive_skills_, skill_context_);
}
// ターン開始時のスキル
SkillModifierContext SkillActor::turn_start_skill() {
	// 攻撃スキルのクールタイムを減らす
	if (!attack_skills_.empty()) {
		for (auto& skill : attack_skills_) {
			--skill.cool_timer;
		}
	}
	// スキルの実行結果を返す
	return SkillSystem::skill_execute(conditional_skills_, skill_context_) + passive_result_context_;
}

// 攻撃スキル
SkillModifierContext SkillActor::attack_skill() {
	// スキルの実行結果を返す
	return SkillSystem::attack_skill(attack_skills_, skill_context_);
}

// スキルの削除
void SkillActor::clear() {
	passive_skills_.clear();
	conditional_skills_.clear();
	attack_skills_.clear();
}
