#ifndef CHARACTERS_HEROES_HERO_CONTEXT_H_
#define CHARACTERS_HEROES_HERO_CONTEXT_H_

#include "Tiles/TileManager.h"
#include "Animations/AnimatedMesh.h"

// 勇者コンテキスト構造体
struct HeroContext {
	// パス
	std::stack<TileNode*> path;
	// タイルグラフ
	TileGraph* graph{ nullptr };
	// 現在のタイル
	TileNode* current_tile{ nullptr };
	// 勇者キャラクター
	class HeroCharacter* character{ nullptr };
	// 行動中フラグ
	bool is_actioning{ false };
};

#endif // CHARACTERS_HEROES_HERO_CONTEXT_H_