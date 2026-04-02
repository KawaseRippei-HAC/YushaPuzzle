#include "UI/HeroUI.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "UI/GaugeTexture.h"
#include "Defines/Assets.h"
#include <string>

/*定数*/
// 数字テクスチャの大きさ
static constexpr int NUMBER_TEXTURE_SIZE = 128;
// 黒
static const GScolor COLOR_B{ 0.0f,0.0f,0.0f,1.0f };
// 白
static const GScolor COLOR_W{ 1.0f,1.0f,1.0f,1.0f };
// 文字間隔
static constexpr float SPECING{ -20.0f };

// --- HP関連 ---//
// 基準となるHPゲージの位置
static const GSvector2	HP_GAUGE_POS{ 64.0f,800.0f };
// HPゲージフレームの位置
static const GSvector2	HP_GAUGE_FRAME_POS{ HP_GAUGE_POS.x,HP_GAUGE_POS.y };
// HP数値テキストの位置
static const GSvector2 HP_TEXT_POS{ HP_GAUGE_POS.x + 130.0f,HP_GAUGE_POS.y + 151.0f };
// HPゲージのテクスチャサイズと表示サイズ
static const GSvector2 HP_TEXTURE_SIZE{ 512.0f,512.0f };
static const GSvector2 HP_DISPLAY_SIZE{ 512.0f * 0.7f,512.0f * 0.6f };
// 大きさ補正
static const GSvector2 HP_GAUGE_SCALING{ 0.7f,0.6f };
static const GSvector2 HP_TEXT_SCALE{ 0.1f,0.1f };

// --- 経験値関連 ---//
// 基準となる経験値ゲージの位置
static const GSvector2	EXP_GAUGE_POS{ 100.0f,840.0f };
// 経験値ゲージ背景の位置
static const GSvector2	EXP_GAUGE_BG_POS{ EXP_GAUGE_POS.x - 10.0f,EXP_GAUGE_POS.y };
// 経験値数値テキストの位置
static const GSvector2 EXP_TEXT_POS{ EXP_GAUGE_POS.x + 110.0f,EXP_GAUGE_POS.y + 180.0f };
// 経験値ゲージのテクスチャサイズと表示サイズ
static const GSvector2 EXP_TEXTURE_SIZE{ 512.0f,512.0f };
static const GSvector2 EXP_DISPLAY_SIZE{ 512.0f * 0.6f,512.0f * 0.6f };
// 大きさ補正
static const GSvector2 EXP_TEXT_SCALE{ 0.15f,0.15f };
static const GSvector2 EXP_GAUGE_BG_SCALING{ 0.6f,0.6f };
// 文字間隔
static constexpr float EXP_TEXT_SPECING{ -7.0f };

// --- LV関連 --- //
// 名前表示の位置
static const GSvector2 NAME_POS{ 64.0f,850.0f };
// ｢Lv｣画像の位置
static const GSvector2 LEVEL_LABEL_POS{ NAME_POS.x + 110.0f,NAME_POS.y + 24.0f };
// レベル(数値)の位置
static const GSvector2 LEVEL_VALUE_POS{ LEVEL_LABEL_POS.x + 50.0f,LEVEL_LABEL_POS.y };
// 大きさ補正
static const GSvector2 NAME_SCALING{ 0.2f,0.2f };
static const GSvector2 LEVEL_SCALING{ 0.1f,0.1f };
static const GSvector2 LEVEL_TEXT_SCALING{ 0.4f,0.4f };
// LVの桁数
static constexpr int LEVEL_DIGIT{ 3 };

// --- 所持金関連 --- //
// 金袋アイコン表示の位置
const GSvector2 GOLD_BASE_POS{ 435.0f,870.0f + 100.0f };
// 所持金数値のオフセット
const GSvector2 GOLD_VALUE_POS{ GOLD_BASE_POS.x + 64.0f,GOLD_BASE_POS.y + 15.0f };
// 大きさ補正
const GSvector2 GOLD_SCALING{ 0.13f,0.13f };
const GSvector2 GOLD_TEXT_SCALING{ 0.3f,0.3f };
// 文字間隔
static constexpr float GOLD_TEXT_SPECING{ -10.0f };
// 所持金の桁数
static constexpr int GOLD_DIGIT{ 6 };

// --- 所持品関連 --- //
// 武器の位置
const GSvector2 WEAPON_POS{ 485.0f,950.0f };
// 盾の位置
const GSvector2 SHIELD_POS{ WEAPON_POS.x + 90.0f,WEAPON_POS.y };
// 大きさ補正
const GSvector2 INVENTRY_FRAME_SCALING{ 0.8f,0.8f };

// --- ターン関連 --- //
// 時間アイコンの位置
const GSvector2 TURN_ICON_POS{ 427.0f,860.0f };


/*実装*/
// コンストラクタ
HeroUI::HeroUI(const HeroCharacter* hero_char) :
	hero_character_{ hero_char }, number_texture_{ Texture::Number,NUMBER_TEXTURE_SIZE,NUMBER_TEXTURE_SIZE } {
}

// 描画処理
void HeroUI::draw_gui()const {
	// ポインタか有効か
	if (!hero_character_) return;

	// 各UI描画の呼び出し
	draw_hp();
	draw_exp();
	draw_name_level();
	draw_gold();
	//draw_item();

	/*時間関連の描画*/ {
		// 時間アイコンの描画
		gsDrawSprite2D(
			Texture::Time,
			&TURN_ICON_POS,
			NULL, NULL, NULL, NULL,
			0.0f
		);
	}
}

// HPUIの描画
void HeroUI::draw_hp() const {
	// HPの受け取り
	const int current_hp = static_cast<int>(hero_character_->hp());
	const int max_hp = static_cast<int>(hero_character_->max_hp());

	// ゲージ用のテクスチャを準備
	static const GaugeTexture gauge{
		Texture::HpGauge,
		static_cast<int>(HP_TEXTURE_SIZE.x),
		static_cast<int>(HP_TEXTURE_SIZE.y) };

	// HPゲージ背景の描画
	gsDrawSprite2D(
		Texture::HpGauge_bg,
		&HP_GAUGE_POS,
		NULL, NULL, NULL, &HP_GAUGE_SCALING,
		0.0f
	);

	// HPゲージを描画
	gauge.draw_clamp(
		HP_GAUGE_POS,
		HP_DISPLAY_SIZE.x, HP_DISPLAY_SIZE.y,
		current_hp, max_hp,
		COLOR_W
	);

	// HPフレームの描画
	gsDrawSprite2D(
		Texture::HpGaugeFrame,
		&HP_GAUGE_FRAME_POS,
		NULL, NULL, NULL, &HP_GAUGE_SCALING,
		0.0f
	);

	// HP数値の描画
	std::stringstream ss;
	ss << current_hp << "/" << max_hp;
	number_texture_.draw(HP_TEXT_POS, ss.str(), COLOR_B, HP_TEXT_SCALE);
}

// EXPUIの描画
void HeroUI::draw_exp() const {
	// EXPの受け取り
	const int current_exp = hero_character_->experience();
	const int required_exp = hero_character_->required_experience();

	// ゲージ用のテクスチャを準備
	static const GaugeTexture gauge{
	 Texture::ExpGauge,
	 static_cast<int>(EXP_TEXTURE_SIZE.x),
	 static_cast<int>(EXP_TEXTURE_SIZE.y)
	};

	// EXP背景の描画
	gsDrawSprite2D(
		Texture::ExpGauge_bg,
		&EXP_GAUGE_BG_POS,
		NULL, NULL, NULL, &EXP_GAUGE_BG_SCALING,
		0.0f
	);

	// EXPゲージを描画
	gauge.draw_loop(
		EXP_GAUGE_POS,
		EXP_DISPLAY_SIZE.x, EXP_DISPLAY_SIZE.y,
		current_exp, required_exp,
		COLOR_W
	);

	// EXP数値の描画
	std::stringstream ss;
	ss << current_exp << "/" << required_exp;
	number_texture_.draw(EXP_TEXT_POS,
		ss.str(), COLOR_B, EXP_TEXT_SCALE,
		EXP_TEXT_SPECING);
}

// 名前関連UIの描画
void HeroUI::draw_name_level() const {
	/*--- 勇者名とレベルの描画 ---*/
	// 勇者の名前表示
	gsDrawSprite2D(Texture::YushaName,
		&NAME_POS,
		NULL, NULL, &COLOR_B, &NAME_SCALING,
		0.0f);

	// Level
	// ｢LV｣の表示
	gsDrawSprite2D(
		Texture::Level,
		&LEVEL_LABEL_POS,
		NULL, NULL, &COLOR_B, &LEVEL_SCALING,
		0.0f);

	// レベル数値の表示
	number_texture_.draw(
		LEVEL_VALUE_POS,
		static_cast<int>(hero_character_->level()),
		LEVEL_DIGIT,
		'0',
		COLOR_B,
		LEVEL_TEXT_SCALING,
		SPECING);
}

// 所持金UIの描画
void HeroUI::draw_gold() const {
	// 金袋テクスチャの表示
	gsDrawSprite2D(
		Texture::MoneyIcon,
		&GOLD_BASE_POS,
		NULL, NULL, NULL, &GOLD_SCALING,
		0.0f
	);

	// 所持金の表示
	number_texture_.draw(
		GOLD_VALUE_POS,
		static_cast<int>(hero_character_->gold()),
		GOLD_DIGIT,
		'0',
		COLOR_B,
		GOLD_TEXT_SCALING,
		GOLD_TEXT_SPECING);
}

// 所持品UIの描画
void HeroUI::draw_item() const {
	// フレームの描画
	gsDrawSprite2D(
		Texture::InventoryFrame,
		&WEAPON_POS,
		NULL, NULL, NULL, &INVENTRY_FRAME_SCALING,
		0.0f
	);
	gsDrawSprite2D(
		Texture::InventoryFrame,
		&SHIELD_POS,
		NULL, NULL, NULL, &INVENTRY_FRAME_SCALING,
		0.0f
	);
}