#include "Systems/UUID64.h"

#include <random>
#include <chrono>

/*定数*/
// 48ビットマスク
static constexpr uint64_t BIT_48_MASK{ 0xFFFFFFFFFFFFULL };
// シフトビット数
static constexpr int SHIFT_BIT_COUNT{ 16 };
// 16ビット最大値
static constexpr int BIT_16_MAX{ 0xFFFF };

/*実装*/
// UUIDの生成
uint64_t UUID64::generate() noexcept {
	/*Random number generator*/
	static std::mt19937_64 rng(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> distribution;
	return static_cast<uint64_t>(distribution(rng));
}

// 時系列ソート可能UUIDの生成
uint64_t UUID64::timesortable() noexcept {
	using namespace std::chrono;

	// ミリ秒単位の時刻を取得
	uint64_t timestamp_ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count();

	// 上位48ビットの取り出し
	uint64_t timestamp_part = (timestamp_ms & BIT_48_MASK) << SHIFT_BIT_COUNT;

	/*Random number generator*/
	static std::mt19937 rng(static_cast<uint32_t>(timestamp_ms));
	static std::uniform_int_distribution<uint16_t> distribution(0, BIT_16_MAX);
	uint64_t random_part = distribution(rng);

	return timestamp_part | random_part;
}