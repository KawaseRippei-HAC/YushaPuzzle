#ifndef CAMERAS_CINEMACHINE_H_
#define CAMERAS_CINEMACHINE_H_

#include <memory>
#include <type_traits>

#include <GStype.h>
#include <GSeffect.h>
#include <GStransform.h>

#include "Defines/Assets.h"
#include "Interfaces/IVirtualCamera.h"

// シネマシーンクラス
class Cinemachine {
public:
	Cinemachine() = default;

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// バーチャルカメラの設定
	template<class TCamera, typename ...Args>
		requires std::is_base_of_v<IVirtualCamera, TCamera>&& std::is_constructible_v<TCamera, Args...>
	void set(Args&& ...args);

	// バーチャルカメラの変更
	template<class TCamera, typename ...Args>
		requires std::is_base_of_v<IVirtualCamera, TCamera>&& std::is_constructible_v<TCamera, Args...>
	void change(float duration, Args&& ...args);

private:
	// エフェクトの再生
	void play_effet(void);

protected:
	// トランスフォーム
	GStransform transform_;

	// 補間時間
	float duration_{ 0.0f };
	// 経過時間
	float elapsed_{ 0.0f };
	// 現在のバーチャルカメラ
	std::unique_ptr<IVirtualCamera> current_;
	// 補間先のバーチャルカメラ
	std::unique_ptr<IVirtualCamera> to_;

	// 集中線エフェクトハンドル
	GSint effect_{ 0 };
};

/*テンプレート関数実装*/
// バーチャルカメラの更新
template<class TCamera, typename ...Args>
	requires std::is_base_of_v<IVirtualCamera, TCamera>&& std::is_constructible_v<TCamera, Args...>
void Cinemachine::set(Args&& ...args) {
	to_.reset(nullptr);
	elapsed_ = duration_ = 0.0f;
	current_ = std::make_unique<TCamera>(std::forward<Args>(args)...);
}

// バーチャルカメラの変更
template<class TCamera, typename ...Args>
	requires std::is_base_of_v<IVirtualCamera, TCamera>&& std::is_constructible_v<TCamera, Args...>
void Cinemachine::change(float duration, Args&& ...args) {
	if (elapsed_ < duration_) gsStopEffect(effect_);
	elapsed_ = 0.0f;
	duration_ = duration;
	to_ = std::make_unique<TCamera>(std::forward<Args>(args)...);
	this->play_effet();
}

#endif // CAMERAS_CINEMACHINE_H_
