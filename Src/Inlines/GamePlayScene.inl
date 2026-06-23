#ifndef INLINES_GAME_PLAY_SCENE_INL_
#define INLINES_GAME_PLAY_SCENE_INL_

#include "Scenes/GamePlayScene/GamePlayScene.h"

/*インライン関数実装*/
// 終了フラグ
inline bool GamePlayScene::is_end() const noexcept {
	return this->is_end_;
}

// 次シーンの取得
inline Scene GamePlayScene::next() const noexcept {
	if (is_retry_) return Scene::GamePlay;
	else if (is_title_) return Scene::LoadToTitle;
	return Scene::Result;
}

#endif // INLINES_GAME_PLAY_SCENE_INL_