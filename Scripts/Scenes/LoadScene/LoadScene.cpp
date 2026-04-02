#include "Scenes/LoadScene/LoadScene.h"

#include <gslib.h>
#include <GSgame.h>

#include "Defines/Assets.h"
#include "Defines/Define.h"
#include "Systems/AssetLoader.h"

static constexpr int SPRITE_ROW{ 4 };
static constexpr int SPRITE_COLUMN{ 4 };
static constexpr int SPRITE_COUNT{ SPRITE_ROW * SPRITE_COLUMN };
static constexpr int SPRITE_SHEET_SIZE{ 1125 };
static constexpr float SPRITE_SHEET_RECT{ SPRITE_SHEET_SIZE / 4.0f };
static constexpr float SPRITE_ANIM_DELAY{ 12.5f };

static const GSvector2 HERO_ANIM_POSITION{
	static_cast<float>(INITIAL_WINDOW_WIDTH) - SPRITE_SHEET_RECT - 100,
	static_cast<float>(INITIAL_WINDOW_HEIGHT) - SPRITE_SHEET_RECT - 100
};

//static const GSvector2 BACK_GROUND_SCALE{ 1.0f, INITIAL_WINDOW_HEIGHT / 1119.0f };

/*実装*/
// コンストラクタ
LoadScene::LoadScene(const char* file_path, Scene next) noexcept :
	file_path_{ file_path }, next_{ next } {
}

// 開始
void LoadScene::start() {
	gsLoadTexture(Texture::LoadSceneBG, "Assets/UI/GamePlayScene/banner.png");
	gsLoadTexture(Texture::LoadSceneAnimation, "Assets/UI/Common/hero_walk.png");

	gslib::Game::run_thread(
		[this] { AssetLoader::load(this->file_path_); this->is_loaded_ = true; }
	);
}

// 更新
void LoadScene::update(float delta_time) {
	animation_timer_ += delta_time;
	if (animation_timer_ < SPRITE_ANIM_DELAY) return;

	sprite_index_ = sprite_index_ + 1 % SPRITE_COUNT;
	animation_timer_ = animation_timer_ - SPRITE_ANIM_DELAY;
}

// 描画
void LoadScene::draw() const {
	// 背景の描画
	gsDrawSprite2D(Texture::LoadSceneBG, NULL, NULL, NULL, NULL, NULL, 0.0f);

	// 勇者歩行アニメーションの描画
	const float row = static_cast<float>(sprite_index_ / SPRITE_ROW);
	const float column = static_cast<float>(sprite_index_ % SPRITE_COLUMN);
	const GSvector2 min{ SPRITE_SHEET_RECT * row, SPRITE_SHEET_RECT * column };
	const GSvector2 max{ SPRITE_SHEET_RECT + min.x, SPRITE_SHEET_RECT + min.y };
	const GSrect rect{ min.x, min.y, max.x, max.y };
	gsDrawSprite2D(Texture::LoadSceneAnimation, &HERO_ANIM_POSITION, &rect, NULL, NULL, NULL, 0.0f);
}

// 終了
void LoadScene::end() {
	sprite_index_ = 0;
	animation_timer_ = 0.0f;
	is_loaded_ = false;

	gsDeleteTexture(Texture::LoadSceneAnimation);
	gsDeleteTexture(Texture::LoadSceneBG);
}

// 終了フラグ
bool LoadScene::is_end() const noexcept {
	return is_loaded_;
}

// 次シーンIDの取得
Scene LoadScene::next() const noexcept {
	return next_;
}