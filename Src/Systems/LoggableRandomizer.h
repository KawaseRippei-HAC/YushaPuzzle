#ifndef SYSTEMS_LOGGABLE_RANDOMIZER_H_
#define SYSTEMS_LOGGABLE_RANDOMIZER_H_

#include <string>
#include <format>
#include <random>
#include <fstream>

// 記録可能乱数生成クラス
class LoggableRandomizer {
public:
	~LoggableRandomizer();

	// 初期化
	static void initialize(bool should_logging);

	// 乱数生成
	template<typename TType>
	[[nodiscard]] static TType generate(TType min, TType max);
	// パーセンテージ
	template<typename TType>
		requires std::is_floating_point_v<TType>
	[[nodiscard]] static bool percentage(TType percent);

	// ログファイルへの書き込み
	static void log(const std::string& text);

	// インスタンスの取得
	static LoggableRandomizer& instance(void);

	/*STL用ユーティリティ*/

	using result_type = std::mt19937_64::result_type;

	static constexpr result_type min() { return std::mt19937_64::min(); }
	static constexpr result_type max() { return std::mt19937_64::max(); }

	result_type operator()(void);

private:
	/*外部からのインスタンス生成禁止*/
	LoggableRandomizer() = default;

	// ログファイルへの書き込み実装
	void log_impl(const std::string& text);

private:
	// 乱数エンジン
	std::mt19937_64 engine_;
	// ログ出力ファイルストリーム
	std::ofstream log_stream_;
};

/*テンプレート関数実装*/
// 乱数生成
template<typename TType>
TType LoggableRandomizer::generate(TType min, TType max) {
	if constexpr (std::is_integral_v<TType>) {
		std::uniform_int_distribution<TType> dist{ min, max };
		TType value = dist(instance());
		LoggableRandomizer::log(std::format("clamped: {}", value));
		return value;
	}
	else if (std::is_floating_point_v<TType>) {
		std::uniform_real_distribution<TType> dist{ min, max };
		TType value = dist(instance());
		LoggableRandomizer::log(std::format("clamped: {}", value));
		return value;
	}
	else static_assert(std::is_arithmetic_v<TType>, "型が不適切です");
}

// パーセンテージ
template<typename TType>
	requires std::is_floating_point_v<TType>
bool LoggableRandomizer::percentage(TType percent) {
	LoggableRandomizer::log("<percentage>");
	const TType value = LoggableRandomizer::generate(static_cast<TType>(0), static_cast<TType>(100));
	const bool result = value <= percent;
	LoggableRandomizer::log(std::format("<percentage = {}>", result));
	return result;
}

// 乱数生成クラス
using Randomizer = LoggableRandomizer;

#endif // SYSTEMS_LOGGABLE_RANDOMIZER_H_