#ifndef INLINES_TITLE_SCENE_INL_
#define INLINES_TITLE_SCENE_INL_

#include "Scenes/TitleScene/TitleScene.h"

/*インライン関数実装*/
// 終了フラグ
inline bool TitleScene::is_end() const noexcept {
	return this->is_end_;
}

// 次シーンの取得
inline Scene TitleScene::next() const noexcept {
	return Scene::LoadToGamePlay;
}

// ゲーム終了フラグ
inline bool TitleScene::is_game_end(void) const noexcept {
	return gsGetKeyTrigger(GKEY_ESCAPE) || this->is_game_end_;
}

#endif // INLINES_TITLE_SCENE_INL_