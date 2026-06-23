#ifndef CHARACTERS_HEROES_HERO_THINK_STATE_H_
#define CHARACTERS_HEROES_HERO_THINK_STATE_H_

#include "Interfaces/IState.h"
#include "Tiles/TileManager.h"
#include "Behaviors/BehaviorTree.h"
#include "Characters/Heroes/HeroDefine.h"

// 勇者思考状態クラス
class HeroThinkState :
	public IHeroState {
public:
	HeroThinkState();

	// 更新時処理
	void on_update(float delta_time, HeroContext& owner) override;

	// 遷移時処理
	void on_enter(HeroContext& owner) override;
	// 終了時処理
	void on_exit(HeroContext& owner) override;

private:
	// 魔王タイル決定
	NodeStatus aim_for_demon_king(void);
	// 強敵タイル決定
	NodeStatus aim_for_mid_boss(void);
	// 要回復フラグ
	bool is_should_recover(void) const noexcept;
	// 宿屋の検索
	NodeStatus find_inn(void);
	// ランダムタイル決定
	void decide_random_time(void);

private:
	// ビヘイビアツリー
	BehaviorTree behavior_;
	// コンテキストキャッシュ
	HeroContext* context_{ nullptr };

	// 思考時間
	float think_timer_{ 0.0f };
	// 目的地タイル
	TileNode* destination_{ nullptr };

	// 魔王判定済みフラグ
	bool is_demon_king_checked_{ false };
	// 強敵判定済みフラグ
	bool is_mid_boss_checked_{ false };

	// 強敵距離インデックス
	size_t mid_boss_index_{ 0 };
	// 強敵タイルキャッシュ
	std::vector<TileNode*> mid_bosses_;

	// 宿屋距離インデックス
	size_t inn_index_{ 0 };
	// 宿屋タイルキャッシュ
	std::vector<TileNode*> inns_;
};

#endif // CHARACTERS_HEROES_HERO_THINK_STATE_H_