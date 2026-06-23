#ifndef TILES_TILE_NODE_H_
#define TILES_TILE_NODE_H_

#include <memory>
#include <optional>

#include <GStype.h>

#include "Tiles/TileData.h"
#include "Interfaces/ITileEvent.h"

/*定数*/
// 回転角度幅
inline constexpr int ROTATION_STEP{ 90 };
// 進行方向数
inline constexpr int DIRECTION_COUNT{ 4 };
// 進行方向最大値
inline constexpr unsigned char DIRECTION_MAX{ 0b1111 };

// 左方向ビット
inline constexpr unsigned char LEFT_BIT{ 1 << 0 };
// 前方向ビット
inline constexpr unsigned char FORWARD_BIT{ 1 << 1 };
// 右方向ビット
inline constexpr unsigned char RIGHT_BIT{ 1 << 2 };
// 後ろ方向ビット
inline constexpr unsigned char BACK_BIT{ 1 << 3 };

// 反対方向ビットオフセット
inline constexpr unsigned char OPPOSITE_BIT_OFFSET{ 2 };

class TileNode {
public:
	TileNode() = default;
	TileNode(int x, int z, TileData& data);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// グラフx座標の取得
	int x(void) const noexcept;
	// グラフz座標の取得
	int z(void) const noexcept;
	// ワールド座標の取得
	GSvector3 world_position(void) const noexcept;
	// ワールド座標の設定
	void world_position(const GSvector3& world_position) noexcept;

	// タイル種別の取得
	TileType type(void) const noexcept;
	// 道形状の取得
	RoadShape road(void) const noexcept;
	// 回転角度の取得
	int rotation(void) const noexcept;

	// 進行方向の取得
	unsigned char directions(void) const noexcept;

	// 重みの取得
	float weight(void) const noexcept;
	// イベントの取得
	ITileEvent* event(void) noexcept;

	// イベントの取得
	ITileEvent* event(void) const noexcept;

	// 歩行可能フラグ
	bool is_walkable(void) const noexcept;
	// 通行可能フラグ
	bool is_passable(unsigned char mask) const noexcept;

	// 進行方向ビットの計算
	static constexpr unsigned char calculate_bit(RoadShape road, int rotation) noexcept;

private:
	// グラフx座標
	int graph_x_{ 0 };
	// グラフz座標
	int graph_z_{ 0 };
	// ワールド座標
	GSvector3 world_position_{ 0.0f, 0.0f, 0.0f };

	// 回転
	int rotation_{ 0 };
	// タイル種別
	TileType type_{ TileType::Abyss };
	// 道形状
	RoadShape road_{ RoadShape::Null };
	// イベント
	std::unique_ptr<ITileEvent> event_{ nullptr };
};

#endif // TILES_TILE_NODE_H_