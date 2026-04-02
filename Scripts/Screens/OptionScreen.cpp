#include "Screens/OptionScreen.h"
#include "Systems/CsvReader.h"

#include <GSeasing.h>
#include <fstream>

// 名前とスライダーのスペース
static const GSvector2 UI_NAME_SPACE{ 150, 100 };

// スライダーと数値のスペース
static constexpr int VOLUME_LEVEL_SPACE{ 30 };

// スライダーとスピーカーのスペース
static const GSvector2 SPEAKER_SPACE{ 250, 64 };

// ボタンの基準位置（画像中央）
static const GSvector2 BACK_BTN_POS{ WINDOW_CENTER.x, 900 };

/* --- 数字 --- */
static constexpr int FONT_SIZE{ 64 };
static constexpr float FONT_SCALE{ 0.5f };
static constexpr int NUM_SPACE{ 25 };

// BGM・SEの初期値
static constexpr float DEFAULT_VOLUME{ 0.5f };

// 音量設定の位置
GSvector2 SOUND_LOGO_POS{ 300 - BTN_TEXTURE_SIZE_1024 * 0.5f, 150 - BTN_TEXTURE_SIZE_1024 * 0.5f };

// staticメンバ変数の実体定義
const NumberTexture OptionScreen::number_texture_{ Texture::Number,NUMBER_SIZE,NUMBER_SIZE };

// コンストラクタ
OptionScreen::OptionScreen() {

	// 起動時に設定ファイルを読み込んで値を適用
	load_settings();

	// BGMスライダーアタッチ
	bgm_slider_.attach([](float volume) { gsSetVolumeBGM(volume); });
	bgm_slider_.attach([=](float value) {
		bgm_value_ = value;

		// 変更があったらタイマーをリセット
		is_dirty_ = true;
		save_timer_ = SAVE_DELAY;
		});

	// SEスライダーアタッチ
	se_slider_.attach([](float volume) { gsSetMasterVolumeSE(volume); });
	se_slider_.attach([=](float value) {
		se_value_ = value;

		// 変更があったらタイマーをリセット
		is_dirty_ = true;
		save_timer_ = SAVE_DELAY;
		});

	// 戻るボタンの設定
	GSvector2 btn_size{ BTN_FONT_SIZE_57 * 5, BTN_FONT_SIZE_57 };	// ボタンサイズ
	GSvector2 btn_pos{ BACK_BTN_POS - btn_size * 0.5f };	// ボタン位置
	GSvector2 btn_space{ (BTN_TEXTURE_SIZE_512 - btn_size.x) * 0.5f, (BTN_TEXTURE_SIZE_512 - btn_size.y) * 0.5f };	// テクスチャ内のボタンまでのスペース
	ButtonWidget* back_btn = new ButtonWidget{ btn_pos, btn_size, Texture::BackSettings, btn_space };
	back_btn->attach_click([=]() {ScreenManager::pop_screen(); });

	// ボタンの登録
	buttonManager_.add_button(back_btn);
}

// デストラクタ
OptionScreen::~OptionScreen() {
	// 画面切り替え時に未保存の変更があれば強制保存してもいい
	if (is_dirty_) {
		save_settings();
	}
}

// 初期化
void OptionScreen::initialize() {

}
// 更新
void OptionScreen::update(float delta_time) {
	bgm_slider_.update(delta_time);
	se_slider_.update(delta_time);
	buttonManager_.update(delta_time);

	// デバウンス保存処理
	if (is_dirty_) {
		save_timer_ -= delta_time;
		if (save_timer_ <= 0.0f) {
			save_settings(); // 保存実行
			is_dirty_ = false;
		}
	}
}

// 描画
void OptionScreen::draw(void) const {
	// BG
	gsDrawRectangle2D(&UI_PAUSE_BG_RECT, &UI_PAUSE_BG_COLOR);

	GScolor color = { 1.0f, 1.0f, 1.0f, 1.0f };
	gsSetSpriteFontColor(&color);

	// 音量設定の描画
	gsDrawSprite2D(Texture::SoundSettingLogo, &SOUND_LOGO_POS, NULL, NULL, NULL, NULL, NULL);


	//// BGMスライダーの描画
	draw_volume_slider(bgm_slider_, BGM_REFERENCE_POS, bgm_value_, Texture::BGM, UI_BGM_SIZE);

	//// SEスライダーの描画
	draw_volume_slider(se_slider_, SE_REFERENCE_POS, se_value_, Texture::SE, UI_SE_SIZE);

	// ボタンの描画
	buttonManager_.draw();
}

void OptionScreen::draw_volume_slider(SliderWidget slider, const GSvector2 REFERENCE_POS, float value, GSuint name_texture, GSvector2 name_size) const {
	int volume = value * 100;

	// 桁数に応じて移動
	int volmue_size = std::to_string(volume).size();
	int i = 3 - volmue_size;
	// 音量の桁数に応じた座標補正値
	float volume_corrected_val = (i * FONT_SIZE) - NUM_SPACE * i;

	/* --- スライダーの描画 --- */
	slider.draw();

	/* --- 数値の描画 --- */
	//gsSetSpriteFontStyle(GS_FONT_NORMAL, FONT_SIZE, "魔導太丸ゴシック");
	GSvector2 pos = { REFERENCE_POS.x + UI_VOLUMEGAGE_SIZE.x * 0.5f + VOLUME_LEVEL_SPACE,
		REFERENCE_POS.y - FONT_SIZE * 0.5f };
	//gsDrawSpriteFont(&pos, "%d", volume);
	// 座標を桁数に応じて補正
	pos.x += volume_corrected_val;
	number_texture_.draw(pos, volume, GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }, GSvector2{ FONT_SCALE, FONT_SCALE }, -NUM_SPACE);

	/* --- スピーカーの描画 --- */
	pos = { REFERENCE_POS.x - UI_VOLUMEGAGE_SIZE.x * 0.5f, REFERENCE_POS.y };
	pos -= SPEAKER_SPACE;
	draw_speaker(pos, volume);

	/* --- 名前の描画 --- */
	pos = { REFERENCE_POS - UI_NAME_SPACE - name_size * 0.5f };
	pos.x -= UI_VOLUMEGAGE_SIZE.x * 0.5f;
	gsDrawSprite2D(name_texture, &pos, NULL, NULL, NULL, NULL, NULL);
}
void OptionScreen::draw_speaker(GSvector2 pos, int volume) const {
	// スピーカー描画
	gsDrawSprite2D(Texture::Speaker, &pos, NULL, NULL, NULL, NULL, NULL);

	GSuint texture = 0;
	GSvector2 volume_pos = pos;
	volume_pos.x += 100;
	if (volume >= 66) {
		texture = Texture::Volume_3;
	}
	else if (volume >= 33) {
		texture = Texture::Volume_2;
	}
	else if (volume > 0) {
		texture = Texture::Volume_1;
	}
	else {
		texture = Texture::Cross;
	}

	// 音量描画
	gsDrawSprite2D(texture, &volume_pos, NULL, NULL, NULL, NULL, NULL);
}

// 設定ファイルの読み込み
void OptionScreen::load_settings() {
	// ファイルが存在するか確認
	std::ifstream check_file(SETTINGS_FILE_PATH);
	if (!check_file) {
		// ファイルがない場合のデフォルト処理
		bgm_value_ = DEFAULT_VOLUME;
		se_value_ = DEFAULT_VOLUME;

		gsSetVolumeBGM(bgm_value_);
		gsSetMasterVolumeSE(se_value_);
		bgm_slider_.set_value(bgm_value_);
		se_slider_.set_value(se_value_);
		return;
	}
	check_file.close();

	// ファイルが存在する場合のみ読み込み
	CsvReader reader(SETTINGS_FILE_PATH);
	// 中身が空だった場合のガード
	if (reader.size() == 0) {
		bgm_value_ = DEFAULT_VOLUME;
		se_value_ = DEFAULT_VOLUME;
	}
	else {
		for (int i = 0; i < reader.size(); ++i) {
			std::string key = reader.get(i, 0);
			float value = reader.getf(i, 1);

			if (key == "BGM") {
				bgm_value_ = value;
				gsSetVolumeBGM(bgm_value_);
			}
			else if (key == "SE") {
				se_value_ = value;
				gsSetMasterVolumeSE(se_value_);
			}
		}
	}
	// 読み込んだ値をスライダーへ反映
	bgm_slider_.set_value(bgm_value_);
	se_slider_.set_value(se_value_);
}

// 設定ファイルの保存
void OptionScreen::save_settings() {
	std::ofstream file(SETTINGS_FILE_PATH);
	if (file) {
		file << "BGM," << bgm_value_ << std::endl;
		file << "SE," << se_value_ << std::endl;
		file.close();
	}
}
