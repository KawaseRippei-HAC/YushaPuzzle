#include "Screens/SkillSelectScreen.h"
#include "Screens/OptionAssets.h"
#include "Systems/LoggableRandomizer.h"
#include "Skills/SkillTable.h"

// スキルの選択肢の数
static constexpr int SKILL_SELECT_NUM{ 3 };

/* --- スキルのボタン --- */
static const GSvector2 SKILL_BTN_SIZE{ 128, 128 };	// ボタンサイズ
static const GSvector2 SKILL_BTN_POS{ 0, 400 };		// ボタン位置
static const GSvector2 SKILL_BTN_SPACE{ (BTN_TEXTURE_SIZE_256 - SKILL_BTN_SIZE.x) * 0.5f, (BTN_TEXTURE_SIZE_256 - SKILL_BTN_SIZE.y) * 0.5f };	// テクスチャ内のボタンまでのスペース

/* --- スキル名 --- */
// フォントサイズ
static constexpr int SKILL_NAME_FONT_SIZE{ 64 };
// 描画位置
static const GSvector2 SKILL_NAME_POS{ 300,750 };

/* --- スキル説明 --- */
// フォントサイズ
static constexpr int SKILL_DESCRIPTION_FONT_SIZE{ 48 };
// 描画位置
static const GSvector2 SKILL_DESCRIPTION_POS{ SKILL_NAME_POS.x, SKILL_NAME_POS.y + 100 };

static const GSvector2 SKILL_COMPLETE_POS{ 1920 * 0.5f, 1080 * 0.5f };
static const GSvector2 SKILL_COMPLETE_CENTER{ 2048 * 0.5f, 2048 * 0.5f };

SkillSelectScreen::SkillSelectScreen(HeroCharacter* hero) :
	hero_{ hero } {
}
SkillSelectScreen::~SkillSelectScreen() {

}

// 初期化
void SkillSelectScreen::initialize() {
	// ボタンのクリア
	button_manager_.clear();
	// スキルの名前と説明を初期化
	skill_name_ = "";
	skill_description_ = "";

	// TODO: スキル情報をスキルプールから持ってくる
	// 全スキルID
	std::vector<SkillId> all_skills = {
		SkillId::Rigidity,
		SkillId::ReinforcingBar,
		SkillId::Blessing,
		SkillId::Agile,
		SkillId::GoodLuck,
		SkillId::Intimidation
	};
	// 使用可能スキル
	std::vector<SkillId> available_skills;

	int max = all_skills.size() - 1;

	// ランダムでスキルを三択で選ぶ
	// スキルがない場合か、スキルが3個決まったら、終了
	if (!all_skills.empty()) {
		while (available_skills.size() < SKILL_SELECT_NUM)
		{
			// ランダムなスキル
			int i = LoggableRandomizer::generate(0, max);
			SkillId find_skill = all_skills[i];
			// スキル検索
			if (hero_->skill_actor()->is_addable_skill(find_skill)) {
				// 追加できる場合、スキル選択用配列に追加
				available_skills.push_back(find_skill);
			}
			// 今回判定したスキルは削除
			all_skills[i] = all_skills.back();
			all_skills.pop_back();
			--max;
			if (max < 0)break;
		}
	}

	// ボタンの位置
	GSvector2 btn_pos = SKILL_BTN_POS;

	// 獲得できるスキルがないか
	is_no_skills_ = available_skills.empty();

	// スキルがあれば
	if (!is_no_skills_) {
		// 獲得できるスキルがある場合、スキルを獲得

		// ボタンの間隔 (1920 - ボタンサイズ * ボタンの数) / (ボタンの数+1)
		const float btn_space = (WINDOW_SIZE.x - SKILL_BTN_SIZE.x * available_skills.size()) / (available_skills.size() + 1);
		for (auto& skill_id : available_skills) {
			// スキルの検索
			const SkillDefinition* skill =  SkillTable::get_skill_data(skill_id);
			// ボタンの間隔分、位置をずらす
			btn_pos.x += btn_space;
			ButtonWidget* skill_btn = new ButtonWidget{ btn_pos, SKILL_BTN_SIZE, skill->texture, SKILL_BTN_SPACE };
			// スキルの追加と画面遷移
			skill_btn->attach_click([=]() {
				// スキルの追加
				hero_->skill_actor()->add_skill(skill_id);
				// バフ設定
				hero_->modifiers().set_buff(hero_->skill_actor()->non_battle_skill().buffs);
				// 現在のスクリーンを終了
				ScreenManager::pop_screen();
				});
			// ボタンのホバー中の登録
			skill_btn->attach_hover([=]() { skill_name_ = skill->name; skill_description_ = skill->description; });
			// ボタンの登録
			button_manager_.add_button(skill_btn);
			// ボタンのサイズ分ずらす
			btn_pos.x += SKILL_BTN_SIZE.x;
		}
	}
}
// 更新
void SkillSelectScreen::update(float delta_time) {
	if (!is_no_skills_) {
		button_manager_.update(delta_time);
	}
	else {
		if (gsGetKey() != 0 || gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
			hero_->add_status_point(1);
			ScreenManager::pop_screen();
		}
	}
}
// 描画
void SkillSelectScreen::draw(void) const {

	if (!is_no_skills_) {
		// BG
		gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);
		button_manager_.draw();

		GSvector2 pos2{ 400, 200 };
		GSvector2 center2{ 1024 * 0.5f, 1024 * 0.5f };
		gsDrawSprite2D(Texture::SkillSelect, &pos2, NULL, &center2, NULL, NULL, 0);

		GSvector2 pos{ 1920 * 0.5f, 1080 * 0.5f };
		GSvector2 center{ 2048 * 0.5f, 2048 * 0.5f };
		gsDrawSprite2D(Texture::SkillSelectFrame, &pos, NULL, &center, NULL, NULL, 0);

		// スキル名の描画
		gsSetSpriteFontStyle(GS_FONT_NORMAL, SKILL_NAME_FONT_SIZE, "MSゴシック");
		gsDrawSpriteFont(&SKILL_NAME_POS, skill_name_.c_str());

		// スキル説明の描画
		gsSetSpriteFontStyle(GS_FONT_NORMAL, SKILL_DESCRIPTION_FONT_SIZE, "MSゴシック");
		gsDrawSpriteFont(&SKILL_DESCRIPTION_POS, skill_description_.c_str());
	}
	else {
		gsDrawSprite2D(Texture::SkillComplete, &SKILL_COMPLETE_POS, NULL, &SKILL_COMPLETE_CENTER, NULL, NULL, 0);
	}
}
