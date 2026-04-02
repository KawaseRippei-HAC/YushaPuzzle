#ifndef INLINES_TILE_MANAGER_INL_
#define INLINES_TILE_MANAGER_INL_

#include "Tiles/TileManager.h"

#include "Defines/Assets.h"

/*インライン関数実装*/
// グラフの取得
inline TileGraph& TileManager::graph() noexcept {
	return graph_;
}

// メッシュIDの変換
inline std::optional<GSuint> TileManager::tile_to_mesh(const TileNode& tile) const noexcept {
	switch (tile.type()) {
	case TileType::Wall: return Mesh::WallTile;

	case TileType::Road:
	case TileType::Event:
	case TileType::MidBoss:
		switch (tile.road()) {
		case RoadShape::Straight: return Mesh::StraightTile;
		case RoadShape::Corner: return Mesh::CornerTile;
		case RoadShape::ThreeWay: return Mesh::ThreeWayTile;
		case RoadShape::CrossRoads: return Mesh::CrossRoadsTile;
		}
	}
	return std::nullopt;
}

// ワールド座標の計算
inline GSvector3 TileManager::calculate_world_position(int x, int z) const noexcept {
	const GSvector3 position{ static_cast<float>(x), 0.0f, -static_cast<float>(z) };
	return position * world_matrix_;
}

// ワールド変換行列の計算
inline GSmatrix4 TileManager::calculate_world_matrix(const TileNode& tile) const noexcept {
	const GSvector3 position{ calculate_world_position(tile.x(), tile.z()) };
	const GSquaternion rotation{ static_cast<float>(tile.rotation()), GSvector3::up() };
	return GSmatrix4::TRS(position, rotation, GSvector3::one());
}

#endif // INLINES_TILE_MANAGER_INL_