#ifndef UI_BUTTON_H_
#define UI_BUTTON_H_

#include <vector>
#include <functional>

#include <GStype.h>

// ボタンの画像サイズ（256x256）
static constexpr int BTN_TEXTURE_SIZE_256{ 256 };
// ボタンの画像サイズ（512x512）
static constexpr int BTN_TEXTURE_SIZE_512{ 512 };
// ボタンの画像サイズ（1024x1024）
static constexpr int BTN_TEXTURE_SIZE_1024{ 1024 };
// ボタンのフォントサイズ 57
static constexpr int BTN_FONT_SIZE_57{ 57 };
// ボタンのフォントサイズ 90
static constexpr int BTN_FONT_SIZE_90{ 90 };

class ButtonWidget {
public:
	/// <param name="position">表示位置（左上座標）XY</param>
	/// <param name="button_range">ボタン範囲 XY</param>
	/// <param name="button_texture">テクスチャID</param>
	/// <param name="button_range">画像の左上からボタンまでのスペース（あれば）</param>
	/// <param name="text">テキスト（あれば）</param>
	/// <param name="text_size">テキストサイズ（何も書かなければ64）</param>
	ButtonWidget(GSvector2 position, GSvector2 button_range, GSuint button_texture, GSvector2 space = GSvector2{ 0, 0 }, const char* text = "", int text_size = 64);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// オブザーバーの接続（クリック時の処理）
	void attach_click(const std::function<void()>& observer);
	// オブザーバーの接続（ホバー中の処理）
	void attach_hover(const std::function<void()>& observer);

private:
	// 通知処理（クリック）
	void click_notify() const;
	// 通知処理（ホバー）
	void hover_notify() const;

	// ボタン範囲
	bool is_inside(const GSvector2& point);

private:

	// ボタンの左上の座標
	GSvector2 position_{ 0.0f, 0.0f };
	// ボタンのテクスチャ
	GSuint button_texture_{ 0 };
	// テキスト
	std::string text_;
	// テキストサイズ
	int text_size_;
	// テキストの場所
	GSvector2 text_pos_;
	// ボタン範囲
	GSrect rectangle_{ 0.0f, 0.0f, 0.0f, 0.0f };
	// ボタンにマウスカーソルが重なっているか
	bool is_hover_{ false };
	// 前のフレームでボタンにマウスカーソルが重なっていたか
	bool prev_is_hover_{ false };

	// オブザーバー配列（クリック）
	std::vector<std::function<void()>> click_observers_;
	// オブザーバー配列（ホバー）
	std::vector<std::function<void()>> hover_observers_;
};

#endif
