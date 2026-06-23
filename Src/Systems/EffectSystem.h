#ifndef SYSTEMS_EFFECT_SYSTEM_H_
#define SYSTEMS_EFFECT_SYSTEM_H_

// エフェクトシステムクラス
class EffectSystem final {
public:
	/*インスタンス生成禁止*/
	EffectSystem() = delete;

	// 初期化
	static void initialize(void) noexcept;
	// 終了
	static void finalize(void) noexcept;

	// 生存フラグ
	static bool is_alive(void) noexcept;

private:
	static inline bool is_alive_{ false };
};

#endif // SYSTEMS_EFFECT_SYSTEM_H_