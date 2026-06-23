#ifndef CARDS_CARD_MANAGER_H_
#define CARDS_CARD_MANAGER_H_

#include <vector>
#include <string>
#include <memory>

#include "Cards/HandManager.h"
#include "UI/NumberTexture.h"

class TileManager;
class EventPool;
class MonsterPool;
struct EventContext;

// カードマネージャークラス
class CardManager {
public:
	CardManager();
	~CardManager();

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;
	// GUI描画
	void draw_gui(void) const;

	// 山札の読み込み
	void load_deck(const std::string& file_name, EventPool& event_pool, MonsterPool& monster_pool);
	// 山札のシャッフル
	void shuffle_deck(void);
	// 山札の消去
	void clear_deck(void);

	// カード使用枚数の取得
	int used_count(void) const noexcept;
	// ドラッグ中カードデータの取得
	TileData* dragging_card_data(void);
	// タイル設置の要求
	TileData* request_replace_tile(void);
	// 使用済みカードの反映
	void apply_used_card(void);

	// 手札の補充
	void hand_replenishment(void) noexcept;
	// 手札の入れ替え
	void hand_replacement(void) noexcept;
	// 操作可能フラグの設定
	void operable(bool is_operable) noexcept;
	// 使用可能カード存在フラグ
	bool can_placeable_hands(class TileGraph& graph);

private:
	// カード使用枚数
	int used_count_{ 0 };
	// 手札マネージャー
	HandManager hand_manager_;
	// 山札配列
	std::vector<TileData> deck_;

	// デッキ枚数表示用テクスチャ
	std::unique_ptr<NumberTexture> deck_num_texture_{ nullptr };
};

#endif // CARDS_CARD_MANAGER_H_