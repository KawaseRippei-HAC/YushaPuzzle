#ifndef SCREENS_STATUS_ALLOCATION_SCREEN_H_
#define SCREENS_STATUS_ALLOCATION_SCREEN_H_

#include "Interfaces/IScreen.h"
#include "Screens/ScreenManager.h"
#include "UI/ButtonManager.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "UI/NumberTexture.h"
#include "Screens/OptionAssets.h"

class StatusAllocationScreen : public IScreen {
public:

	StatusAllocationScreen(HeroCharacter* hero);
	~StatusAllocationScreen();

	// 初期化
	void initialize() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

private:
	// ステータス追加
	void status_up(int num);
	// ステータスの上昇値の描画
	void status_up_draw(int num) const;

	// ステータス描画
	void draw_status(int num) const;

	// スキルレベル描画
	void draw_skill_level() const;

	// スキル説明のポップアップ表示
	void skill_pop_up(void) const;
private:

	/* --- マネージャー --- */
	// ボタン管理クラス
	ButtonManager button_manager_;
	// 勇者のパラメータ
	HeroCharacter* hero_{ nullptr };

	NumberTexture number_texture_{ Texture::Number, NUMBER_SIZE, NUMBER_SIZE };

	// スキル
	int skill_up_id_{ 0 };

	// ポップアップ表示
	bool is_pop_up_{ false };
	// スキル名
	std::string skill_name_{ "" };
	// スキル説明
	std::string skill_description_{ "" };
	// スキル
	std::vector<SkillState> hero_skill_state_;
};

#endif // SCREENS_STATUS_ALLOCATION_SCREEN_H_
