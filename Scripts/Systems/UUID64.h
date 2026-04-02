#ifndef SYSTEMS_UUID_H_
#define SYSTEMS_UUID_H_

// 自作UUIDクラス
class UUID64 {
public:
	/*インスタンス生成禁止*/
	UUID64() = delete;

	// UUIDの生成
	static uint64_t generate(void) noexcept;
	// 時系列ソート可能UUIDの生成
	static uint64_t timesortable(void) noexcept;
};

#endif // SYSTEMS_UUID_H_