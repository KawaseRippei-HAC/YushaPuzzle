#ifndef UTILITIES_OBJECT_POOL_HPP_
#define UTILITIES_OBJECT_POOL_HPP_

#include "Utilities/ObjectPool.h"

#include <sstream>
#include <fstream>
#include <stdexcept>

#include "Systems/LoggableRandomizer.h"

/*実装*/
// コンストラクタ
template<typename TTable>
void ObjectPool<TTable>::load(const std::string& file_name) {
	std::ifstream file{ file_name };
	if (!file.is_open())
		// TODO: エラーメッセージの記述
		throw std::runtime_error{ "" };

	std::string line{ "" };
	while (std::getline(file, line)) {
		const size_t index = static_cast<size_t>(std::stoi(line));
		if (index < table_count_) pool_.push_back(TTable::get_create_func(index));
	}
}

// オブジェクトのランダム生成
template<typename TTable>
template<typename ...Args>
ObjectPool<TTable>::return_type ObjectPool<TTable>::random_create(Args ...args) {
	if (pool_.empty()) return nullptr;
	const size_t index = Randomizer::generate<size_t>(0, pool_.size() - 1);
	return pool_[index](args...);
}

#endif //UTILITIES_OBJECT_POOL_HPP_