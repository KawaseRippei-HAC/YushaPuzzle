#ifndef RULE_EXPLANATION_SCREEN_H_
#define RULE_EXPLANATION_SCREEN_H_

#include "Interfaces/IScreen.h"
#include "Screens/ScreenManager.h"
#include "UI/ButtonManager.h"

class RuleExplanationScreen : public IScreen {
public:
	RuleExplanationScreen();
	~RuleExplanationScreen();

	// 初期化
	void initialize() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

private:

	// ボタン管理クラス
	ButtonManager button_manager_;

	bool* is_title_{ nullptr };
	bool* is_retry_{ nullptr };
};

#endif
