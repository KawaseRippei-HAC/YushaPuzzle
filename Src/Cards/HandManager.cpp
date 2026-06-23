#include "Cards/HandManager.h"

#include <bit>
#include <algorithm>

#include <gslib.h>

#include "Systems/Mouse.h"
#include "Cards/CardManager.h"
#include "Interfaces/ITileEvent.h"

/*定義*/
// ドロー開始位置
#define DRAW_BEGIN_POSITION GSvector2{ 960.0f, 1200.0f }
// フォーカス中手札オフセット
#define FOCUSING_HAND_OFFSET GSvector2{ 0.0f, -50.0f }

// 手札先頭位置
#define HAND_FIRST_POSITION GSvector2{ 1800.0f, 920.0f }
// 手札オフセット
#define HAND_OFFSET GSvector2{ -150.0f, 0.0f }

/*定数*/
static constexpr float HAND_BIND_SPEED{ 80.0f };

/*実装*/
// コンストラクタ
HandManager::HandManager() {
	const GSvector2 offset = HAND_OFFSET;
	const GSvector2 first = HAND_FIRST_POSITION;

	hands_.reserve(MAX_HAND_QUANTITY);
	for (size_t i = 0; i < MAX_HAND_QUANTITY; ++i) {
		positions_[i] = first + (offset * static_cast<float>(i));
	}

	Mouse::attach_drag_handler(GMOUSE_BUTTON_1, this);
}

// デストラクタ
HandManager::~HandManager() {
	Mouse::detach_drag_handler(this);
	const auto end_it = hands_.rend();
	for (auto it = hands_.rbegin(); it != end_it; ++it) {
		if (ITileEvent* event = (*it)->tile_data().event) delete event;
		(*it)->tile_data().event = nullptr;
	}
}

// 更新
void HandManager::update(float delta_time) {
	for (size_t i = 0; i < hands_.size(); ++i) {
		auto& card = hands_[i];
		if (!card || card.get() == dragging_card_) continue;
		card->move_to_wards(delta_time, positions_[i]);
	}

	//if (dragging_bit_.any()) return;

	//for (auto it = hands_.rbegin(); it != hands_.rend(); ++it) {
	//	auto& card = (*it);
	//	if (!card || !card->is_hovering()) card->set_focus(false);
	//	else { card->set_focus(true); break; }
	//}
}

// GUI描画
void HandManager::draw_gui() const {
	for (const auto& card : hands_) {
		if (card) card->draw_gui();
	}

#ifdef _DEBUG
	glPushMatrix();
	{
		const GSvector3 origin = Mouse::ray().origin;
		const GSvector3 direction = Mouse::ray().direction;
		const float t = -origin.y / direction.y;
		const GSvector3 world{
			origin.x + (direction.x * t),
			origin.y + (direction.y * t),
			origin.z + (direction.z * t)
		};
		glTranslatef(world.x, world.y, world.z);
		glutWireSphere(0.25f, 8, 8);
	}
	glPopMatrix();
#endif
}

// 手札カードの取得
std::vector<CardObject*> HandManager::cards() noexcept {
	std::vector<CardObject*> result;
	result.reserve(hands_.size());
	for (auto& card : hands_) {
		if (card) result.push_back(card.get());
	}
	return result;
}

// 操作可能フラグの設定
void HandManager::operable(bool is_operable) noexcept {
	is_operable_ = is_operable;
}

// ドラッグ開始時処理
void HandManager::on_drag_begin(const GSvector2& position) {
	// 操作可能でなければリターン
	if (!is_operable_) return;

	// 手札を逆順に走査
	const auto end_it = hands_.rend();
	for (auto it = hands_.rbegin(); it != end_it; ++it) {
		auto& card = *it;
		// カードが無い、またはホバー中でなければ以降を無視
		if (!card || !card->is_hovering()) continue;
		// 選択されたカードをドラッグ開始
		dragging_card_ = card.get();
		return;
	}
}

// ドラッグ中処理
void HandManager::on_dragging(
	const GSvector2& start_position, const GSvector2& current_position, const GSvector2& moved_delta) {
	// ドラッグ中のカードデータがなければリターン
	if (!dragging_card_) return;
	// ドラッグ中のカードを更新
	dragging_card_->on_dragging(current_position);
}

// ドラッグ終了時処理
void HandManager::on_drag_end(const GSvector2& position) {
	// ドラッグ中のカードデータがなければリターン
	if (!dragging_card_) return;
	// ドラッグ中カードからデータを取得
	dropped_card_data_ = dragging_card_data();
	// ドラッグ中カードを無効化
	dragging_card_ = nullptr;
}

// ドラッグ中カードデータの取得
TileData* HandManager::dragging_card_data() {
	return (dragging_card_) ? &dragging_card_->tile_data() : nullptr;
}

// ドロップされたカードデータの取得
TileData* HandManager::dropped_card_data() {
	placed_card_data_ = dropped_card_data_;
	dropped_card_data_ = nullptr;
	return placed_card_data_;
}

// 使用済みカードの反映
void HandManager::apply_used_card() {
	if (!placed_card_data_) return;

	// 設置済みカードを削除
	const auto it = std::find_if(
		hands_.begin(), hands_.end(),
		[this](const std::unique_ptr<CardObject>& card) {
			return (card) ? &card.get()->tile_data() == placed_card_data_ : false;
		}
	);
	hands_.erase(it);

	placed_card_data_ = nullptr;
}

// カードのドロー
void HandManager::draw_card(TileData& data) {
	if (hands_.size() >= MAX_HAND_QUANTITY) return;
	hands_.push_back(std::make_unique<CardObject>(DRAW_BEGIN_POSITION, data));
}

// 手札の解放
std::vector<TileData> HandManager::release() {
	std::vector<TileData> result;
	while (!hands_.empty()) {
		result.push_back(hands_.back()->tile_data());
		hands_.pop_back();
	}
	return result;
}