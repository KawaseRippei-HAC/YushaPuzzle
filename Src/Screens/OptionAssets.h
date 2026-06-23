#ifndef SCREENS_OPTION_ASSETS_H_
#define SCREENS_OPTION_ASSETS_H_

/* 共通 */

// ポーズボタン
static constexpr int PAUSE_KEY{ GKEY_TAB };

// 画面サイズ
static const GSvector2 WINDOW_SIZE{ 1920, 1080 };
// 画面中央
static const GSvector2 WINDOW_CENTER{ WINDOW_SIZE * 0.5f };

// BG
static const GScolor UI_PAUSE_BG_COLOR{ 0, 0, 0, 0.7f };	// ポーズ時の背景の色
static const float UI_PAUSE_BG_OUTSIDE{ 70 };				// ポーズ時の背景の外周
static const GSrect UI_PAUSE_BG_RECT{ UI_PAUSE_BG_OUTSIDE, UI_PAUSE_BG_OUTSIDE, WINDOW_SIZE.x - UI_PAUSE_BG_OUTSIDE, WINDOW_SIZE.y - UI_PAUSE_BG_OUTSIDE };

/* --- 音量設定 UIサイズ ---  */
static const GSvector2 UI_SOUNDSETTING_SIZE{ 1024, 1024 };
static const GSvector2 UI_BGM_SIZE{ 256, 256 };
static const GSvector2 UI_SE_SIZE{ 256, 256 };
static const GSvector2 UI_NOZZLE_SIZE{ 43, 41 };
static const GSvector2 UI_VOLUMEGAGE_SIZE{ 714- 25 * 2, 48 };

/* --- スキルレベル関連 --- */
// 基準位置とスキルレベルの間隔
static const GSvector2 SKILL_LEVEL_SPACE{ 90, 90 };
// スキルレベルのスケール
static const GSvector2 SKILL_LEVEL_SCALE{ 0.6f, 0.6f };
// スキルレベルのスケール
static constexpr float SKILL_LEVEL_TEXTURE_SPACE{ 16.0f };
// スキルレベルテクスチャのサイズ
static constexpr float SKILL_LEVEL_TEX_SIZE{ 64.0f };

/* --- ポップアップ --- */
// フレームサイズ
static const GSvector2 POP_UP_FRAME_SIZE{ 500, 100 };
// フレームの幅
static constexpr float POP_UP_FRAME_WIDTH{ 5.0f };
// フレームカラー
static const GScolor POP_UP_FRAME_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
// 背景カラー
static const GScolor POP_UP_BG_COLOR{ 0.0f, 0.0f, 0.0f, 1.0f };
// フレームとスキル名の間隔
static constexpr int FRAME_NAME_SPACE{ 10 };
// スキルの名前と説明の間隔
static constexpr int POP_UP_NAME_DESCRIPTION_SPACE{ 45 };
// スキル名のフォントサイズ
static constexpr int POP_UP_SKILL_NAME_FONT_SIZE{ 30 };
// スキル説明のフォントサイズ
static constexpr int POP_UP_SKILL_DESCRIPTION_FONT_SIZE{ 25 };

// 一文字のサイズ
static constexpr int NUMBER_SIZE{ 128 };

#endif
