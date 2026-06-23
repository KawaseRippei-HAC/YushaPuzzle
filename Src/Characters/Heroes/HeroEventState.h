#ifndef CHARACTERS_HEROES_HERO_EVENT_STATE_H_
#define CHARACTERS_HEROES_HERO_EVENT_STATE_H_

#include "Characters/Heroes/HeroDefine.h"

class ITileEvent;

// 勇者イベント状態
class HeroEventState :
	public IHeroState {
public:
	// 更新
	void on_update(float delta_time, HeroContext& context) override;

	// 遷移時処理
	void on_enter(HeroContext& context) override;
	// 終了時処理
	void on_exit(HeroContext& context) override;

private:
	// 中心まで移動すべきか
	bool should_move_to_center(const HeroContext& context)const noexcept;

private:
	// イベント
	ITileEvent* event_{ nullptr };

	// 中心に到達しているか？
	bool is_centering_{ false };
};

#endif // CHARACTERS_HEROES_HERO_EVENT_STATE_H_