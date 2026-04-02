#ifndef INLINES_TILE_GRAPH_INL_
#define INLINES_TILE_GRAPH_INL_

#include "Tiles/TileGraph.h"

/*インライン関数実装*/
// 幅の取得
inline int TileGraph::width() const noexcept {
	return width_;
}

// 深度の取得
inline int TileGraph::depth() const noexcept {
	return depth_;
}

// 中心の取得
inline int TileGraph::center() const noexcept {
	return (depth_ - 1) / 2;
}

// サイズの取得
inline size_t TileGraph::size() const noexcept {
	return static_cast<size_t>(width_ * depth_);
}

// 開始地点タイルの取得
inline TileNode* TileGraph::start_tile() {
	return start_tile_;
}

// 到達地点タイルの取得
inline TileNode* TileGraph::goal_tile() {
	return goal_tile_;
}

// タイル配列の取得
inline const std::vector<std::vector<std::unique_ptr<TileNode>>>& TileGraph::tiles() const noexcept {
	return tiles_;
}

// ヒューリスティック関数
inline float TileGraph::heuristic(const TileNode& lhs, const TileNode& rhs) noexcept {
	const int dx = lhs.x() - rhs.x();
	const int dz = lhs.z() - rhs.z();
	return std::fabs(static_cast<float>(dx)) + std::fabs(static_cast<float>(dz));
}

// 開始地点タイルの作成
inline std::unique_ptr<TileNode> TileGraph::make_start_tile(int x, int z) noexcept {
	// 開始地点タイルデータ
	TileData data{
		TileType::Road,
		RoadShape::Straight,
		NULL, nullptr
	};

	// 開始地点タイルの生成
	return std::make_unique<TileNode>(x, z, data);
}

#endif // INLINES_TILE_GRAPH_INL_