#ifndef TILES_TILE_MANAGER_H_
#define TILES_TILE_MANAGER_H_

#include <optional>

#include "Tiles/TileGraph.h"
#include "Tiles/TileRenderer.h"

class EventPool;
class MonsterPool;
class MidBossPool;
struct EventContext;

// タイルマネージャークラス
class TileManager {
public:
	TileManager() = default;
	explicit TileManager(const std::string& file_name);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// マップの読み込み
	void load_map(const std::string& file_name,
		EventPool& event_pool, MonsterPool& monster_pool, MidBossPool& mid_boss_pool);

	// タイル置換の試行
	bool try_replace_tile(int x, int z, TileData& data);

	// グラフの取得
	TileGraph& graph(void) noexcept;

	// 座標の変換
	std::pair<int, int> cursor_to_coord(void) const noexcept;
	// 座標のクランプ
	std::pair<int, int> coord_clamp(int x, int z) const noexcept;

	// メッシュIDの変換
	std::optional<GSuint> tile_to_mesh(const TileNode& tile) const noexcept;
	// ワールド座標の計算
	GSvector3 calculate_world_position(int x, int z) const noexcept;
	// ワールド変換の計算
	GSmatrix4 calculate_world_matrix(const TileNode& tile) const noexcept;

	// データのパース
	static TileData parse_data(const std::string& cell, EventPool& event_pool, EventContext& context);

	// 端タイルの作成
	void make_tip_tiles(DemonKingCharacter& demon_king) noexcept;

private:
	// グラフ
	TileGraph graph_;
	// レンダラー
	TileRenderer renderer_;
	// ワールド座標変換行列
	GSmatrix4 world_matrix_{ GS_MATRIX4_IDENTITY };
};

#include "Inlines/TileManager.inl"

#endif // TILES_TILE_MANAGER_H_