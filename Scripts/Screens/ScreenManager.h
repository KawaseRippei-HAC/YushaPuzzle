#ifndef SCREEN_MANAGER_H_
#define SCREEN_MANAGER_H_

#include "Interfaces/IScreen.h"

// スクリーンのID
enum class ScreenID {
	Pause,				// ポーズ画面
	SoundOption,		// 音量設定画面
	Status,				// ステータス画面
	StatusAllocation,	// ステータス振り分け画面
	SkillSelect,		// スキル選択画面
	BuildingExplanation,// 建物の説明画面
	RuleExplanation,	// ルール説明画面
};

class ScreenManager {
public:

	// 更新
	static void update(float delta_time);
	// 描画
	static void draw(void);
	// スクリーンの登録
	static void register_screen(ScreenID id, IScreen* screen);
	// スクリーンの追加
	static void push_screen(ScreenID id);
	// 最後に追加したスクリーンの削除
	static void pop_screen();
	// スタックの消去
	static void stack_clear();
	// マップ消去
	static void map_clear();
	// 全て消去
	static void clear();
	//スクリーンが空か
	static bool is_empty();

private:
	// インスタンス化禁止
	ScreenManager() = delete;

	// 一番上の初期化処理を実行したか
	inline static bool is_top_initialize_{ false };

	// スクリーンスタック
	inline static std::stack<IScreen*> screen_stack_;

	// スクリーンマップ
	inline static std::map<ScreenID, IScreen*> screen_map_;
};

#endif
