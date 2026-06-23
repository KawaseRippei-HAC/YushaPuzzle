#ifndef INLINES_RESULT_SCENE_INL_
#define INLINES_RESULT_SCENE_INL_

#include "Scenes/ResultScene/ResultScene.h"

/*インライン関数実装*/
// 終了フラグ
inline bool ResultScene::is_end() const noexcept {
	return this->is_end_;
}

// 次シーンの取得
inline Scene ResultScene::next() const noexcept {
	switch (next_scene_) {
	case NextScene::None: return Scene::LoadToTitle;
	case NextScene::GamePlay: return Scene::GamePlay;
	case NextScene::StageSelect: return Scene::LoadToTitle;
	default: return Scene::LoadToTitle;
	}
}

#endif // INLINES_RESULT_SCENE_INL_