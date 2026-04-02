#include "Tiles/TileManager.h"

#include <fstream>
#include <sstream>

#include <gslib.h>
#include <GSmathf.h>
#include <imgui/imgui.h>

#include "Systems/Mouse.h"
#include "Events/EventPool.h"
#include "Events/EvecntContext.h"
#include "Characters/Monsters/MonsterPool.h"

/*定義*/
// タイル中心座標オフセット
#define TILE_CENTER_OFFSET GSvector3{ TILE_SIZE * 0.5f, 0.0f, TILE_SIZE * 0.5f }
// 魔王出現位置オフセット
#define DEMON_KING_OFFSET GSvector3::right() * (TILE_SIZE / 3.0f)

/*実装*/
// コンストラクタ
TileManager::TileManager(const std::string& file_name) {
	//// マップの読み込み
	//this->load_map(file_name);
}

// 更新
void TileManager::update(float delta_time) {
	for (const auto& tiles : graph_.tiles()) {
		for (const auto& tile : tiles) {
			if (tile) tile->update(delta_time);
		}
	}
}

// 描画
void TileManager::draw() const {
	// タイルの描画
	renderer_.draw();
	for (const auto& tiles : graph_.tiles()) {
		for (const auto& tile : tiles) {
			if (tile) tile->draw();
		}
	}
}

// マップの読み込み
void TileManager::load_map(const std::string& file_name,
	EventPool& event_pool, MonsterPool& monster_pool, MidBossPool& mid_boss_pool) {
	std::ifstream file{ file_name };
	// TODO: エラーメッセージの記述
	if (!file.is_open()) throw std::runtime_error{ "" };

	std::string line{ "" };
	std::vector<std::vector<TileData>> map_data;
	EventContext context{
		.monster_pool_ = &monster_pool,
		.mid_boss_pool_ = &mid_boss_pool
	};

	while (std::getline(file, line)) {
		std::string cell{ "" };
		std::stringstream ss{ line };

		// 空のstd::vectorを追加
		map_data.push_back({});
		while (std::getline(ss, cell, ',')) {
			// パースしたデータを末尾のstd::vectorに追加
			map_data.back().push_back(parse_data(cell, event_pool, context));
		}
	}

	// グラフの構築
	graph_.build(map_data);

	// ワールド変換行列の計算
	const GSmatrix4 scale = GSmatrix4::scale({ -TILE_SIZE, 1.0f, TILE_SIZE });
	const GSmatrix4 translate = GSmatrix4::translate({ 0.0f, 0.0f, TILE_SIZE * graph_.center() });
	world_matrix_ = scale * translate;

	// メッシュインスタンスの追加
	for (const auto& tiles : graph_.tiles()) {
		for (const auto& tile : tiles) {
			if (!tile) continue;
			// ワールド変換行列の設定
			const GSmatrix4 matrix = calculate_world_matrix(*tile);
			tile->world_position(matrix.position());

			// タイルにメッシュが必要なら、レンダラーでインスタンス化
			const auto mesh = tile_to_mesh(*tile);
			if (!mesh.has_value()) continue;
			renderer_.add_instance(mesh.value(), matrix);
		}
	}

	const int depth = graph_.depth() - 1;
	const int width = map_data.front().size() - 1;
	constexpr size_t corner_count{ 4 };
	static const GSvector3 masks[corner_count] = {
		{ 0, 0, 0 }, { 1, 0, 0 }, { 1, 0, 1 }, { 0, 0, 1 },
	};
	static const GSvector3 offsets[corner_count] = {
		{ 0.5f, 0.0f, 0.5f }, { -0.5f, 0.0f, 0.5f }, { -0.5f, 0.0f, -0.5f }, { 0.5f, 0.0f, -0.5f },
	};

	// 設置可能エリアを示すインスタンスメッシュを四つ角に追加
	GSquaternion rot = GSquaternion::identity();
	static const GSvector3 offset = GSvector3{ 1.2f, 1.0f, 1.2f };
	static const GSquaternion yaw = GSquaternion::angleAxis(-90.0f, GSvector3::up());
	const GSvector3 max{ static_cast<float>(width), 0.0f, -static_cast<float>(depth) };
	for (int i = 0; i < corner_count; ++i) {
		const GSvector3 pos = max * masks[i] + GSvector3::left();
		const GSmatrix4 mat = GSmatrix4::TRS(pos * world_matrix_ + offsets[i], rot, GSvector3::one());
		renderer_.add_instance(Mesh::AreaCorner, mat);
		rot *= yaw;
	}

	// ストレージバッファの作成
	renderer_.create_buffers(graph_.size());
}

// タイル置換の試行
bool TileManager::try_replace_tile(int x, int z, TileData& data) {
	// タイルが置換できなければ失敗を返す
	if (!graph_.try_replace_tile(x, z, data)) return false;

	// 置換後のタイルの取得
	const auto& tile = graph_.tiles()[z][x];

	// ワールド変換行列の再設定
	const GSmatrix4 matrix = calculate_world_matrix(*tile);
	tile->world_position(matrix.position());

	// タイルにメッシュが必要なら、
	const auto mesh = tile_to_mesh(*tile);
	if (mesh.has_value()) {
		// レンダラーでインスタンス化して更新
		renderer_.add_instance(mesh.value(), matrix);
		renderer_.update_instances(mesh.value());
	}

	// 成功
	return true;
}

// 座標の変換
std::pair<int, int> TileManager::cursor_to_coord() const noexcept {
	// マウスからのレイの取得
	const GSvector3 origin = Mouse::ray().origin;
	const GSvector3 direction = Mouse::ray().direction;

	// y = 0のxz無限平面に対する交点の計算
	// NOTE: カード設置中は常にカメラが下向きなため、交差しないことは考慮しない
	const float t = -origin.y / direction.y;
	const GSvector3 intersection{
		origin.x + (direction.x * t),
		origin.y + (direction.y * t),
		origin.z + (direction.z * t)
	};

	// 交点をもとにタイル座標系へ変換
	const GSvector3 translated = intersection - TILE_CENTER_OFFSET;
	const GSvector3 inversed = translated * world_matrix_.getInverse();

	// タイル座標を整数にキャスト
	const int x = static_cast<int>(std::floor(inversed.x));
	const int z = static_cast<int>(std::floor(-inversed.z));

	return std::pair{ x, z };
}

// 座標のクランプ
std::pair<int, int> TileManager::coord_clamp(int x, int z) const noexcept {
	const int depth = graph_.depth();
	const int width = graph_.width() - GOAL_TILE_OFFSET;
	const int clamped_x = std::clamp(x, START_TILE_OFFSET, width - 1);
	const int clamped_z = std::clamp(z, 0, depth - 1);
	return std::make_pair(clamped_x, clamped_z);
}

// データのパース
TileData TileManager::parse_data(const std::string& cell, EventPool& event_pool, EventContext& context) {
	TileData data;
	// セルが空ならデフォルトデータ（奈落タイル）を返す
	if (cell.empty()) return data;

	enum Element : size_t {
		Type, Road, Rotation,
	};

	size_t count{ 0 };
	std::string element{ "" };
	std::stringstream ss{ cell };

	while (std::getline(ss, element, ':')) {
		switch (count++) {
			case Element::Type:
				data.type = static_cast<TileType>(std::stoi(element));
				data.event = event_pool.create(data.type, context);
				break;

			case Element::Road:
				data.road = static_cast<RoadShape>(std::toupper(element.front()));
				break;

			case Element::Rotation:
				data.rotation = std::stoi(element);
				break;
		}
	}

	return data;
}

// 端タイルの作成
void TileManager::make_tip_tiles(DemonKingCharacter& demon_king) noexcept {
	graph_.make_tip_tiles(demon_king);

	// スタートのタイル座標を設定
	if (TileNode* start_tile = graph_.start_tile()) {
		const GSmatrix4 start_matrix = calculate_world_matrix(*start_tile);
		start_tile->world_position(start_matrix.position());

		// スタートのメッシュを設定
		auto start_mesh = tile_to_mesh(*start_tile);
		if (start_mesh.has_value()) {
			// レンダラーでインスタンス化して更新
			renderer_.add_instance(start_mesh.value(), start_matrix);
			renderer_.update_instances(start_mesh.value());
		}
	}

	// ゴールのタイル座標を設定
	if (TileNode* goal_tile = graph_.goal_tile()) {
		const GSmatrix4 goal_matrix = calculate_world_matrix(*goal_tile);
		goal_tile->world_position(goal_matrix.position());

		// 魔王トランスフォームの設定
		const GSvector3 demon_king_pos = goal_matrix.position() + DEMON_KING_OFFSET;
		demon_king.transform().position(demon_king_pos);
		demon_king.transform().lookAt(GSvector3::left());

		// ゴールのメッシュを設定
		auto goal_mesh = tile_to_mesh(*goal_tile);
		if (goal_mesh.has_value()) {
			// レンダラーでインスタンス化して更新
			renderer_.add_instance(goal_mesh.value(), goal_matrix);
			renderer_.update_instances(goal_mesh.value());
		}
	}
}