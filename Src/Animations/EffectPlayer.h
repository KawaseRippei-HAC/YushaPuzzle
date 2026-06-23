#ifndef ANIMATIONS_EFFECT_PLAYER_H_
#define ANIMATIONS_EFFECT_PLAYER_H_

#include <GStype.h>

// エフェクト再生クラス
class EffectPlayer {
public:
	EffectPlayer() = default;
	~EffectPlayer();

	// 更新
	void update(float delta_time);

	// エフェクトの再生
	EffectPlayer& play(GSuint effect, float duration = -1.0f) noexcept;
	// エフェクトの停止
	void stop(void) noexcept;

	// 位置の設定
	EffectPlayer& position(const GSvector3& position) noexcept;
	// 回転の設定
	EffectPlayer& rotation(const GSvector3& rotation) noexcept;
	// 拡大縮小の設定
	EffectPlayer& scale(const GSvector3& scale) noexcept;
	// 色の設定
	EffectPlayer& color(const GScolor& color) noexcept;

private:
	GSint handle_{ 0 };
	float duration_{ 0.0f };
	float elapsed_{ 0.0f };
};

#endif // ANIMATIONS_EFFECT_PLAYER_H_