#ifndef SYSTEMS_MOUSE_H_
#define SYSTEMS_MOUSE_H_

#include <functional>
#include <unordered_set>
#include <unordered_map>

#include <GStype.h>
#include <GSinput.h>

#include "Physics/Ray.h"

class IDragHandler;

// マウスクラス
class Mouse {
public:
	// 更新
	static void update(float delta_time);

	// 描画
	static void draw() noexcept;

	// カーソル表示の設定
	static void show_cursor(bool is_show = true) noexcept;

	// ドラッグハンドラの接続
	static void attach_drag_handler(GMOUSTATE button, IDragHandler* handler) noexcept;
	// ドラッグハンドラの解除
	static void detach_drag_handler(IDragHandler* handler) noexcept;

	// ドラッグハンドラの消去
	static void clear_drag_handlers(bool is_fullclear = false) noexcept;

	// ホイール回転量の取得
	static float wheel(void) noexcept;

	// カーソル位置の取得
	static GSvector2 position(void) noexcept;
	// カーソル移動量の取得
	static GSvector2 velocity(void) noexcept;

	// レイの取得
	static const Ray& ray(void) noexcept;

private:
	/*インスタンス生成禁止*/
	Mouse() = delete;

	// ドラッグの更新
	static void update_drag(GMOUSTATE button, std::unordered_set<IDragHandler*>& handlers) noexcept;

private:
	// ホイール回転量
	static inline float wheel_{ 0.0f };

	// カーソル位置
	static inline GSvector2 position_{ 0.0f, 0.0f };
	// カーソル移動量
	static inline GSvector2 velocity_{ 0.0f, 0.0f };

	// レイ
	static inline Ray ray_{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	// ドラッグハンドラマップ
	static inline std::unordered_map<GMOUSTATE, std::unordered_set<IDragHandler*>> handlers_map_;
};

#include "Inlines/Mouse.inl"

#endif // SYSTEMS_MOUSE_H_