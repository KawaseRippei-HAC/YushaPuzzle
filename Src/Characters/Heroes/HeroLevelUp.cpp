#include "Characters/Heroes/HeroLevelUp.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Systems/LoggableRandomizer.h"
#include "Skills/SkillTable.h"
#include "Screens/ScreenManager.h"

// 追加で上がる値
static constexpr int ADD_PARAMATER{ 3 };

// スキル獲得のレベル（この倍数のレベルになる時、スキル獲得）
static constexpr int SKILL_ACQUISITION_LEVEL{ 2 };

HeroLevelUp::HeroLevelUp(HeroCharacter* hero_character) : character_{ hero_character } {

}

// 戦闘終了時
void HeroLevelUp::execute(int amount) {
	// スキル獲得画面の枚数を初期化
	skill_select_screens_ = 0;

	// 経験値追加
	character_->add_experience(amount);
	// レベルアップ
	while (character_->experience() >= character_->required_experience()) {
		// 差分を出す
		int num = character_->experience() - character_->required_experience();
		// 経験値を0にする
		character_->reset_experience();
		character_->add_experience(num);
		character_->level_up(1);

		//レベルが上がるごとに必要経験値を追加
		character_->add_required_experience(5);

		// パラメータ上昇
		pramater_up();

		// レベルがスキル獲得レベル倍数の時は、スキル獲得画面の枚数を増やす
		if (character_->level() % SKILL_ACQUISITION_LEVEL == 0) {
			++skill_select_screens_;
		}
		// ステータスポイントの追加
		character_->add_status_point(1);
	}

	// レベルが上がった場合
	if (character_->status_point() > 0) {
		// SE再生
		gsPlaySE(SE::LevelUp);
		// ステータス割り振り画面を出す
		ScreenManager::push_screen(ScreenID::StatusAllocation);
		// スキルの追加
		add_skill();
	}
	
}
// パラメータアップ
void  HeroLevelUp::pramater_up() {
	character_->add_max_hp(ADD_PARAMATER * 10);
	character_->add_attack(ADD_PARAMATER);
	character_->add_defense(ADD_PARAMATER);
	character_->add_speed(ADD_PARAMATER);
	character_->add_luck(ADD_PARAMATER);
}
// スキルの獲得
void HeroLevelUp::add_skill() {
	// スキル獲得レベルならスキル選択画面を追加
	for (int i = 0; i < skill_select_screens_; ++i) {
		// スキル選択画面を出す
		ScreenManager::push_screen(ScreenID::SkillSelect);
	}
}
