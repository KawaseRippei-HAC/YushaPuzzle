#include "Tiles/TileNode.h"

/*定数*/
// 一本道ビット
static constexpr unsigned char STRAIGHT_BITS[] = {
	FORWARD_BIT | BACK_BIT,
	LEFT_BIT | RIGHT_BIT,
	BACK_BIT | FORWARD_BIT,
	RIGHT_BIT | LEFT_BIT
};
// 曲がり角ビット
static constexpr unsigned char CORNER_BITS[] = {
	LEFT_BIT | FORWARD_BIT,
	LEFT_BIT | BACK_BIT,
	BACK_BIT | RIGHT_BIT,
	RIGHT_BIT | FORWARD_BIT
};
// 三叉路ビット
static constexpr unsigned char THREEWAY_BITS[] = {
	LEFT_BIT | FORWARD_BIT | RIGHT_BIT,
	FORWARD_BIT | LEFT_BIT | BACK_BIT,
	LEFT_BIT | BACK_BIT | RIGHT_BIT,
	BACK_BIT | RIGHT_BIT | FORWARD_BIT
};
// 十字路ビット
static constexpr unsigned char CROSSROADS_BIT{
	LEFT_BIT | FORWARD_BIT | RIGHT_BIT | BACK_BIT
};

/*実装*/
// コンストラクタ
TileNode::TileNode(int x, int z, TileData& data) :
	graph_x_{ x }, graph_z_{ z },
	type_{ data.type }, road_{ data.road },
	rotation_{ data.rotation }, event_{ data.event } {
}

// 更新
void TileNode::update(float delta_time) {
	if (!event_) return;
	event_->update_object(delta_time);
}
// 描画
void TileNode::draw() const {
	if (!event_) return;
	const GSquaternion rotation{ static_cast<float>(rotation_), GSvector3::up() };
	const GSmatrix4 matirx = GSmatrix4::TRS(world_position(), rotation, GSvector3::one());
	event_->draw_object(matirx);
}

// グラフx座標の取得
int TileNode::x() const noexcept {
	return graph_x_;
}

// グラフz座標の取得
int TileNode::z() const noexcept {
	return graph_z_;
}

// ワールド座標の取得
GSvector3 TileNode::world_position() const noexcept {
	return world_position_;
}

// ワールド座標の設定
void TileNode::world_position(const GSvector3& world_position) noexcept {
	world_position_ = world_position;
}

// タイル種別の取得
TileType TileNode::type() const noexcept {
	return type_;
}

// 道形状の取得
RoadShape TileNode::road() const noexcept {
	return road_;
}

// 回転角度の取得
int TileNode::rotation() const noexcept {
	return rotation_;
}

// 進行方向の取得
unsigned char TileNode::directions() const noexcept {
	return calculate_bit(road_, rotation_);
}

// 重みの取得
float TileNode::weight() const noexcept {
	return (event_) ? event_->weight() : DEFAULT_EVENT_WEIGHT;
}

// イベントの取得
ITileEvent* TileNode::event() noexcept {
	return event_.get();
}

// イベントの取得
ITileEvent* TileNode::event() const noexcept {
	return event_.get();
}

// 歩行可能フラグ
bool TileNode::is_walkable() const noexcept {
	return type_ >= TileType::Road;
}

// 通行可能フラグ
bool TileNode::is_passable(unsigned char mask) const noexcept {
	if (type_ == TileType::Abyss) return true;

	const unsigned char dir = directions();

	if (!mask || !dir) return false;
	else if (mask & LEFT_BIT && dir & RIGHT_BIT) return true;
	else if (mask & FORWARD_BIT && dir & BACK_BIT) return true;
	else if (mask & RIGHT_BIT && dir & LEFT_BIT) return true;
	else if (mask & BACK_BIT && dir & FORWARD_BIT) return true;

	return false;
}

// 進行方向ビットの計算
constexpr unsigned char TileNode::calculate_bit(RoadShape road, int rotation) noexcept {
	const int index =
		(rotation / ROTATION_STEP) % DIRECTION_COUNT;

	switch (road) {
	case RoadShape::Straight: return STRAIGHT_BITS[index];
	case RoadShape::Corner: return CORNER_BITS[index];
	case RoadShape::ThreeWay: return THREEWAY_BITS[index];
	case RoadShape::CrossRoads: return CROSSROADS_BIT;
	}

	return NULL;
}