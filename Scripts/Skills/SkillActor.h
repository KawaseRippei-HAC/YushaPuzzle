#ifndef SKILLS_SKILL_ACTOR_H_
#define SKILLS_SKILL_ACTOR_H_

#include "Skills/SkillDefinition.h"

// スキルを持ったクラスのみ使用するクラス
class SkillActor {
public:
	~SkillActor();

	// スキルに使う条件を設定
	void set_skill_context(CharacterBase* owner, CharacterBase* target = nullptr, int tile_count = 0);

	// 所持スキルの取得
	std::vector<SkillState> skills();

	// 追加可能なスキルか
	bool is_addable_skill(const SkillId& add_skill_id);

	// スキルの追加
	bool add_skill(const SkillId& add_skill_id, int level = 1);

	// スキルの追加（配列）
	void add_skills(const std::vector<SkillState>& add_skills);

	// 戦闘外のスキル
	SkillModifierContext non_battle_skill();

	// 戦闘開始時のスキル
	void battle_start_skill();
	// ターン開始時のスキル
	SkillModifierContext turn_start_skill();

	// 攻撃スキル
	SkillModifierContext attack_skill();

	// スキルの削除
	void clear();

private:
	// パッシブスキル
	std::vector<SkillState> passive_skills_;
	// 条件付きパッシブスキル
	std::vector<SkillState> conditional_skills_;
	// 攻撃スキル
	std::vector<SkillState>attack_skills_;

	// 常時発動スキルのコンテキス
	SkillModifierContext passive_result_context_;
	// スキルコンテキスト
	SkillContext skill_context_;
};

#endif // SKILLS_SKILL_ACTOR_H_
