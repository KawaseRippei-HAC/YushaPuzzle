#include <UI/GaugeTexture.h>

/*実装*/
// コンストラクタ
GaugeTexture::GaugeTexture(GSuint gauge_texture, int texture_width, int texture_height) :
	gauge_texture_{ gauge_texture },
	texture_width_{ texture_width },
	texture_height_{ texture_height } {
}

// 描画(値を最大値でクランプする)
void GaugeTexture::draw_clamp(
	const GSvector2& position,
	int width, int height, int value, int max_value,
	const GScolor& gauge_color) const {
	if (value > max_value) { value = max_value; }
	if (value < 0) { value = 0; }

	const float rate = (max_value == 0) ? 0.0f : (value / (float)max_value);

	// テクスチャの幅をrateに応じて切り出す
	const GSrect texture_rect{
		0.0f,
		0.0f,
		texture_width_ * rate,
		(float)texture_height_
	};
	// ゲージのスケーリング
	const GSvector2 gauge_scaling{
		(width / (float)texture_width_),
		height / (float)texture_height_
	};
	// アンカーポイントを左上に明示的に指定する
	const GSvector2 anchor{ 0.0f,0.0f };

	// ゲージの描画
	gsDrawSprite2D(gauge_texture_, &position, &texture_rect, &anchor,
		&gauge_color, &gauge_scaling, 0.0f);
}

// 描画(値を最大値でループ(0~max)させる
void GaugeTexture::draw_loop(
	const GSvector2& position,
	int width, int height, int value, int max_value,
	const GScolor& gauge_color) const {
	if (max_value <= 0)return;
	if (value < 0) { value = 0; }

	value = value % max_value;

	const float rate = value / (float)max_value;

	const GSvector2 gauge_scaling{
		(width / (float)texture_width_) * rate,
		height / (float)texture_height_
	};
	gsDrawSprite2D(gauge_texture_, &position, NULL, NULL,
		&gauge_color, &gauge_scaling, 0.0f);
}