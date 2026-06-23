#include"NumberTexture.h"
#include <sstream>
#include <iomanip>

// コンストラクタ
NumberTexture::NumberTexture(GSuint texture, int width, int height) :
	texture_{ texture }, width_{ width }, height_{ height } {
}

// 描画
void NumberTexture::draw(const GSvector2& position,
	int num, int digit, char fill, const GScolor& color,
	const GSvector2& scale, float specing) const {
	std::stringstream ss;
	ss << std::setw(digit) << std::setfill(fill) << num;
	draw(position, ss.str(), color, scale, specing);
}

// 描画
void NumberTexture::draw(const GSvector2& position, int num,
	const GScolor& color, const GSvector2& scale, float specing) const {
	draw(position, std::to_string(num), color, scale, specing);
}

// 文字からテクスチャ上のインデックスを取得するヘルパー関数
static int char_to_index(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c == '+') return 10;
	if (c == '-') return 11;
	//if (c == '.') return 12;
	if (c == ':') return 12;
	if (c == '/') return 13;
	return -1;
}

// 描画
void NumberTexture::draw(const GSvector2& position,
	const std::string& num, const GScolor& color, const GSvector2& scale, float specing) const {
	const int num_colums = 4;

	for (int i = 0; i < (int)num.size(); ++i) {
		if (num[i] == ' ') continue;

		const int n = char_to_index(num[i]);
		if (n < 0)continue;

		// 格子状のテクスチャから文字の矩形を計算
		const float u = static_cast<float>((n % num_colums) * width_);
		const float v = static_cast<float>((n / num_colums) * height_);
		const GSrect rect(u, v, u + width_, v + height_);

		// 文字の描画位置を計算する際に、文字の幅と間隔の両方を考慮する
		const float advance = (width_ * scale.x) + specing;

		const GSvector2 pos{ position.x + i * advance,position.y };
		gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, &scale, 0);
	}
}
