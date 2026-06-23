#include "Animations/AcquisitionAnimation.h"

// 移動量
static const GSvector3 MOVE_AMOUNT{ 0.0f, 0.01f, 0.0f };

// コンストラクタ
AcquisitionAnimation::AcquisitionAnimation() {

}
// デストラクタ
AcquisitionAnimation::~AcquisitionAnimation() {
	clear();
}

// 更新
void AcquisitionAnimation::update() {
	if (draw_animations_.empty()) return;

	for (auto& anima : draw_animations_) {
		// 透明にする
		anima->color.a -= 0.005f;
		if (anima->color.a <= 0) {
			continue;
		}

		// 移動
		anima->position += MOVE_AMOUNT;
	}
}

// 描画
void AcquisitionAnimation::draw() const{
	if (draw_animations_.empty()) return;

	//OpenGLの設定を退避
	glPushAttrib(GL_LIGHTING_BIT);
	//ライティング無効
	glDisable(GL_LIGHTING);

	// 描画
	for (auto& anima : draw_animations_) {
		if (anima->color.a <= 0) {
			continue;
		}
		gsDrawSprite3D(anima->texture_id, &anima->position, &anima->rect, NULL, &anima->color, &anima->scale, 0);
	}

	//OpenGLの各種設定を復帰
	glPopAttrib();
}

// 追加
void AcquisitionAnimation::add(GSuint texture_id, GSvector2 ratio, GSvector3 position, float scale) {

	const float x = ratio.x * 0.5f;
	const float y = ratio.y * 0.5f;
	const GSrect animation_rect = { -x, y, x, -y };

	const GSvector2 animation_scale = { scale, scale };
	// 登録
	draw_animations_.push_back(new GetAnimationContext{ texture_id, position, animation_rect, animation_scale });
}

// 削除
void AcquisitionAnimation::clear() {
	for (auto& anima : draw_animations_) {
		delete anima;
	}
	draw_animations_.clear();
}
