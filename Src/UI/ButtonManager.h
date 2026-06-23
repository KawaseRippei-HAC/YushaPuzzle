#ifndef UI_BUTTON_MANAGER_H_
#define UI_BUTTON_MANAGER_H_

#include "UI/ButtonWidget.h"

// ボタン管理クラス
class ButtonManager {
public:
	// コンストラクタ
	ButtonManager();
	// デストラクタ
	~ButtonManager();

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;
	// ボタンの追加
	void add_button(ButtonWidget* button);
	// ボタンの削除
	void clear();

private:
	std::vector<ButtonWidget*> buttons_;
};


#endif
