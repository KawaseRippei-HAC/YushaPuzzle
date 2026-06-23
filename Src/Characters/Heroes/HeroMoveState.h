#ifndef CHARACTERS_HEROES_HERO_MOVE_STATE_H_
#define CHARACTERS_HEROES_HERO_MOVE_STATE_H_

#include <queue>

#include "Tiles/TileManager.h"
#include "Characters/Heroes/HeroDefine.h"

class ICharacterMover;

// 勇者移動状態クラス
class HeroMoveState :
	public IHeroState {
public:
	// 更新時処理
	void on_update(float delta_time, HeroContext& context) override;

	// 遷移時処理
	void on_enter(HeroContext& context) override;

private:
	// 移動の開始
	void move_start(HeroContext& context);
	// 移動の終了
	void move_end(HeroContext& context);

	// 接近の開始
	void start_approach(HeroContext& context);
	// 接近の終了
	void end_approach(HeroContext& context);

	// 離脱の開始
	void start_departure(HeroContext& context);
	// 離脱の終了
	void end_departure(HeroContext& context);

	// 移動処理の取得
	ICharacterMover* get_mover(TileNode* tile, bool is_path_empty, HeroCharacter& charcter) noexcept;

private:
	// 接近中フラグ
	bool is_in_approach_{ false };
	// イベント有効フラグ
	bool is_event_active_{ false };

	// 移動処理
	ICharacterMover* mover_{ nullptr };
	// 移動時処理関数ポインタ
	void(ICharacterMover::* on_move_)(float, CharacterBase&) { nullptr };

	// 目的地タイル
	TileNode* destination_tile_{ nullptr };
	// 経由座標
	GSvector3 transit_position_{ 0.0f, 0.0f, 0.0f };
};

#endif // CHARACTERS_HEROES_HERO_MOVE_STATE_H_