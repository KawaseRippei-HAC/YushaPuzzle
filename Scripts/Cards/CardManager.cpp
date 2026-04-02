#include "Cards/CardManager.h"

#include <string>
#include <sstream>
#include <fstream>

#include "Events/EventPool.h"
#include "Events/EvecntContext.h"
#include "Tiles/TileManager.h"
#include "Systems/LoggableRandomizer.h"
#include "Characters/Monsters/MonsterPool.h"

/*定数*/
// 山札最大枚数
static constexpr size_t MAX_DECK_SIZE{ 50 };
// 数字テクスチャの大きさ
static constexpr int NUMBER_TEXTURE_SIZE = 128;
// デッキ50のライン
static constexpr int DECK50_LINE = 25;
// デッキ25のライン 
static constexpr int DECK25_LINE = 10;
// デッキテクスチャの大きさ
static constexpr float DECK_TEXTURE_SIZE = 512.0f;
// デッキ描画位置
static const GSvector2 DECK_POSITION{ 1610.0f,570.0f };
// デッキのスケール
static const GSvector2 DECK_SCALE{ 0.6f,0.6f };
// テキストの大きさ
static constexpr float TEXT_SIZE = 128.0f;
// テキストの位置補正 X
static constexpr float TEXT_POS_CORRECTION_X = 10.0f;
// テキストの位置補正 Y
static constexpr float TEXT_POS_CORRECTION_Y = 20.0f;
// 数字の間隔
static constexpr float SPECING{ -17.0f };

/*実装*/
// コンストラクタ
CardManager::CardManager() {
	// 数字テクスチャの初期化
	deck_num_texture_ = std::make_unique<NumberTexture>(Texture::Number, NUMBER_TEXTURE_SIZE, NUMBER_TEXTURE_SIZE);
}

// デストラクタ
CardManager::~CardManager() {
	this->clear_deck();
}

// 更新
void CardManager::update(float delta_time) {
	hand_manager_.update(delta_time);
}

// 描画
void CardManager::draw() const {

}

// GUI描画
void CardManager::draw_gui() const {
	hand_manager_.draw_gui();

	// デッキの描画処理
	int deck_count = static_cast<int>(deck_.size());
	// 0枚の時は表示しない
	if (deck_count > 0) {
		GSuint deck_tex_id = 0;

		// 条件に応じたテクスチャ選択
		if (deck_count > DECK50_LINE) {
			deck_tex_id = Texture::Deck50;
		}
		else if (deck_count > DECK25_LINE) {
			deck_tex_id = Texture::Deck25;
		} else {
			deck_tex_id = Texture::Deck10;
		}

		// デッキ画像の描画
		gsDrawSprite2D(deck_tex_id, &DECK_POSITION, NULL, NULL, NULL, &DECK_SCALE, 0.0f);

		// 枚数の描画
		if (deck_num_texture_) {
			float display_w = DECK_TEXTURE_SIZE * DECK_SCALE.x;
			float display_h = DECK_TEXTURE_SIZE * DECK_SCALE.y;

			// 画像の中心座標
			float center_x = DECK_POSITION.x + (display_w / 2.0f) + TEXT_POS_CORRECTION_X;
			float center_y = DECK_POSITION.y + (display_h / 2.0f) + TEXT_POS_CORRECTION_Y;

			// 数字のスケール
			GSvector2 num_scale{ 0.4f,0.4f };

			// 文字列の幅を計算
			int digits = std::to_string(deck_count).length();
			float text_width = TEXT_SIZE * num_scale.x * digits;
			float text_height = TEXT_SIZE + num_scale.y;

			// 中央寄せのための座標計算
			GSvector2 num_pos{
				center_x - (text_width / 2.0f),
				center_y - (text_height / 2.0f)
			};

			// 数字の描画
			deck_num_texture_->draw(num_pos, deck_count, GScolor(0.0f, 0.0f, 0.0f, 1.0f), num_scale, SPECING);
		}
	}
}

// 山札の読み込み
void CardManager::load_deck(const std::string& file_name, EventPool& event_pool, MonsterPool& monster_pool) {
	// ファイルの読み込み
	std::ifstream file{ file_name };
	if (!file.is_open()) return;

	EventContext context{ .monster_pool_ = &monster_pool };

	std::string line{ "" };
	while (deck_.size() < MAX_DECK_SIZE && std::getline(file, line)) {
		std::string cell{ "" };
		std::stringstream ss{ line };
		while (std::getline(ss, cell, ',')) {
			// TODO: イベントプールからのランダムイベント生成ではなく、指定イベントを生成できるようにする処理
			deck_.push_back(TileManager::parse_data(cell, event_pool, context));
		}
	}

	this->shuffle_deck();

	// 手札の初期化
	for (size_t i = 0; i < MAX_HAND_QUANTITY; ++i) {
		TileData& data = deck_.back(); deck_.pop_back();
		hand_manager_.draw_card(data);
	}
}

// 山札のシャッフル
void CardManager::shuffle_deck() {
	Randomizer::log("<deck shuffle>");
	std::shuffle(deck_.begin(), deck_.end(), Randomizer::instance());
	Randomizer::log("<deck shuffle end>");
}

// 山札の消去
void CardManager::clear_deck() {
	const auto end_it = deck_.rend();
	for (auto it = deck_.rbegin(); it != end_it; ++it) {
		if (ITileEvent* event = (*it).event) delete event;
		(*it).event = nullptr;
	}
	deck_.clear();
}

// カード使用枚数の取得
int CardManager::used_count() const noexcept {
	return used_count_;
}

// ドラッグ中カードデータの取得
TileData* CardManager::dragging_card_data() {
	return hand_manager_.dragging_card_data();
}

// タイル設置の要求
TileData* CardManager::request_replace_tile() {
	return hand_manager_.dropped_card_data();
}

// 使用済みカードの反映
void CardManager::apply_used_card() {
	// カードを使用済みに
	hand_manager_.apply_used_card();

	// 使用枚数の加算
	++used_count_;
}

// 手札の補充
void CardManager::hand_replenishment() noexcept {
	while (hand_manager_.cards().size() < MAX_HAND_QUANTITY) {
		if (deck_.empty()) break;
		TileData& data = deck_.back(); deck_.pop_back();
		hand_manager_.draw_card(data);
	}
}

// 手札の入れ替え
void CardManager::hand_replacement() noexcept {
	// 手札を退避
	std::vector<TileData> old = hand_manager_.release();
	// 再ドロー
	while (hand_manager_.cards().size() < MAX_HAND_QUANTITY) {
		if (deck_.empty()) break;
		TileData& data = deck_.back(); deck_.pop_back();
		hand_manager_.draw_card(data);
	}
	// 元の手札をデッキに戻してシャッフル
	for (TileData& data : old) deck_.push_back(data);
	this->shuffle_deck();
}

// 操作可能フラグの設定
void CardManager::operable(bool is_operable) noexcept {
	hand_manager_.operable(is_operable);
	hand_manager_.on_drag_end(GSvector2::zero());
}

// 使用可能カード存在フラグ
bool CardManager::can_placeable_hands(TileGraph& graph) {
	for (int z = 0; z < graph.depth(); ++z) {
		for (int x = 0; x < graph.width(); ++x) {
			for (CardObject* card : hand_manager_.cards()) {
				if (graph.is_placeable(x, z, card->tile_data())) return true;
			}
		}
	}
	return false;
}