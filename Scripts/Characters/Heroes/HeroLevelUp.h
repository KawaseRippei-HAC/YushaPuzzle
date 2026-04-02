#ifndef CHARCTERS_HERIS_HERO_LEVEL_UP_H_
#define CHARCTERS_HERIS_HERO_LEVEL_UP_H_
#include "Interfaces/ILevelUp.h"
class HeroCharacter;

// ヒーローのレベルアップ
class HeroLevelUp : public ILevelUp {
public:
	HeroLevelUp(HeroCharacter* heroCharacter);

	// レベルアップの条件勇者なら経験値魔王ならマス数
	void execute(int amount) override;
	// パラメータアップ
	void pramater_up() override;
	// スキルの獲得
	void add_skill() override;
private:
	// スキル選択画面を何枚出すか
	int skill_select_screens_{ 0 };

	HeroCharacter* character_{ nullptr };
};

#endif
