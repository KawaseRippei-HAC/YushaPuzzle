#ifndef UI_SLIDER_H_
#define UI_SLIDER_H_

#include <vector>
#include <functional>

#include <GStype.h>

// スライダーウィジェットクラス
class SliderWidget {
public:
	/// <param name="initial_value">: 初期値（0.0f～1.0f）</param>
	/// <param name="position">: 表示位置（左上原点）</param>
	/// <param name="thumb_size">: 操作部分のサイズ</param>
	/// <param name="bar_size">: 背景部分のサイズ</param>
	/// <param name="thumb_texture">: 操作部分のテクスチャID</param>
	/// <param name="bar_texture">: 背景部分のテクスチャID</param>
	/// <param name="is_test_se">: ボタンを離した時、SEを再生するか</param>
	/// <param name="bar_texure_space">: 画像の左上から背景バーまでのスペース</param>
	SliderWidget(
		float initial_value,
		const GSvector2& position,
		const GSvector2& thumb_size, const GSvector2& bar_size,
		GSuint thumb_texture, GSuint bar_texture,
		bool is_test_se = false,
		const GSvector2& bar_texure_space = GSvector2{ 0, 0 }
	);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// オブザーバーの接続
	void attach(const std::function<void(float)>& observer);

	// 外部から値を強制的にセットする関数
	void set_value(float value);

private:
	// 通知処理
	void notify(float progress) const;

	// 範囲内フラグ
	static bool is_inside(const GSvector2& point, const GSrect& rectangle);
	// 操作部範囲内フラグ
	bool is_thumb_inside(const GSvector2& mouse_point) const;
	// 背景範囲内フラグ
	bool is_bar_inside(const GSvector2& mouse_point) const;

	// 操作部x座標の適用
	float apply_thumb_x(const GSvector2& mouse_point) const;
	// 値の適用
	float apply_value(void) const;

	// 値の変更
	void change_value(const GSvector2& mouse_position);

private:
	// 値
	float value_{ 0.0f };
	// 位置
	GSvector2 position_{ 0.0f, 0.0f };

	// 操作部サイズ
	GSvector2 thumb_size_{ 0.0f, 0.0f };
	// 背景サイズ
	GSvector2 track_size_{ 0.0f, 0.0f };

	// 操作部位置
	GSvector2 thumb_position_{ 0.0f, 0.0f };

	// 操作部テクスチャ
	GSuint thumb_texture_{ 0 };
	// 背景テクスチャ
	GSuint track_texture_{ 0 };

	// 背景の描画位置
	GSvector2 track_draw_pos_{ 0.0f, 0.0f };

	// ホールド中フラグ
	bool is_holding_{ false };

	// ボタンを離した時、SEを再生するか
	bool is_test_se_{ false };

	// SE再生フラグ
	bool is_se_playback_{ false };

	// オブザーバー配列
	std::vector<std::function<void(float)>> observers_;
};

#include "Inlines/SliderWidget.inl"

#endif // UI_SLIDER_H_