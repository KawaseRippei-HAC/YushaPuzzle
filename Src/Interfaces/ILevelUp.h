#ifndef INTERFACES_ILEVEL_UP_H_
#define INTERFACES_ILEVEL_UP_H_
#include "Characters/CharacterParameter.h"

// レベルアップ
// 継承して使用
class ILevelUp {
public:
	// レベルアップの条件勇者なら経験値魔王ならマス数
	virtual void execute(int amount) = 0;
	// パラメータアップ
	virtual void pramater_up() = 0;
	// スキルの獲得
	virtual void add_skill() = 0;
};

#endif // INTERFACES_ILEVEL_UP_H_
