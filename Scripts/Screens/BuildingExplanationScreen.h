#ifndef SCREENS_EVENT_EXPLANATION_SCREEN_H_
#define SCREENS_EVENT_EXPLANATION_SCREEN_H_

#include "Interfaces/IScreen.h"
#include "Characters/Heroes/HeroCharacter.h"

// 建物のタイプID
namespace ExplanationTypeId {
	enum ExplanationTypeId : GSuint {
		Inn,		// 宿屋
		Training,	// 訓練
		Master,		// スキル獲得

		Max
	};
}

// イベントの説明スクリーン
class BuildingExplanationScreen : public IScreen {
public:
	BuildingExplanationScreen(HeroCharacter* hero);
	~BuildingExplanationScreen();

	// 初期化
	void initialize() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

private:
	// 開始フレームかどうか
	bool is_start_frame_{ false };

	// 説明画像のテクスチャID
	GSuint texture_id_{ 0 };

	// 建物のタイプID
	GSuint type_id_{ 0 };

	// 初めて到着したか
	bool is_first_time_[ExplanationTypeId::Max]{ false, false, false };

	HeroCharacter* hero_{ nullptr };
};

#endif // SCREENS_EVENT_EXPLANATION_SCREEN_H_
