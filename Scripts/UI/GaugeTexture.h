#ifndef GAUGE_H_
#define GAUGE_H_

#include <gslib.h>

// ゲージテクスチャ
class GaugeTexture {
public:
	// コンストラクタ
	GaugeTexture(GSuint gauge_texture, int texture_width, int texture_height);

	// 描画
	void draw_clamp(
		const GSvector2& position,
		int width, int height, int value, int max_value,
		const GScolor& gauge_color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) const;

	// 描画
	void draw_loop(
		const GSvector2& position,
		int width, int height, int value, int max_value,
		const GScolor& gauge_color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) const;

private:
	// ゲージ画像
	GSuint gauge_texture_;
	// テクスチャ幅
	int texture_width_;
	// テクスチャ高さ
	int texture_height_;
};

#endif





