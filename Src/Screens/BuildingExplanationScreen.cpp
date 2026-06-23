#include "Screens/BuildingExplanationScreen.h"
#include "Screens/OptionAssets.h"
#include "Defines/Assets.h"
#include "Screens/ScreenManager.h"

static const GSvector2 EXPLANATION_CENTER{ 2048 * 0.5f, 1024 * 0.5f };

BuildingExplanationScreen::BuildingExplanationScreen(HeroCharacter* hero) :
	hero_{ hero } {
	is_start_frame_ = true;
}

BuildingExplanationScreen::~BuildingExplanationScreen() {
}

// 初期化
void BuildingExplanationScreen::initialize() {
	is_start_frame_ = true;

	// 勇者のいるイベントタイプに応じてテクスチャIDを変更
	switch (hero_->event_type()) {
	case Event::Inn:
		texture_id_ = Texture::InnEventExplanation;
		type_id_ = ExplanationTypeId::Inn;
		break;
	case Event::Training:
		texture_id_ = Texture::TrainingEventExplanation;
		type_id_ = ExplanationTypeId::Training;
		break;
	case Event::SkillAcquisition:
		texture_id_ = Texture::MasterHouseEventExplanation;
		type_id_ = ExplanationTypeId::Master;
		break;
	default: break;
	}

	// 初めて訪れて無ければこのスクリーンを消す
	if (is_first_time_[type_id_] != false) {
		ScreenManager::pop_screen();
	}
	else {
		is_first_time_[type_id_] = true;
	}
}
// 更新
void BuildingExplanationScreen::update(float delta_time) {
	// 何か押されたら
	if (!is_start_frame_ && (gsGetKey() != 0 || gsGetMouseButtonTrigger(GMOUSE_BUTTON_1))) {
		ScreenManager::pop_screen();
	}
	is_start_frame_ = false;
}
// 描画
void BuildingExplanationScreen::draw(void) const {
	// BG
	gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);

	// 説明
	gsDrawSprite2D(texture_id_, &WINDOW_CENTER, NULL, &EXPLANATION_CENTER, NULL, NULL, 0);
}
