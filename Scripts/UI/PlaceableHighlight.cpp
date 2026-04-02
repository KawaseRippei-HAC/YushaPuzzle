#include "UI/PlaceableHighlight.h"

#include <gslib.h>

#include "Defines/Assets.h"
#include "Tiles/TileManager.h"

/*実装*/
// コンストラクタ
PlaceableHighlight::PlaceableHighlight(TileManager& tile_manager) :
	tile_manager_{ &tile_manager } {
}

// 更新
void PlaceableHighlight::update(float delta_time) {
	position_ = GSvector3::moveTowards(position_, destination_, delta_time);
}

// 描画
void PlaceableHighlight::draw() const {
	if (!is_visible_) return;
	glPushMatrix();
	{
		glTranslatef(position_.x, position_.y, position_.z);
		gsDrawMesh((is_placeable_) ? Mesh::PlaceableHighlight : Mesh::NonPlaceableHighlight);
	}
	glPopMatrix();
}

// 設置可能の判定
void PlaceableHighlight::judge_placeable(TileData& data) {
	// カーソルが前フレームに指していた座標と同じならリターン
	const auto pair = tile_manager_->cursor_to_coord();
	const auto [x, z] = tile_manager_->coord_clamp(pair.first, pair.second);
	if (x == prev_x_ && z == prev_z_) return;

	// 座標と設置可能フラグを更新
	prev_x_ = x; prev_z_ = z;
	destination_ = tile_manager_->calculate_world_position(x, z);
	TileGraph& graph = tile_manager_->graph();
	const TileType type = graph.tiles()[z][x]->type();
	is_placeable_ = (type == TileType::Abyss) ? graph.is_placeable(x, z, data) : false;
}

// 表示可能の設定
void PlaceableHighlight::visible(bool is_visible) {
	if (is_visible_ == is_visible) return;
	if (is_visible_ = is_visible) {
		const auto pair = tile_manager_->cursor_to_coord();
		const auto [x, z] = tile_manager_->coord_clamp(pair.first, pair.second);
		position_ = tile_manager_->calculate_world_position(x, z);
	}
}

// ハイライト中座標の取得
std::pair<int, int> PlaceableHighlight::highlighted_coord() const noexcept {
	return std::make_pair(prev_x_, prev_z_);
}