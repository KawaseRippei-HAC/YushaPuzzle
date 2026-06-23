#ifndef CHARCTERS_DEMON_KING_LEVEL_UP_H_
#define CHARCTERS_DEMON_KING_LEVEL_UP_H_
#include "Interfaces/ILevelUp.h"
class DemonKingCharacter;


// 魔王のレベルアップ
class DemonKingLevelUp : public ILevelUp {
public:
	// コンストラクタ
	DemonKingLevelUp(DemonKingCharacter* demon_king);

	// レベルアップの条件勇者なら経験値魔王ならマス数
	void execute(int tile_count) override;
	// パラメータアップ
	void pramater_up() override;
	// スキルの獲得
	void add_skill() override;

private:
	// ランダムでスキルを選択
	void random_skill();

private:
	DemonKingCharacter* character_{ nullptr };

	// 前回のマス数
	int prev_count_{ 0 };
};

#endif
