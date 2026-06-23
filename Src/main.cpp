#include <GSgame.h>

#include <GSstandard_shader.h>

#include "Tweens/Tween.h"
#include "Defines/Define.h"
#include "Scenes/SceneManager.h"
#include "Systems/Mouse.h"
#include "Systems/AssetLoader.h"
#include "Systems/EffectSystem.h"
#include "Systems/LoggableRandomizer.h"

#include "Scenes/LoadScene/LoadScene.h"
#include "Scenes/TitleScene/TitleScene.h"
#include "Scenes/GamePlayScene/GamePlayScene.h"
#include "Scenes/ResultScene/ResultScene.h"

// マイゲームクラス
class MyGame final :
	public gslib::Game {
public:
	// コンストラクタ
	MyGame(int argc, char** argv) :
#ifdef _DEBUG
		gslib::Game{ INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT }
#else
		gslib::Game{ INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, true }
#endif
	{
		try {
			// 乱数の初期化
			Randomizer::initialize(true);
		}
		catch (std::exception& e) {
			this->error_log(e.what());
			should_terminate_ = true;
		}
		catch (...) {
			this->error_log("unknown error throwed.");
			should_terminate_ = true;
		}
	}

private:
	// 開始
	void start() override {
		try {
			// シェーダーの有効化
			gsInitDefaultShader();
			// 視錐台カリングの有効化
			gsEnable(GS_FRUSTUM_CULLING);

			// エフェクトの初期化
			EffectSystem::initialize();
			// マウスカーソルを非表示にする
			Mouse::show_cursor(false);

			// シーンの登録
			scene_manager_.register_scene(Scene::LoadToTitle,
				new LoadScene{ "Assets/Data/title_assets.csv", Scene::Title });
			scene_manager_.register_scene(Scene::Title, new TitleScene{});
			scene_manager_.register_scene(Scene::LoadToGamePlay,
				new LoadScene{ "Assets/Data/gameplay_assets.csv", Scene::GamePlay });
			scene_manager_.register_scene(Scene::GamePlay, new GamePlayScene{});
			scene_manager_.register_scene(Scene::Result, new ResultScene{});

			// シーンの開始
			scene_manager_.change_scene(Scene::LoadToTitle);
		}
		catch (std::exception& e) {
			this->error_log(e.what());
			should_terminate_ = true;
		}
		catch (...) {
			this->error_log("unknown error throwed.");
			should_terminate_ = true;
		}
	}

	// 更新
	void update(float delta_time) override {
		if (should_terminate_) return;

		try {
			// シーンの更新
			scene_manager_.update(delta_time);
		}
		catch (std::exception& e) {
			this->error_log(e.what());
			should_terminate_ = true;
		}
		catch (...) {
			this->error_log("unknown error throwed.");
			should_terminate_ = true;
		}
	}

	// 描画
	void draw() override {
		if (should_terminate_) return;

		try {
			// シーンの描画
			scene_manager_.draw();
		}
		catch (std::exception& e) {
			this->error_log(e.what());
			should_terminate_ = true;
		}
		catch (...) {
			this->error_log("unknown error throwed.");
			should_terminate_ = true;
		}
	}

	// 終了
	void end() override {
		// Tweenの消去
		Tween::clear();
		// スクリーンの消去
		ScreenManager::clear();
		// アセットの消去
		AssetLoader::clear();
		// エフェクトの終了
		EffectSystem::finalize();

		gsDisable(GS_FRUSTUM_CULLING);
		gsEndDefaultShader();
	}

	// 実行中フラグ
	bool is_running() override {
		return !should_terminate_ && !scene_manager_.is_game_end();
	}

	// エラーログ出力
	void error_log(const std::string& text) {
		std::ofstream file{ "Logs/error_log.txt", std::ios::app };
		if (file.is_open()) file << text << std::endl;
	}

private:
	// シーンマネージャー
	SceneManager scene_manager_;
	// 強制終了フラグ
	bool should_terminate_{ false };
};

int main(int argc, char** argv) {
	//_CrtSetBreakAlloc(NULL);
	return MyGame{ argc, argv }.run();
}