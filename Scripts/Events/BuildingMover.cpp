#include "Events/BuildingMover.h"

#include "Characters/Heroes/HeroDefine.h"

/*定数*/
// 勇者 建物移動時 最小スケール
static constexpr float MOVE_MIN_SCALE{ 0.25f };

/*実装*/
// 初期化
void BuildingMover::initialize(const GSvector3& from, const GSvector3& to) {
	timer_ = HERO_DEFAULT_MOVE_TIME;
	from_position_ = from;
	to_position_ = to;
	look_rotation_ = GSquaternion::lookRotation(to - from);
};

// 接近時処理
void BuildingMover::on_approach(float delta_time, CharacterBase& character) {
	// 位置をLerp
	const float t = 1.0f - (timer_ / HERO_DEFAULT_MOVE_TIME);
	const GSvector3 elapsed = from_position_.lerp(to_position_, std::clamp(t, 0.0f, 1.0f));
	character.transform().position(elapsed);

	// 回転処理
	const float dt = delta_time * character.current_move_speed_multiplier();
	const GSquaternion rotation = GSquaternion::rotateTowards(
		character.transform().rotation(), look_rotation_, HERO_DEFAULT_TURN_SPEED * dt);
	character.transform().rotation(rotation);

	// 縮小処理
	const float scale = LERP(t, 1.0f, MOVE_MIN_SCALE);
	character.transform().localScale(GSvector3::one() * scale);

	// タイマーの減算
	timer_ -= dt;
}

// 離脱時処理
void BuildingMover::on_departure(float delta_time, CharacterBase& character) {
	// 位置をLerp
	const float t = 1.0f - (timer_ / HERO_DEFAULT_MOVE_TIME);
	const GSvector3 elapsed = from_position_.lerp(to_position_, std::clamp(t, 0.0f, 1.0f));
	character.transform().position(elapsed);

	// 回転処理
	const float dt = delta_time * character.current_move_speed_multiplier();
	const GSquaternion rotation = GSquaternion::rotateTowards(
		character.transform().rotation(), look_rotation_, HERO_DEFAULT_TURN_SPEED * dt);
	character.transform().rotation(rotation);

	// 拡大処理
	const float scale = LERP(t, MOVE_MIN_SCALE, 1.0f);
	character.transform().localScale(GSvector3::one() * scale);

	// タイマーの減算
	timer_ -= dt;
}

// 完了フラグ
bool BuildingMover::is_completed() const noexcept {
	return timer_ <= 0.0f;
}