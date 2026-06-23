#ifndef UTILITIES_OBJECT_POOL_H_
#define UTILITIES_OBJECT_POOL_H_

#include <vector>
#include <string>

// オブジェクトプールクラス
template<typename TTable>
class ObjectPool {
	// 生成関数ポインタ
	using create_func = typename TTable::create_func;
	// 戻り値の型
	using return_type = typename TTable::return_type;
	// 引数の型
	using argument_types = typename TTable::argument_types;
	// 生成関数テーブル数
	inline static constexpr size_t table_count_{ TTable::count };

public:
	void load(const std::string& file_name);

	// オブジェクトのランダム生成
	template<typename ...Args>
	return_type random_create(Args ...args);

protected:
	// 生成関数プール
	std::vector<create_func> pool_;
};

#include "Utilities/ObjectPool.hpp"

#endif // UTILITIES_OBJECT_POOL_H_