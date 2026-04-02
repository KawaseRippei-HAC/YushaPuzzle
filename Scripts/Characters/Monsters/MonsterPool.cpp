#include "MonsterPool.h"

/*実装*/
// モンスターの生成
MonsterBase* MonsterPool::create() {
	return this->random_create();
}

// レベル参照生成
MonsterBase* MonsterPool::create_with_level(level_t level) {
	if (pool_.empty()) return nullptr;

	std::vector<size_t> indices;
	for (size_t i = 0; i < pool_.size(); ++i) {
		if (level >= MonsterTable::get_min_level(i)) indices.push_back(i);
	}

	const size_t index = Randomizer::generate<size_t>(0, indices.size() - 1);

	return pool_[indices[index]]();
}