#include "Events/FallInAbyssEvent.h"

#include <GSeasing.h>

#include "Characters/Heroes/HeroDefine.h"

/*定義*/
// 勇者落下地点オフセット
#define HERO_FALL_OFFSET GSvector3::down() * TILE_SIZE

/*実装*/
// 実行
void FallInAbyssEvent::execute(float delta_time, CharacterBase& character) {
	character.die(delta_time);
	is_finished_ = true;
}

// 終了フラグ
bool FallInAbyssEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動処理の取得
ICharacterMover* FallInAbyssEvent::mover() noexcept {
	return &fall_in_abyss_mover_;
}

// 移動中有効フラグ
bool FallInAbyssEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	return true;
}

// 接近の初期化
void FallInAbyssEvent::FallInAbyssMover::init_approach(const GSvector3& from, const GSvector3& to) {
	timer_ = HERO_DEFAULT_MOVE_TIME;
	from_position_ = from;
	to_position_ = to + HERO_FALL_OFFSET;
	look_rotation_ = GSquaternion::lookRotation(to - from);
}

// 接近時処理
void FallInAbyssEvent::FallInAbyssMover::on_approach(float delta_time, CharacterBase& character) {
	const float t = 1.0f - timer_ / HERO_DEFAULT_MOVE_TIME;
	const GSvector3 position = GSvector3::slerp(from_position_, to_position_, std::clamp(t, 0.0f, 1.0f));
	character.transform().position(position);

	// 回転処理
	const GSquaternion rotation = GSquaternion::rotateTowards(
		character.transform().rotation(), look_rotation_, HERO_DEFAULT_TURN_SPEED * delta_time);
	character.transform().rotation(rotation);

	// タイマーの減算
	timer_ -= delta_time;
}

// 完了フラグ
bool FallInAbyssEvent::FallInAbyssMover::is_completed() const noexcept {
	return timer_ <= 0.0f;
}