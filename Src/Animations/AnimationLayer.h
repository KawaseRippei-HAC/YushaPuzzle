#ifndef ANIMATIONS_ANIMATION_LAYER_H_
#define ANIMATIONS_ANIMATION_LAYER_H_

#include <vector>
#include <memory>
#include <functional>

#include <GStype.h>
#include <GSgraphics.h>

#include "Animations/AnimationEvent.h"

/*定数*/
// 標準補完時間
static constexpr float DEFAULT_LERP_TIME{ 10.0f };

// アニメーションレイヤークラス
class AnimationLayer final {
private:
	// アニメーションイベント
	using Event = AnimationEvent;
public:
	// コンストラクタ
	AnimationLayer(GSuint animation = 0, GSuint motion = 0, bool loop = true);

	// 更新
	void update(float delta_time);

	// イベントの追加
	void add_event(
		GSuint target_motion, float trigger_time, const std::function<void(void)>& event_callback);
	// イベントの消去
	void clear_event(GSuint target_motion);

	// モーションの変更
	void change_motion(GSuint motion, bool loop = true, float lerp_time = DEFAULT_LERP_TIME);
	// モーションの強制変更
	void forced_change_motion(GSuint motion, bool loop = true, float lerp_time = DEFAULT_LERP_TIME);

	// 現在のモーションの取得
	GSuint current_motion(void) const noexcept;

	// モーション再生時間の取得
	float motion_time(void) const noexcept;
	// モーション再生時間の設定
	void motion_time(float time) noexcept;

	// モーション速度の設定
	void motion_speed(float scale) noexcept;

	// モーション終了時間の取得
	float motion_end_time(void) const noexcept;
	// モーション終了フラグ
	bool is_motion_finished(void) const noexcept;

	// ローカル用ボーン変換行列の取得
	GSmatrix4 local_bone_matrix(GSuint bone_no) const;
	// ボーン数の取得
	int count_bone(void) const noexcept;

private:
	// アニメーションID
	GSuint animation_{ 0 };

	// モーション
	GSuint motion_{ 0 };
	// モーションタイマー
	float motion_timer_{ 0.0f };
	// 前回のモーション
	GSuint prev_motion_{ 0 };
	// 前回のモーションタイマー
	float prev_motion_timer_{ 0.0f };

	// モーションループフラグ
	bool is_looping_{ true };

	// モーション速度
	float motion_speed_{ 1.0f };

	// 補間時間
	float lerp_time_{ DEFAULT_LERP_TIME };
	// 補間タイマー
	float lerp_timer_{ 0.0f };

	std::vector<std::unique_ptr<Event>> events_;
};

#include "Inlines/AnimationLayer.inl"

#endif // ANIMATIONS_ANIMATION_LAYER_H_