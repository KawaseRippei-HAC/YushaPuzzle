#ifndef CHARACTERS_HEROES_HERO_IDLE_STATE_H_
#define CHARACTERS_HEROES_HERO_IDLE_STATE_H_

#include "Characters/Heroes/HeroDefine.h"

// 勇者待機状態クラス
class HeroIdleState final :
	public IHeroState {
public:
	// 更新時処理
	void on_update(float delta_time, HeroContext& context) override;

	// 遷移時処理
	void on_enter(HeroContext& context) override;
};

#endif // CHARACTERS_HEROES_HERO_IDLE_STATE_H_