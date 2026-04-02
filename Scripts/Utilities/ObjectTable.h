#ifndef UTILITIES_OBJECT_TABLE_H_
#define UTILITIES_OBJECT_TABLE_H_

#include "Events/InnEvent.h"
#include "Events/EncounterEvent.h"
#include "Events/FallInAbyssEvent.h"

template<typename TFunc>
struct FuncTraits;

template<class TReturn, typename ...Args>
struct FuncTraits<TReturn(*)(Args...)> {
	using return_type = TReturn;
	using argument_types = std::tuple<Args...>;
};

// オブジェクトテーブルクラス
template<class TFactory, class ...TDerived>
class ObjectTable {
	// 関数ポインタ分解
	using traits = FuncTraits<TFactory>;

public:
	// 生成関数ポインタ
	using create_func = TFactory;
	// 戻り値の型
	using return_type = typename traits::return_type;
	// 引数の型
	using argument_types = typename traits::argument_types;
	// オブジェクト数
	static inline constexpr size_t count{ sizeof...(TDerived) };

	// オブジェクトの生成
	template<typename ...Args>
		requires std::is_same_v<std::tuple<Args...>, argument_types>
	static return_type create(size_t index, Args&& ...args) {
		if (create_func func = get_create_func(index)) return func(args...);
		// TODO: エラーメッセージの記述
		throw std::runtime_error{ "" };
	}

	// 生成関数の取得
	static constexpr create_func get_create_func(size_t index) {
		if (index >= count) return nullptr;
		return table_[index];
	}

private:
	template<class T, typename ...Args>
	static constexpr return_type create_func_impl(Args&& ...args) noexcept {
		return new T{ std::forward<Args>(args)... };
	}

	template<class T, size_t ...t_index>
	static consteval create_func make_create_func_impl(std::index_sequence<t_index...>) noexcept {
		return &create_func_impl<T, std::tuple_element_t<t_index, argument_types>...>;
	}

	// 生成関数の作成
	template<class T>
	static consteval create_func make_create_func(void) noexcept {
		return make_create_func_impl<T>(
			std::make_index_sequence<std::tuple_size_v<argument_types>>{}
		);
	}

	// 生成関数テーブル
	static inline constexpr std::array<create_func, count> table_{ make_create_func<TDerived>()... };
};

#endif // UTILITIES_OBJECT_TABLE_H_