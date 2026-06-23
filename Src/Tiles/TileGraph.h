#ifndef TILES_TILE_GRAPH_H_
#define TILES_TILE_GRAPH_H_

#include <stack>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>

#include "Tiles/TileNode.h"
#include "Characters/Monsters/DemonKingCharacter.h"

/*定数*/
// 開始地点タイルオフセット
inline constexpr int START_TILE_OFFSET{ 1 };
// 到達地点タイルオフセット
inline constexpr int GOAL_TILE_OFFSET{ 1 };

// タイルグラフクラス
class TileGraph {
public:
	// グラフの構築
	void build(std::vector<std::vector<TileData>>& map_data);

	// タイルの検索
	template<typename TCompare>
		requires std::is_invocable_r_v<bool, TCompare, const TileNode&>
	std::vector<TileNode*> find_tiles(TCompare&& compare_func);

	// パスの検索
	std::stack<TileNode*> find_path(const TileNode& start, const TileNode& goal);

	// タイル置換の試行
	bool try_replace_tile(int x, int z, TileData& data);
	// 設置可能フラグ
	bool is_placeable(int x, int z, const TileData& data);

	// 幅の取得
	int width(void) const noexcept;
	// 深度の取得
	int depth(void) const noexcept;
	// 中心の取得
	int center(void) const noexcept;
	// サイズの取得
	size_t size(void) const noexcept;

	// 開始地点タイルの取得
	TileNode* start_tile(void);
	// 到達地点タイルの取得
	TileNode* goal_tile(void);

	// タイル配列の取得
	const std::vector<std::vector<std::unique_ptr<TileNode>>>& tiles(void) const noexcept;

	// 端タイルの作成
	void make_tip_tiles(DemonKingCharacter& demon_king) noexcept;

	// ヒューリスティック関数
	static float heuristic(const TileNode& lhs, const TileNode& rhs) noexcept;

private:
	// 開始地点タイルの作成
	static std::unique_ptr<TileNode> make_start_tile(int x, int z) noexcept;
	// 到達地点タイルの作成
	static std::unique_ptr<TileNode> make_goal_tile(int x, int z, DemonKingCharacter& demon_king) noexcept;

	// 隣接タイルの走査
	template<typename TFunc>
		requires std::is_invocable_v<TFunc, size_t, int, int> || std::is_invocable_v<TFunc, int, int>
	void for_each_neighbor(int x, int z, TFunc&& func);

	// 実コストマップの作成
	std::unordered_map<const TileNode*, float> make_g_cost_map(const TileNode& start) const;
	// パスの復元
	static std::stack<TileNode*> restoration_path(
		const TileNode& start, const TileNode& goal,
		const std::unordered_map<const TileNode*, const TileNode*>& parent_map);

private:
	// グラフ幅
	int width_{ 0 };
	// グラフ深度
	int depth_{ 0 };

	// 開始地点タイル
	TileNode* start_tile_{ nullptr };
	// 到達地点タイル
	TileNode* goal_tile_{ nullptr };
	// タイル配列
	std::vector<std::vector<std::unique_ptr<TileNode>>> tiles_;
};

#include "Tiles/TileGraph.hpp"
#include "Inlines/TileGraph.inl"

#endif // TILES_TILE_GRAPH_H_