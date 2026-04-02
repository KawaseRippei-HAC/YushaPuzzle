#ifndef SCENES_RESULT_SCENE_RESULT_SCENE_H_
#define SCENES_RESULT_SCENE_RESULT_SCENE_H_

#include "Interfaces/IScene.h"
#include "UI/ButtonManager.h"
#include "Screens/ScreenManager.h"


namespace NextScene {
	enum : GSuint {
		None,
		Title,
		GamePlay,
		StageSelect
	};
}

// リザルトシーンクラス
class ResultScene final :
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

private:
	// 終了フラグ
	bool is_end_{ false };

	// ゲームクリアフラグ
	bool is_gameclear_{ false };

	// 次のシーン
	int next_scene_{ NextScene::None };

	// BGMフラグ（SEが鳴り終わった後にBGM再生用）
	bool is_bgm_{ false };

	ButtonManager button_manager_;
	//スクリーン管理クラス
	ScreenManager* screen_manager_{ nullptr };
};

#include "Inlines/ResultScene.inl"

#endif // SCENES_RESULT_SCENE_RESULT_SCENE_H_