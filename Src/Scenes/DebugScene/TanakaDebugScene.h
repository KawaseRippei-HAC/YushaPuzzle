#ifndef SCENES_DEBUG_SCENE_TANAKA_DEBUG_SCENE_H_
#define SCENES_DEBUG_SCENE_TANAKA_DEBUG_SCENE_H_

#include "Interfaces/IScene.h"

#include "UI/ButtonManager.h"

// バトルシステムの動作確認
#include "Systems/BattleSystem.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Characters/Monsters/MonsterCharacter.h"
#include "Tiles/TileManager.h"

class TanakaDebugScene :
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
	bool is_end(void) const noexcept override { return is_end_; }

	// 次シーンIDの取得
	Scene next(void) const noexcept override { return Scene::Debug; }

private:
	// 終了フラグ
	bool is_end_{ false };

	bool is_title_{ false };

	ButtonManager buttonManager_;

	// バトルシステムの動作確認
	std::unique_ptr<TileManager> tile_{ nullptr };
	HeroCharacter* hero_;
	//MonsterCharacter* enemy_;
};

#endif // SCENES_DEBUG_SCENE_TANAKA_DEBUG_SCENE_H_