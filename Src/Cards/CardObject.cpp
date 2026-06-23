#include "Cards/CardObject.h"

#include "Systems/Mouse.h"
#include "Defines/Assets.h"
#include "Defines/Define.h"
#include "Tiles/TileManager.h"

/*定義*/
// カードサイズ
#define DEFAULT_CARD_SIZE GSvector2{ 180.0f, 256.0f }
// 選択中カードオフセット
#define FOCUSED_CARD_OFFSET GSvector2{ 0.0f, -50.0f }
// カードアイコン画像オフセット
#define CARD_ICON_OFFSET GSvector2{ 0.0f, -32.0f }

/*定数*/
// カード背景画像ピボット
static const GSvector2 CARD_BACK_PIVOT{ 512.0f * 0.5f, 512.0f * 0.5f };
// カードアイコン画像ピボット
static const GSvector2 CARD_ICON_PIVOT{ 256.0f * 0.5f, 256.0f * 0.5f };
// イベントアイコン画像スケール
static const GSvector2 EVENT_ICON_SCALE{ 0.65f, 0.65f };
// ウィンドウ中心位置
static const GSvector2 WINDOW_CENTER{
	static_cast<float>(INITIAL_WINDOW_WIDTH / 2),
	static_cast<float>(INITIAL_WINDOW_HEIGHT / 2)
};
// カーソル最小オフセット距離
static constexpr float CURSOR_MIN_DISTANCE{ 50.0f };
// カーソル最大オフセット距離
static constexpr float CURSOR_MAX_DISTANCE{ 150.0f };

/*実装*/
// コンストラクタ
CardObject::CardObject(GSvector2 position, TileData& data) :
	size_{ DEFAULT_CARD_SIZE }, position_{ position }, data_{ std::move(data) } {
}

// 更新
void CardObject::update(float delta_time) {

}

// GUI描画
void CardObject::draw_gui() const {
	// 背景画像の描画;
	const GSvector2 back_pos = position_ + ((is_focusing_) ? FOCUSED_CARD_OFFSET : GSvector2::zero());
	gsDrawSprite2D(Texture::CardBackGround, &back_pos, NULL, &CARD_BACK_PIVOT, NULL, NULL, 0.0f);

	// アイコン画像位置
	const GSvector2 icon_pos = back_pos + CARD_ICON_OFFSET;

	// 地面画像の描画
	const GSuint ground_texture = (!data_.event) ? Texture::PavedCardBack : Texture::GlassCardBack;
	gsDrawSprite2D(ground_texture, &icon_pos, NULL, &CARD_ICON_PIVOT, NULL, NULL, 0.0f);

	// 道画像の描画
	const GSuint road_texture = shape_to_sprite(data_.road);
	const float road_angle = static_cast<float>(-data_.rotation);
	gsDrawSprite2D(road_texture, &icon_pos, NULL, &CARD_ICON_PIVOT, NULL, NULL, road_angle);

	// イベントアイコン画像の描画
	if (!data_.event || data_.event->type() <= EventType::MidBoss) return;
	const GSuint icon_texture = data_.event->icon();
	gsDrawSprite2D(icon_texture, &icon_pos, NULL, &CARD_ICON_PIVOT, NULL, &EVENT_ICON_SCALE, 0.0f);
}

// タイルデータの取得
TileData& CardObject::tile_data() noexcept {
	return data_;
}

// ドラッグ開始処理
void CardObject::on_drag_begin(GSvector2 position) {
	is_focusing_ = true;
}

// ドラッグ中処理
void CardObject::on_dragging(GSvector2 position) {
	const GSvector2 offset = position - WINDOW_CENTER;
	const GSvector2 direction = offset.normalized();
	const float distance = offset.magnitude();
	const float clamped = std::clamp(distance, CURSOR_MIN_DISTANCE, CURSOR_MAX_DISTANCE);
	position_ = GSvector2::moveTowards(position_, position + direction * clamped, 100.0f);
}

// ドラッグ中処理
void CardObject::on_drag_end(GSvector2 position) {
	is_focusing_ = false;
}

// 位置の補間
void CardObject::move_to_wards(float delta_time, GSvector2 position) noexcept {
	constexpr float CARD_MIN_SPEED{ 60.0f };
	constexpr float CARD_MAX_SPEED{ 400.0f };
	const float speed = (position - position_).lengthSq() * 0.001f;
	const float clamped = std::clamp(speed, CARD_MIN_SPEED, CARD_MAX_SPEED);
	position_ = GSvector2::moveTowards(position_, position, clamped * delta_time);
}

// 選択中フラグ
bool CardObject::is_hovering() const noexcept {
	const GSvector2 point = Mouse::position();
	if (point.x < position_.x - size_.x * 0.5f) return false;
	if (point.x > position_.x + size_.x * 0.5f) return false;
	if (point.y < position_.y - size_.y * 0.5f) return false;
	if (point.y > position_.y + size_.y * 0.5f) return false;
	return true;
}

// フォーカスの設定
void CardObject::set_focus(bool is_focused) noexcept {
	is_focusing_ = is_focused;
}

// スプライトIDの変換
constexpr GSuint CardObject::shape_to_sprite(RoadShape shape) {
	switch (shape) {
	case RoadShape::Straight: return Texture::CardStraightRoad;
	case RoadShape::Corner: return Texture::CardCornerRoad;
	case RoadShape::ThreeWay: return Texture::CardThreeWayRoad;
	case RoadShape::CrossRoads: return Texture::CardCrossRoadsRoad;
	}
	return Texture::CardStraightRoad;
}