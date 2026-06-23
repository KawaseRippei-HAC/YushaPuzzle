#include "Systems/SkillSystem.h"
#include "Systems/LoggableRandomizer.h"
#include "Skills/SkillTable.h"
#include "Skills/SkillActor.h"

// スキルの実行
SkillModifierContext SkillSystem::skill_execute(std::vector<SkillState>& skills, SkillContext& skill_context) {
	SkillModifierContext result;
	if (!skills.empty()) {
		for (auto& skill : skills) {
			// スキルのデータを取得
			const SkillDefinition* skill_definition = SkillTable::get_skill_data(skill.id);
			// 発動条件が合っていなければ次のスキルへ
			if (!skill_definition->condition(skill_context)) continue;

			// スキル実行後の修正値を計算
			result += set_modifier_context(*skill_definition, skill_context, skill.level);
		}
	}
	return result;
}

// 攻撃スキルの実行
SkillModifierContext SkillSystem::attack_skill(std::vector<SkillState>& attack_skills, SkillContext& skill_context) {

	SkillModifierContext result_context;

	// スキルがない時か、40%の確率で攻撃する
	if (attack_skills.empty() || LoggableRandomizer::generate(0, 100) <= 40) {
#ifdef _TEST
		std::cout << "<< 通常攻撃 >>" << std::endl;
		std::cout << "hp" << (int)result_context.buffs.hp << " attack" << (int)result_context.buffs.attack << " defense" << (int)result_context.buffs.defense
			<< " speed" << (int)result_context.buffs.speed << " luck" << (int)result_context.buffs.luck << std::endl;
#endif
		// 何もせず終了
		return result_context;
	}

	// 使用可能スキル
	std::vector<SkillState*> available_skills;
	for (auto& skill : attack_skills) {
		// スキルのクールタイムが0以下かつ発動条件を満たしている場合、使用可能とする
		if (skill.cool_timer <= 0 && SkillTable::get_skill_data(skill.id)->condition(skill_context)) {
			available_skills.push_back(&skill);
		}
	}

	// 使用可能なスキルがあれば
	if (!available_skills.empty()) {
		// サイズを取得
		const int max = available_skills.size();
		// ランダムで抽選
		const int num = LoggableRandomizer::generate(0, max - 1);

		// スキル情報を取得
		const SkillDefinition* skill_data = SkillTable::get_skill_data(available_skills[num]->id);

		// 攻撃スキル実行後の修正値を計算
		result_context += set_modifier_context(*skill_data, skill_context, available_skills[num]->level);

#ifdef _TEST
		std::cout << "<< スキル発動 >> スキル名 : " << skill_data.name << std::endl;
		std::cout << "hp" << (int)result_context.buffs.hp << " attack" << (int)result_context.buffs.attack << " defense" << (int)result_context.buffs.defense
			<< " speed" << (int)result_context.buffs.speed << " luck" << (int)result_context.buffs.luck << std::endl;
#endif
		// クールタイムをセット
		available_skills[num]->cool_timer = skill_data->max_cool_time;
	}

	// 結果を返す
	return result_context;
}

// スキルの修正値を設定
SkillModifierContext SkillSystem::set_modifier_context(const SkillDefinition& skill, const SkillContext& skill_context, const int level) {
	SkillModifierContext result;

	// バフスキルの場合
	if (skill.is_buff_skill) result.buffs = skill.effect(skill_context) * level;
	// デバフスキルの場合
	else result.target_debuffs = skill.effect(skill_context) * level;

	return result;
}
