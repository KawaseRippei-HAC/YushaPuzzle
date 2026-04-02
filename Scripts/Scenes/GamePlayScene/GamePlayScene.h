#ifndef SCENES_GAME_PLAY_SCENE_GAME_PLAY_SCENE_H_
#define SCENES_GAME_PLAY_SCENE_GAME_PLAY_SCENE_H_

#include "UI/HeroUI.h"
#include "UI/ButtonManager.h"
#include "Interfaces/IScene.h"
#include "Systems/GameManager.h"
#include "Screens/ScreenManager.h"
#include "Cameras/DraggableCamera.h"

// ゲームプレイシーンクラス
class GamePlayScene final :
	public IScene {
public:
	// コンストラクタ
	GamePlayScene();

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
	// タイトルフラグ
	bool is_title_{ false };
	// リトライフラグ
	bool is_retry_{ false };

	// ゲームマネージャー
	GameManager game_manager_;

	// 勇者UI表示クラス
	std::unique_ptr<HeroUI> hero_ui_{ nullptr };

	// 背景位置
	const GSvector2	banner_pos_{ 0,1080.0f - 256.0f };
	const GSrect banner_rect_{ 0.0f,0.0f,1920.0f,256.0f, };

	// ボタン管理クラス
	ButtonManager buttonManager_;
};

#include "Inlines/GamePlayScene.inl"

#endif // SCENES_GAME_PLAY_SCENE_GAME_PLAY_SCENE_H_
