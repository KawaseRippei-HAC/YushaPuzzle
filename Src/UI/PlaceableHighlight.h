#ifndef UI_PLACEABLE_HIGHLIGHT_H_
#define UI_PLACEABLE_HIGHLIGHT_H_

// 設置可能ハイライトクラス
class PlaceableHighlight {
public:
	PlaceableHighlight(class TileManager& tile_manager);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// 設置可能の判定
	void judge_placeable(struct TileData& data);
	// 表示可能の設定
	void visible(bool is_visible);

	// ハイライト中座標の取得
	std::pair<int, int> highlighted_coord(void) const noexcept;

private:
	int prev_x_{ 0 };
	int prev_z_{ 0 };
	bool is_visible_{ false };
	bool is_placeable_{ false };
	GSvector3 position_{ 0.0f, 0.0f, 0.0f };
	GSvector3 destination_{ 0.0f, 0.0f, 0.0f };
	TileManager* tile_manager_{ nullptr };
};

#endif // UI_PLACEABLE_HIGHLIGHT_H_