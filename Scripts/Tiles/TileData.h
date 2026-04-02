#ifndef TILES_TILE_DATA_H_
#define TILES_TILE_DATA_H_

#include <memory>

#include "Interfaces/ITileEvent.h"

/*定数*/
inline constexpr float TILE_SIZE{ 3.0f };

// タイル種別
enum class TileType {
	// 壁タイル
	Wall = -1,
	// 奈落タイル
	Abyss = 0,
	// 道タイル
	Road = 1,
	// イベントタイル
	Event = 2,
	// 強敵タイル
	MidBoss = 3,
};

// 道形状
enum class RoadShape {
	Null = 'O',

	// 一本道形状
	Straight = 'I',
	// 曲がり角形状
	Corner = 'L',
	// 三叉路形状
	ThreeWay = 'T',
	// 十字路形状
	CrossRoads = 'X',
};

// タイルデータ
struct TileData {
	TileType type{ TileType::Abyss };
	RoadShape road{ RoadShape::Null };
	int rotation{ 0 };
	ITileEvent* event{ nullptr };
};

#endif // TILES_TILE_DATA_H_