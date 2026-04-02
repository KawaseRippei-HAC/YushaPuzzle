#include "Systems/LoggableRandomizer.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

/*定数*/
// ログファイルディレクトリ名
static constexpr char LOG_DIRECTRY_NAME[] = { "Logs" };
// 基底ファイル名
static constexpr char BASE_FILE_NAME[] = { "random_log" };
// テキスト拡張子
static constexpr char TEXT_EXT[] = { ".txt" };

/*実装*/
// デストラクタ
LoggableRandomizer::~LoggableRandomizer() {
	if (log_stream_.is_open()) log_stream_.close();
}

// 初期化
void LoggableRandomizer::initialize(bool should_logging) {
	namespace ch = std::chrono;

	// 現在の時刻からシード値を設定
	auto now = ch::high_resolution_clock::now();
	auto seed = now.time_since_epoch().count();
	instance().engine_.seed(static_cast<unsigned long long>(seed));

	// ログが無効なら以降を無視
	if (!should_logging) return;

	// システムクロックに変換
	const auto sys_clock = time_point_cast<ch::system_clock::duration>(
		now - ch::high_resolution_clock::now() + ch::system_clock::now()
	);
	const std::time_t now_clock = ch::system_clock::to_time_t(sys_clock);

	// ローカル時刻に変換
	std::tm local_time{};
#ifdef _WIN32
	localtime_s(&local_time, &now_clock);
#else
	localtime_r(&now_clock, &local_time);
#endif

	// ファイル名の結合
	std::ostringstream file_name;
	const auto formated_time = std::put_time(&local_time, "%Y-%m-%d-%H-%M-%S");
	file_name << BASE_FILE_NAME << '_' << formated_time << TEXT_EXT;

	// ログファイルディレクトリの作成
	std::filesystem::create_directories(LOG_DIRECTRY_NAME);

	// エイリアス
	std::ofstream& log_stream = instance().log_stream_;

	// ファイルパスの結合
	std::ostringstream file_path;
	file_path << LOG_DIRECTRY_NAME << '/' << file_name.str();

	// ファイルオープン
	log_stream.open(file_path.str(), std::ios::out);
	// 今回のシード値を出力
	LoggableRandomizer::log(std::format("seed: {}", seed));
}

// ログファイルへの書き込み
void LoggableRandomizer::log(const std::string& text) {
	instance().log_impl(text);
}

// インスタンスの取得
LoggableRandomizer& LoggableRandomizer::instance() {
	static LoggableRandomizer instance;
	return instance;
}

// 関数呼び出し演算子オーバーロード
LoggableRandomizer::result_type LoggableRandomizer::operator()() {
	// 乱数生成
	result_type value = engine_();
	// 値を出力
	LoggableRandomizer::log(std::format("base: {}", value));
	return value;
}

// ログファイルへの書き込み実装
void LoggableRandomizer::log_impl(const std::string& text) {
	// ログファイルが開かれていたら出力
	if (log_stream_.is_open()) log_stream_ << text << "\n";
}