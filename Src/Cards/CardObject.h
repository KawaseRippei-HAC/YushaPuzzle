#ifndef CARDS_CARD_OBJECT_H_
#define CARDS_CARD_OBJECT_H_

#include <memory>
#include <optional>

#include <GStype.h>

#include "Tiles/TileData.h"
#include "Utilities/Noncopyable.h"
#include "Interfaces/IDragHandler.h"

// カードオブジェクトクラス
class CardObject :
	private Noncopyable<CardObject> {
public:
	explicit CardObject(GSvector2 position, TileData& data);

	// 更新
	void update(float delta_time);
	// GUI描画
	void draw_gui(void) const;

	// タイルデータの取得
	TileData& tile_data(void) noexcept;

	// ドラッグ開始処理
	void on_drag_begin(GSvector2 position);
	// ドラッグ中処理
	void on_dragging(GSvector2 position);
	// ドラッグ中処理
	void on_drag_end(GSvector2 position);

	// 位置の補間
	void move_to_wards(float delta_time, GSvector2 position) noexcept;

	// 選択中フラグ
	bool is_hovering(void) const noexcept;
	// フォーカスの設定
	void set_focus(bool is_focused) noexcept;

private:
	// スプライトIDの変換
	static constexpr GSuint shape_to_sprite(RoadShape shape);

private:
	// タイルデータ
	TileData data_;
	// フォーカス中フラグ
	bool is_focusing_{ false };
	// カードサイズ
	GSvector2 size_{ 0.0f, 0.0f };
	// 位置
	GSvector2 position_{ 0.0f, 0.0f };
};

#endif // CARDS_CARD_OBJECT_H_