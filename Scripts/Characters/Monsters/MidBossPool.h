#ifndef CHARACTERS_MONSTERS_MID_BOSS_POOL_H_
#define CHARACTERS_MONSTERS_MID_BOSS_POOL_H_

#include "Utilities/ObjectPool.h"
#include "Characters/Monsters/MidBossTable.h"

// 強敵プールクラス
class MidBossPool :
	public ObjectPool<MidBossTable> {
public:
	MidBossCharacter* create(void);
};

#endif // CHARACTERS_MONSTERS_MID_BOSS_POOL_H_