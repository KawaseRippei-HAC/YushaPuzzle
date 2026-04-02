#ifndef CHARACTERS_MONSTERS_MONSTER_POOL_H_
#define CHARACTERS_MONSTERS_MONSTER_POOL_H_

#include "Utilities/ObjectPool.h"
#include "Characters/Monsters/MonsterTable.h"

// モンスタープールクラス
class MonsterPool :
	public ObjectPool<MonsterTable> {
public:
	MonsterBase* create(void);

	// レベル参照生成
	MonsterBase* create_with_level(level_t level);
};

#endif // CHARACTERS_MONSTERS_MONSTER_POOL_H_