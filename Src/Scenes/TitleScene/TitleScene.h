#ifndef SCENES_TITLE_SCENE_TITLE_SCENE_H_
#define SCENES_TITLE_SCENE_TITLE_SCENE_H_

#include "Interfaces/IScene.h"
#include "Screens/ScreenManager.h"
#include "UI/ButtonManager.h"

// タイトルシーンクラス
class TitleScene final :
	public IScene {
public:
	// 開始
	void start(void) override;

	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

	// 終了
	void end(void) override;
	// 終了フラグ
	bool is_end(void) const noexcept override;

	// 次シーンIDの取得
	Scene next(void) const noexcept override;

	// ゲーム終了フラグ
	bool is_game_end(void) const noexcept override;

private:
	// 終了フラグ
	bool is_end_{ false };

	// ボタン管理クラス
	ButtonManager button_manager_;

	bool is_game_end_{ false };
};

#include "Inlines/TitleScene.inl"

#endif // SCENES_TITLE_SCENE_TITLE_SCENE_H_