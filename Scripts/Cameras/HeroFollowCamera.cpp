#include "Cameras/HeroFollowCamera.h"

#include "Characters/CharacterBase.h"

/*定数*/
// 勇者オフセット
static const GSvector3 HERO_OFFSET{ 7.0f, 4.0f, -2.0f };
// 勇者高さ
static constexpr float HERO_HEIGHT{ 1.5f };

/*実装*/
// コンストラクタ
HeroFollowCamera::HeroFollowCamera(const CharacterBase& hero) :
	hero_{ &hero } {
	const GSvector3 position = hero.transform().position();
	transform_.position(position + HERO_OFFSET);
	transform_.lookAt(position + GSvector3::up() * HERO_HEIGHT);
}

// 更新
void HeroFollowCamera::update(float delta_time) {
	transform_.position(hero_->transform().position() + HERO_OFFSET);
}