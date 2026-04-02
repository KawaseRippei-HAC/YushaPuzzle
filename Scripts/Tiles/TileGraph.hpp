#ifndef TILES_TILE_GRAPH_HPP_
#define TILES_TILE_GRAPH_HPP_

#include "Tiles/TileGraph.h"

/*定数*/
// 隣接x座標オフセット
static constexpr int NEIGHBOR_X_OFFSETS[] = { 0, 1, 0, -1 };
// 隣接z座標オフセット
static constexpr int NEIGHBOR_Z_OFFSETS[] = { -1, 0, 1, 0 };

/*テンプレート関数実装*/
// タイルの検索
template<typename TCompare>
	requires std::is_invocable_r_v<bool, TCompare, const TileNode&>
std::vector<TileNode*> TileGraph::find_tiles(TCompare&& compare_func) {
	std::vector<TileNode*> nears;
	for (auto& tiles : tiles_) {
		for (auto& tile : tiles) if (tile && compare_func(*tile)) nears.push_back(tile.get());
	}
	return nears;
}

// 隣接タイルの走査
template<typename TFunc>
	requires std::is_invocable_v<TFunc, size_t, int, int> || std::is_invocable_v<TFunc, int, int>
void TileGraph::for_each_neighbor(int x, int z, TFunc&& func) {
	// 隣接座標を走査
	for (size_t i = 0; i < DIRECTION_COUNT; ++i) {
		const int nx = x + NEIGHBOR_X_OFFSETS[i];
		const int nz = z + NEIGHBOR_Z_OFFSETS[i];
		// 関数オブジェクトを実行
		if constexpr (std::is_invocable_v<TFunc, size_t, int, int>) func(i, nx, nz);
		else if constexpr (std::is_invocable_v<TFunc, int, int>) func(nx, nz);
	}
}

#endif // TILES_TILE_GRAPH_HPP_