#ifndef SCREENS_SKILL_SELECT_SCREEN_H_
#define SCREENS_SKILL_SELECT_SCREEN_H_

#include "Interfaces/IScreen.h"
#include "Screens/ScreenManager.h"
#include "UI/ButtonManager.h"
#include "Characters/Heroes/HeroCharacter.h"

class SkillDefinition;

// スキル獲得画面
class SkillSelectScreen : public IScreen {
public:
	/// <param name="screen_manager">スクリーンマネージャー</param>
	/// <param name="hero">勇者</param>
	SkillSelectScreen(HeroCharacter* hero);
	~SkillSelectScreen();

	// 初期化
	void initialize() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

private:

	// 獲得できるスキルがないか
	bool is_no_skills_{ false };

	/* --- マネージャー --- */
	// ボタン管理クラス
	ButtonManager button_manager_;
	// 勇者のパラメータ
	HeroCharacter* hero_{ nullptr };

	// スキル名
	std::string skill_name_{ "" };
	// スキル説明
	std::string skill_description_{ "" };
};

#endif // SCREENS_SKILL_SELECT_SCREEN_H_