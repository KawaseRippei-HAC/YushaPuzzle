#ifndef CARDS_HAND_MANAGER_H_
#define CARDS_HAND_MANAGER_H_

#include <vector>
#include <memory>
#include <bitset>

#include "Cards/CardObject.h"
#include "Interfaces/IDragHandler.h"

/*定数*/
// 最大手札枚数
inline constexpr size_t MAX_HAND_QUANTITY{ 5 };

class CardManager;

// 手札マネージャークラス
class HandManager :
	public IDragHandler {
public:
	HandManager();
	~HandManager();

	// 更新
	void update(float delta_time);
	// GUI描画
	void draw_gui(void) const;

	// 手札カードの取得
	std::vector<CardObject*> cards(void) noexcept;
	// 操作可能フラグの設定
	void operable(bool is_operable) noexcept;

	// ドラッグ開始時処理
	void on_drag_begin(const GSvector2& position) override;
	// ドラッグ中処理
	void on_dragging(
		const GSvector2& start_position, const GSvector2& current_position, const GSvector2& moved_delta) override;
	// ドラッグ終了時処理
	void on_drag_end(const GSvector2& position) override;

	// ドラッグ中カードデータの取得
	TileData* dragging_card_data(void);
	// ドロップされたカードデータの取得
	TileData* dropped_card_data(void);

	// 使用済みカードの反映
	void apply_used_card(void);
	// カードのドロー
	void draw_card(TileData& data);

	// 手札の解放
	std::vector<TileData> release(void);

private:
	// 操作可能フラグ
	bool is_operable_{ true };

	// ドラッグ中カード
	CardObject* dragging_card_{ nullptr };
	// ドロップされたカードデータ
	TileData* dropped_card_data_{ nullptr };
	// 設置されたカードデータ
	TileData* placed_card_data_{ nullptr };

	// 手札カード配列
	std::vector<std::unique_ptr<CardObject>> hands_;
	// 手札位置配列
	std::array<GSvector2, MAX_HAND_QUANTITY> positions_{ GSvector2::zero() };
};

#endif // CARDS_HAND_MANAGER_H_