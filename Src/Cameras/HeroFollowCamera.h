#ifndef CAMERAS_HERO_FOLLOW_CAMERA_H_
#define CAMERAS_HERO_FOLLOW_CAMERA_H_

#include "Interfaces/IVirtualCamera.h"

class CharacterBase;

// 勇者追従カメラクラス
class HeroFollowCamera :
	public IVirtualCamera {
public:
	HeroFollowCamera(const CharacterBase& hero);

	// 更新
	void update(float delta_time) override;

private:
	// 勇者キャラクター
	const CharacterBase* hero_{ nullptr };
};

#endif // CAMERAS_HERO_FOLLOW_CAMERA_H_