#ifndef SYSTEMS_SKILL_SYSTEM_H_
#define SYSTEMS_SKILL_SYSTEM_H_

#include "Skills/SkillTable.h"
#include "Skills/SkillDefinition.h"

// スキルシステムクラス
// スキルの実行処理だけを持つクラス
class SkillSystem {
public:
	// スキルの実行
	static SkillModifierContext skill_execute(std::vector<SkillState>& skills, SkillContext& skill_context);

	// 攻撃スキルの実行
	static SkillModifierContext attack_skill(std::vector<SkillState>& attack_skills, SkillContext& skill_context);

private:
	/*インスタンス生成禁止*/
	SkillSystem() = delete;

	// スキルの修正値を設定
	static SkillModifierContext set_modifier_context(const SkillDefinition& skill, const SkillContext& skill_context, const int level);
};

#endif // !SYSTEMS_SKILL_SYSTEM_H_
