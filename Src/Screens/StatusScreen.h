#ifndef SCREENS_STATUS_SCREEN_H_
#define SCREENS_STATUS_SCREEN_H_

#include "Interfaces/IScreen.h"
#include "Screens/ScreenManager.h"
#include "UI/ButtonManager.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "UI/NumberTexture.h"
#include "Defines/Assets.h"

// 一文字のサイズ
static constexpr int STATUS_NUMBER_SIZE{ 128 };

namespace ParamaterState {
	enum {
		HP,
		Attack,
		Defense,
		Speed,
		Luck,

		Max
	};
}


struct Paramater {
	level_t level{ 0 };
	point_t hp{ 0 };
	point_t max_hp{ 0 };
	attack_t attack{ 0 };
	defense_t defense{ 0 };
	speed_t speed{ 0 };
	luck_t luck{ 0 };
};


class StatusScreen : public IScreen {
public:
	
	StatusScreen(HeroCharacter* hero_parameter = nullptr);
	~StatusScreen();

	// 初期化
	void initialize() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

private:
	// パラメータを設定
	void set_parameter(const level_t level, const point_t hp, const point_t max_hp,
		const attack_t attack, const defense_t defense, const speed_t speed, const luck_t luck);

	// レベル描画
	void draw_level(void) const;

	// ステータス描画
	void draw_status(int num) const;

	// 修正値を描画（バフとデバフ）
	void draw_modifier(int num) const;

	// スキルレベル描画
	void draw_skill_level() const;

	// スキル説明のポップアップ表示
	void skill_pop_up(void) const;

private:

	/* --- マネージャー --- */
	// ボタン管理クラス
	ButtonManager button_manager_;
	// 勇者のパラメータ
	HeroCharacter* hero_character_;
	// パラメータ
	Paramater paramater_;
	// ナンバーテクスチャ
	NumberTexture number_texture_{ Texture::Number, STATUS_NUMBER_SIZE, STATUS_NUMBER_SIZE };
	// バフとデバフの値
	Modifier modifier_;

	// リザルト用のステータス表示かのフラグ
	bool is_result_status_{ false };

	// ポップアップ表示
	bool is_pop_up_ { false };
	// スキル名
	std::string skill_name_{ "" };
	// スキル説明
	std::string skill_description_{ "" };
	// スキル
	std::vector<SkillState> hero_skill_state_;
};

#endif // SCREENS_STATUS_SCREEN_H_
