#include "Cameras/Cinemachine.h"

#include <gslib.h>
#include <imgui/imgui.h>

/*実装*/
// 更新
void Cinemachine::update(float delta_time) {
	GSmatrix4 matrix = transform_.localToWorldMatrix();

	if (current_) {
		current_->update(delta_time);
		matrix = current_->transform().localToWorldMatrix();
	}

	if (to_) {
		to_->update(delta_time);

		float t{ 1.0f };
		if (duration_ != 0.0f) t = (elapsed_ += delta_time) / duration_;

		const GSvector3 pos =
			matrix.position().lerp(to_->transform().position(), std::clamp(t, 0.0f, 1.0f));
		const GSquaternion rot =
			matrix.rotation().lerp(to_->transform().rotation(), std::clamp(t, 0.0f, 1.0f));

		matrix = GSmatrix4::TRS(pos, rot, GSvector3::one());

		const GSvector3 effect = pos + matrix.forward();
		gsSetEffectPosition(effect_, &effect);

		if (t >= 1.0f) {
			elapsed_ = duration_ = 0.0f;
			current_ = std::move(to_);
			gsStopEffect(effect_);
			effect_ = 0;
		}
	}

	transform_.rotation(matrix.rotation());
	transform_.position(matrix.position());
}

// 描画
void Cinemachine::draw() const {
	// 視野変換行列の設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	const GSvector3 pos = transform_.position();
	const GSvector3 at = pos + transform_.forward();
	const GSvector3 up = transform_.up();
	gluLookAt(
		pos.x, pos.y, pos.z,
		at.x, at.y, at.z,
		up.x, up.y, up.z
	);
}

// エフェクトの再生
void Cinemachine::play_effet() {
	const GScolor color{ 1.0f, 1.0f, 1.0f, 0.35f };
	effect_ = gsPlayEffect(Effect::CameraTransition, NULL);
	gsSetEffectColor(effect_, &color);
}