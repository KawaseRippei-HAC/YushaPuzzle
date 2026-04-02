#include "Characters/Monsters/MidBossPool.h"

/*À‘•*/
// ƒ‰ƒ“ƒ_ƒ€¶¬
MidBossCharacter* MidBossPool::create() {
	return this->random_create();
}