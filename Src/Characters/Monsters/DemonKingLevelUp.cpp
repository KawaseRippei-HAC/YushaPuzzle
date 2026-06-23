#include "Characters/Monsters/DemonKingLevelUp.h"
#include "Characters/Monsters/DemonKingCharacter.h"
#include "Systems/LoggableRandomizer.h"
#include "Skills/SkillTable.h"

// 追加で上がる値
static constexpr int ADD_PARAMATER{ 5 };
// スキル獲得のレベル（この倍数のレベルになる時、スキル獲得）
static constexpr int SKILL_ACQUISITION_LEVEL{ 1 };

// コンストラクタ
DemonKingLevelUp::DemonKingLevelUp(DemonKingCharacter* demon_king) : character_{ demon_king } {
}

// レベルアップの条件勇者なら経験値魔王ならマス数
void DemonKingLevelUp::execute(int amount) {
	// 今回使用した数を求める
	// 前回との差を出す
	const int count = amount - prev_count_;

	// 経験値追加
	character_->add_experience(count);
	// 追加して必要経験値を上回っていた場合
	while (character_->experience() >= character_->required_experience()) {
		// 差分を出す
		int num = character_->experience() - character_->required_experience();
		// 経験値を0にする
		character_->reset_experience();
		// 1レベル上がった後の経験値にする
		character_->add_experience(num);
		character_->level_up(1);

		// パラメータ上昇
		pramater_up();

		// スキルを所持できる場合、スキル獲得
		if (character_->get_battle_actor().skill_actor() != nullptr) {
			// スキル獲得
			add_skill();
		}
	}

	// 前回のデータとする
	prev_count_ = amount;
}
// パラメータアップ
void DemonKingLevelUp::pramater_up() {
	character_->add_max_hp(ADD_PARAMATER * 10);
	character_->heal(ADD_PARAMATER * 10);
	character_->add_attack(ADD_PARAMATER);
	character_->add_defense(ADD_PARAMATER);
	character_->add_speed(ADD_PARAMATER);
	character_->add_luck(ADD_PARAMATER);
}
// スキルの獲得
void DemonKingLevelUp::add_skill() {
	// スキル獲得レベルならスキル選択画面を追加
	if (character_->level() % SKILL_ACQUISITION_LEVEL == 0) {
		// TODO: 固定レベルで固定スキルを取得
		// 仮でスキルをランダムで獲得
		random_skill();
	}
}

// ランダムでスキルを選択
void DemonKingLevelUp::random_skill() {
	// 全スキルID
	std::vector<SkillId> all_skills = {
		SkillId::Rigidity,
		SkillId::ReinforcingBar,
		SkillId::Blessing,
		SkillId::Agile,
		SkillId::GoodLuck,
		SkillId::Intimidation
	};

	// ランダムでスキルを追加
	if (!all_skills.empty()) {
		int max = all_skills.size() - 1;
		while (max >= 0)
		{
			// ランダムなスキル
			int skill_id = LoggableRandomizer::generate(0, max);
			SkillId find_skill = all_skills[skill_id];
			// スキル検索
			if (character_->get_battle_actor().skill_actor()->is_addable_skill(find_skill)) {
				// スキルを追加できる場合スキルを追加
				character_->get_battle_actor().skill_actor()->add_skill(find_skill);
				return;
			}
			// スキルを追加出来なかった場合
			// 今回判定したスキルは削除
			all_skills[skill_id] = all_skills.back();
			all_skills.pop_back();
			--max;
		}
	}
}
