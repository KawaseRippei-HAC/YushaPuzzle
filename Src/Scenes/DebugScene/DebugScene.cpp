//#include "Scenes/DebugScene/DebugScene.h"
//#include "Screens/PauseScreen.h"
//#include "Screens/OptionScreen.h"
//#include "Characters/Heroes/HeroAttackAction.h"
//#include "Characters/CharacterNameTable.h"
//#include <GSeasing.h>
//
//enum Context : unsigned char {
//	HeroParam,
//	EnemyParam,
//};
//
///*実装*/
//// コンストラクタ
//DebugScene::DebugScene() :
//	hero_{ &context_,CharacterID::HeroId } ,
//	monster_{&context_,CharacterID::SlimeId}
//{
//}
//
//// 開始
//void DebugScene::start() {
//	context_.set<HeroParameter*>(HeroParam, &hero_.param());
//	context_.set<CharacterParameter*>(EnemyParam, &monster_.param());
//}
//
//// 更新
//void DebugScene::update(float delta_time) {
//	// 戦闘が終了していたら、以降の処理は行わない
//	if (scene_state_ == GameOver) {
//		return;
//	}
//
//	// シーンの状態によって処理を分岐
//	switch (scene_state_) {
//	case Action:
//		// 現在のターンのキャラクターが行動する
//		if (is_hero_turn_) {
//			hero_.update(delta_time);    // 勇者の攻撃
//		} else {
//			monster_.update(delta_time); // モンスターの攻撃
//		}
//
//		// どちらかのHPが0以下になったかチェック
//		if (hero_.param().hp() <= 0 || monster_.param().hp() <= 0) {
//			// 0以下なら戦闘終了状態に移行
//			scene_state_ = GameOver;
//		} else {
//			// 行動がおわったら待ち状態に移行
//			scene_state_ = SceneState::Wait;
//			// 待ち時間を設定
//			turn_wait_timer_ = TURN_WAIT_TIME;
//		}
//		break;
//
//	case Wait:
//		// 待ち時間を更新
//		turn_wait_timer_ -= delta_time;
//		// 待ち時間が終了したら
//		if (turn_wait_timer_ <= 0.0f) {
//			// 次のキャラクターの行動に移行
//			scene_state_ = SceneState::Action;
//			// ターンを交代
//			if (!is_hero_turn_) {
//				is_hero_turn_ = true;
//			} else {
//				is_hero_turn_ = false;
//			}
//		}
//		break;
//
//	case GameOver:
//		// 何もしない
//		break;
//
//		//if (gsGetKeyTrigger(GKEY_P)) {
//		//	// ポーズ追加
//		//	screenManager_.push_screen(new PauseScreen(&screenManager_, &this->is_end_));
//		//}
//		//if (gsGetKeyTrigger(GKEY_O)) {
//		//	// オプション追加
//		//	screenManager_.push_screen(new OptionScreen(&screenManager_));
//		//}
//		//if (gsGetKeyTrigger(GKEY_BACKSPACE)) {
//		//	// 最後に追加したスクリーンの削除
//		//	screenManager_.pop_screen();
//		//}
//
//		//// 最後に追加したスクリーンの更新
//		//screenManager_.update(delta_time);
//	}
//}
//
//// 描画
//void DebugScene::draw() const {
//	gsFontParameter(GS_FONT_NORMAL, 64, "MSゴシック");
//	gsTextPos(0, 0);
//	gsDrawText("デバッグシーン");
//
//	//screenManager_.draw();
//}
//
//// 終了
//void DebugScene::end() {
//	this->is_end_ = false;
//}