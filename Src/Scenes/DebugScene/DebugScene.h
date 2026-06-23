//#ifndef SCENES_DEBUG_SCENE_H_
//#define SCENES_DEBUG_SCENE_H_
//
//#include "Interfaces/IScene.h"
//#include "Screens/ScreenManager.h"
//#include "Utilities/Blackboard.h"
//#include "Characters/Heroes/Hero.h"
//#include "Characters/Monsters/Monster.h"
//
//// デバッグシーンクラス
//class DebugScene final :
//	public IScene {
//public:
//	// コンストラクタ
//	DebugScene();
//
//	// 開始
//	void start(void) override;
//	// 更新
//	void update(float delta_time) override;
//	// 描画
//	void draw(void) const override;
//
//	// 終了
//	void end(void) override;
//	// 終了フラグ
//	bool is_end(void) const noexcept override { return is_end_; }
//
//	// 次シーンIDの取得
//	Scene next(void) const noexcept override { return Scene::Debug; }
//
//private:
//	// シーンの状態
//	enum SceneState {
//		Action,   // キャラクターが行動する
//		Wait,     // 次のターンまでの待機時間
//		GameOver, // 戦闘終了
//	};
//
//	// 終了フラグ
//	bool is_end_{ false };
//	// ヒーローターンか判定するフラグ
//	bool is_hero_turn_{ true };
//
//	// 現在のシーンの状態を管理
//	SceneState scene_state_{ SceneState::Action };
//
//	// ターン切り替えの待ち時間タイマー
//    float turn_wait_timer_{ 0.0f };
//	// ターン切り替え待ち時間
//	float TURN_WAIT_TIME = 10.0f;
//
//	// スクリーンマネージャー
//	ScreenManager screenManager_;
//	// コンテキスト
//	Blackboard context_;
//	Hero hero_;
//	Monster monster_;
//};
//
//#endif // SCENES_DEBUG_SCENE_H_