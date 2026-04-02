#include "Tiles/TileGraph.h"

#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>

//#include "Events/EncounterEvent.h"
#include "Events/FallInAbyssEvent.h"
#include "Events/DemonKingEncounterEvent.h"

/*実装*/
// グラフの構築
void TileGraph::build(std::vector<std::vector<TileData>>& map_data) {
	depth_ = map_data.size();
	width_ = map_data.front().size() + START_TILE_OFFSET + GOAL_TILE_OFFSET;

	// 深度方向にリサイズ
	tiles_.resize(depth_);
	for (int z = 0; z < depth_; ++z) {
		// 現在の深度のstd::vectorを幅方向にリサイズ
		tiles_[z].resize(width_);
		for (int x = START_TILE_OFFSET; x < width_ - GOAL_TILE_OFFSET; ++x) {
			TileData& data = map_data[z][x - GOAL_TILE_OFFSET];
			tiles_[z][x] = std::make_unique<TileNode>(x, z, data);
		}
	}
}

std::stack<TileNode*> TileGraph::find_path(const TileNode& start, const TileNode& goal) {
	struct record {
		float f_cost{ 0.0f };
		float g_cost{ 0.0f };
		const TileNode* tile{ nullptr };
		bool operator>(const record& other) const { return f_cost > other.f_cost; }
	};

	// 親タイルマップ
	std::unordered_map<const TileNode*, const TileNode*> parent_map;
	// 実スコアマップ
	std::unordered_map<const TileNode*, float> g_cost_map = make_g_cost_map(start);
	// オープンセット
	std::priority_queue<record, std::vector<record>, std::greater<record>> open_set;

	// 開始地点をプッシュ
	open_set.push({ heuristic(start, goal), 0, &start });

	while (!open_set.empty()) {
		record current = open_set.top();
		open_set.pop();

		// 現在のタイルが目的地のタイルだったら
		if (current.tile == &goal) {
			// パスを復元して返す
			return restoration_path(start, goal, parent_map);
		}

		// 現在のタイルが歩行不可なら以降を無視
		if (!current.tile->is_walkable()) continue;

		// 隣接座標を走査
		const unsigned char dir = current.tile->directions();
		for_each_neighbor(current.tile->x(), current.tile->z(),
			[&](size_t i, int nx, int nz) {
				// 隣接座標が無効な値だったら以降を無視
				if (nx < 0 || nz < 0 || nx >= width_ || nz >= depth_) return;

				// 現在のタイルの通行不可な方向なら以降を無視
				const unsigned char mask = dir & (1 << i);
				if (!mask) return;

				// 隣接タイルが通行不可なら以降を無視
				const TileNode* neighbor = tiles_[nz][nx].get();
				if (!neighbor || !neighbor->is_passable(mask)) return;

				// 現在のコストが隣接タイルのコスト以上なら以降を無視
				float tentative_g = current.g_cost + neighbor->weight();
				if (mask & FORWARD_BIT) tentative_g -= 1.0f;
				else if (mask & BACK_BIT) tentative_g += 1.0f;
				if (tentative_g >= g_cost_map[neighbor]) return;

				// コストを更新
				g_cost_map[neighbor] = tentative_g;

				// hコストを求め、オープンセットにプッシュ
				const float h = heuristic(*neighbor, goal);
				open_set.push({ tentative_g + h, tentative_g, neighbor });

				// 親タイルを設定
				parent_map[neighbor] = current.tile;
			}
		);
	}

	// パスなし
	return {};
}

// タイル置換の試行
bool TileGraph::try_replace_tile(int x, int z, TileData& data) {
	// 座標が無効な値なら失敗を返す
	if (x < 0 || z < 0 || x >= width_ || z >= depth_) return false;

	// 置換元タイルが無い、または奈落タイル以外なら失敗を返す
	auto& tile = tiles_[z][x];
	if (!tile || tile->type() != TileType::Abyss) return false;

	// 設置不可だったら失敗を返す
	if (!is_placeable(x, z, data)) return false;

	// 新しいタイルで上書き
	tile = std::make_unique<TileNode>(x, z, data);
	return true;
}

// 設置可能フラグ
bool TileGraph::is_placeable(int x, int z, const TileData& data) {
	int passable_count{ 0 };
	bool is_placeable{ true };

	// 新しいデータの進行方向ビットを取得
	const unsigned char directions = TileNode::calculate_bit(data.road, data.rotation);

	// 隣接タイルを走査
	for_each_neighbor(x, z,
		[&](size_t i, int nx, int nz) {
			if (!is_placeable) return;

			// 進行方向ビットマスク
			const unsigned char passable = directions & (1 << i);

			// 隣接座標が無効な値のとき、進行可能なら強制終了
			// 進行不可なら以降を無視
			const bool is_out_of_map = (nx < 0 || nz < 0 || nx >= width_ || nz >= depth_);
			if (is_out_of_map) { is_placeable = !passable; return; }

			// 隣接タイルが無いとき、進行可能なら強制終了
			// 進行不可なら以降を無視
			const auto& neighbor = tiles_[nz][nx];
			if (!neighbor) { is_placeable = !passable; return; }

			// 隣接タイルが奈落タイルなら以降を無視
			if (neighbor->type() == TileType::Abyss) return;

			// 隣接タイルが壁タイルのとき、進行可能なら強制終了
			// 進行不可なら以降を無視
			if (neighbor->type() == TileType::Wall) { is_placeable = !passable; return; }

			// 対面の道どちらかのみが進行可能なら強制終了
			const unsigned char opposite_mask = (1 << (i + OPPOSITE_BIT_OFFSET)) % DIRECTION_MAX;
			const bool is_accessible = neighbor->directions() & opposite_mask;
			if (!passable || !is_accessible) { is_placeable = !(passable || is_accessible); return; }

			// 通行可能な道の加算
			++passable_count;
		}
	);

	return passable_count && is_placeable;
}

// 実コストマップの作成
std::unordered_map<const TileNode*, float> TileGraph::make_g_cost_map(const TileNode& start) const {
	std::unordered_map<const TileNode*, float> g_costs;
	// 全コストをfloat最大値で初期化
	for (const auto& tiles : tiles_) {
		for (const auto& tile : tiles) {
			g_costs[tile.get()] = std::numeric_limits<float>::max();
		}
	}
	// 開始地点の実コストを0に設定
	g_costs[&start] = 0.0f;
	return g_costs;
};

// パスの復元
std::stack<TileNode*> TileGraph::restoration_path(
	const TileNode& start, const TileNode& goal,
	const std::unordered_map<const TileNode*, const TileNode*>& parent_map) {
	// 親タイルを開始地点まで遡る
	std::stack<TileNode*> path;
	TileNode* at{ const_cast<TileNode*>(&goal) };
	for (; at != &start; at = const_cast<TileNode*>(parent_map.at(at))) {
		path.push(at);
	}
	return path;
};

// 到達地点タイルの作成
std::unique_ptr<TileNode> TileGraph::make_goal_tile(int x, int z, DemonKingCharacter& demon_king) noexcept {
	TileData data{
		TileType::Event,
		RoadShape::Straight,
		// TODO: 魔王イベントを生成して渡す
		NULL, new DemonKingEncounterEvent{ demon_king }
	};

	// 到達地点タイルの生成
	return std::make_unique<TileNode>(x, z, data);
}

// 端タイルの作成
void TileGraph::make_tip_tiles(DemonKingCharacter& demon_king) noexcept {

	// 中心を取得
	const int center_z = center();

	// 開始地点タイルの生成
	constexpr int start_x{ 0 };
	tiles_[center_z][start_x] = make_start_tile(start_x, center_z);
	start_tile_ = tiles_[center_z][start_x].get();

	// 到達地点タイルの生成
	const int goal_x{ width_ - 1 };
	tiles_[center_z][goal_x] = make_goal_tile(goal_x, center_z, demon_king);
	goal_tile_ = tiles_[center_z][goal_x].get();
}