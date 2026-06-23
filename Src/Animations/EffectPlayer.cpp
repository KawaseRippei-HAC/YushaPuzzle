#include "Animations/EffectPlayer.h"

#include <GSeffect.h>

#include "Systems/EffectSystem.h"

/*実装*/
// デストラクタ
EffectPlayer::~EffectPlayer() {
	if (!EffectSystem::is_alive()) return;
	if (duration_ < 0.0f) this->stop();
	if (elapsed_ < duration_) this->stop();
}

// 更新
void EffectPlayer::update(float delta_time) {
	if (duration_ <= 0.0f) return;

	const float t = (elapsed_ += delta_time) / duration_;
	if (t >= 1.0f) this->stop();
}

// エフェクトの再生
EffectPlayer& EffectPlayer::play(GSuint effect, float duration) noexcept {
	if (elapsed_ || duration_ > 0.0f) this->stop();
	elapsed_ = 0.0f;
	duration_ = duration;
	handle_ = gsPlayEffect(effect, nullptr);
	return *this;
}

// エフェクトの停止
void EffectPlayer::stop(void) noexcept {
	gsStopEffect(handle_);
	elapsed_ = duration_ = 0.0f;
	handle_ = 0;
}

// 位置の設定
EffectPlayer& EffectPlayer::position(const GSvector3& position) noexcept {
	gsSetEffectPosition(handle_, &position);
	return *this;
}

// 回転の設定
EffectPlayer& EffectPlayer::rotation(const GSvector3& rotation) noexcept {
	gsSetEffectRotationRollPitchYaw(handle_, rotation.x, rotation.y, rotation.z);
	return *this;
}

// 拡大縮小の設定
EffectPlayer& EffectPlayer::scale(const GSvector3& scale) noexcept {
	gsSetEffectScale(handle_, &scale);
	return *this;
}

// 色の設定
EffectPlayer& EffectPlayer::color(const GScolor& color) noexcept {
	gsSetEffectColor(handle_, &color);
	return *this;
}