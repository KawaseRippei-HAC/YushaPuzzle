#ifndef OPTION_SCREEN_H_
#define OPTION_SCREEN_H_

#include "Defines/Assets.h"
#include "Interfaces/IScreen.h"
#include "UI/SliderWidget.h"
#include "Screens/ScreenManager.h"
#include "UI/ButtonManager.h"
#include "Screens/OptionAssets.h"
#include "UI/NumberTexture.h"

class OptionScreen : public IScreen {
public:

	OptionScreen();
	~OptionScreen();

	// 初期化
	void initialize() override;
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw(void) const override;

private:
	// 音量スライダーの描画
	void draw_volume_slider(SliderWidget slider, const GSvector2 REFERENCE_POS, float value, GSuint name_texture, GSvector2 name_size) const;

	// スピーカーの描画
	void draw_speaker(GSvector2 pos, int volume) const;

	// 設定ファイルの読み書き
	void load_settings();
	void save_settings();

private:
	//=== スライダー共通 ====
	// 音量の初期値
	float initial_volume{ 0.5f };

	//=== BGMスライダー関連 ====
	// BGM基準位置（BGMスライダーの中心座標）
	const GSvector2 BGM_REFERENCE_POS{ WINDOW_CENTER.x, WINDOW_CENTER.y - 128 };
	// BGM音量の値
	float bgm_value_{ gsGetVolumeBGM() };
	// BGMスライダー表示位置
	GSvector2 bgm_slider_pos{ BGM_REFERENCE_POS - UI_VOLUMEGAGE_SIZE * 0.5f };

	//=== SEスライダー関連 ====
	// SE基準位置（BGMスライダーの中心座標）
	const GSvector2 SE_REFERENCE_POS{ WINDOW_CENTER.x, WINDOW_CENTER.y + 128 };
	// SE音量の値
	float se_value_{ gsGetMasterVolumeSE() };
	// SEスライダー表示位置
	GSvector2 se_slider_pos{ SE_REFERENCE_POS - UI_VOLUMEGAGE_SIZE * 0.5f };

	// BGMスライダー
	SliderWidget bgm_slider_{ bgm_value_, bgm_slider_pos, UI_NOZZLE_SIZE, UI_VOLUMEGAGE_SIZE, Texture::Nozzle, Texture::VolumeGauge, false, GSvector2{25,0} };
	// SEスライダー
	SliderWidget se_slider_{ se_value_, se_slider_pos, UI_NOZZLE_SIZE, UI_VOLUMEGAGE_SIZE, Texture::Nozzle, Texture::VolumeGauge, true, GSvector2{25,0} };

	//=== ボタン共通 ===
	// ボタンの間
	const float space_{ 200 };

	// ボタン管理クラス
	ButtonManager buttonManager_;

	// 数字テクスチャ
	static const NumberTexture number_texture_;

	// 設定ファイルのパス
	static constexpr char SETTINGS_FILE_PATH[] = "Assets/Data/volume_settings.csv";

	//== デバウンス処理用変数 ===
	// 変更があったかどうかのフラグ
	bool is_dirty_{ false };
	// 現在のタイマー
	float save_timer_{ 0.0f };
	// 操作後の保存遅延時間
	static constexpr float SAVE_DELAY{ 30.0f };
};

#endif