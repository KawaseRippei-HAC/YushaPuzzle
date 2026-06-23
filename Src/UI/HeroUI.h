#ifndef UI_HERO_STATUS_H_
#define UI_HERO_STATUS_H_

#include "UI/NumberTexture.h"

class HeroCharacter;

// 勇者UI表示クラス
class HeroUI {
public:
	// コンストラクタで表示対象の勇者パラメータを受け取る
	HeroUI(const HeroCharacter* hero_char);

	// 描画処理
	void draw_gui()const;

private:
	// 参照勇者パラメータ
	const HeroCharacter* hero_character_{ nullptr };
	// 数値描画用のインスタンス
	const NumberTexture number_texture_;

private:
	// HPの描画
	void draw_hp() const;
	// 経験値の描画
	void draw_exp() const;
	// 名前関連の描画
	void draw_name_level() const;
	// 所持金の描画
	void draw_gold() const;
	// 所持品関連の描画
	void draw_item() const;
};

#endif // UI_HERO_STATUS_H_
