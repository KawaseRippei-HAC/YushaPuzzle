#ifndef SCENES_SCENE_ID_H_
#define SCENES_SCENE_ID_H_

// シーンID
enum class Scene : unsigned char {
	// Nullシーン
	Null = 0,

	// デバッグシーン
	Debug,

	// ロードシーン
	LoadToTitle,
	// ロードシーン
	LoadToGamePlay,

	// タイトルシーン
	Title,
	// ゲームプレイシーン
	GamePlay,
	// リザルトシーン
	Result,
};

#endif // SCENES_SCENE_ID_H_