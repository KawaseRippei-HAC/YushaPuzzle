#ifndef INTERFACES_ISCENE_H_
#define INTERFACES_ISCENE_H_

#include <GSinput.h>

#include "Scenes/SceneID.h"

// シーン抽象インターフェース
class IScene {
public:
	virtual ~IScene(void) = default;

	// 開始
	virtual void start(void) = 0;

	// 更新
	virtual void update(float delta_time) = 0;
	// 描画
	virtual void draw(void) const = 0;

	// 終了
	virtual void end(void) = 0;
	// 終了フラグ
	virtual bool is_end(void) const noexcept = 0;

	// 次シーンIDの取得
	virtual Scene next(void) const noexcept = 0;

	// ゲーム終了フラグ
	virtual bool is_game_end(void) const noexcept {
		return gsGetKeyState(GKEY_ESCAPE)
			|| (gsXBoxPadButtonState(0, GS_XBOX_PAD_START) && gsXBoxPadButtonState(0, GS_XBOX_PAD_BACK));
	}
};

#endif // INTERFACES_ISCENE_H_