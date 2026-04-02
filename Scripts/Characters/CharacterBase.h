#ifndef CHARACTERS_CHARACTER_BASE_H_
#define CHARACTERS_CHARACTER_BASE_H_

#include <string>

#include <GStype.h>
#include <GStransform.h>

#include "Animations/AnimatedMesh.h"
#include "Characters/CharacterParameter.h"
#include "Modifiers/StatusModifiers.h"

// キャラクター基底クラス
class CharacterBase {
public:
	CharacterBase(GSuint mesh, GSuint motion = 0, bool loop = true);
	CharacterBase(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true);
	virtual ~CharacterBase() = default;

	// 名前の取得
	const std::string& name(void) const noexcept;

	// レベルの取得
	virtual level_t level(void) const noexcept;

	// HPの取得
	virtual point_t hp(void) const noexcept;
	// 最大HPの取得
	virtual point_t max_hp(void) const noexcept;

	// 攻撃力の取得
	virtual attack_t attack(void) const noexcept;
	// 攻撃力の修正値を取得（バフとデバフ）
	virtual attack_t attack_modifier(void) const noexcept;
	// 防御力の取得
	virtual defense_t defense(void) const noexcept;
	// 防御力の修正値を取得
	virtual defense_t defense_modifier(void) const noexcept;

	// 素早さの取得
	virtual speed_t speed(void) const noexcept;
	// 素早さの修正値を取得（バフとデバフ）
	virtual speed_t speed_modifier(void) const noexcept;
	// 現在の移動速度倍率の取得
	float current_move_speed_multiplier(void) const noexcept;
	// 運の取得
	virtual luck_t luck(void) const noexcept;
	// 運の修正値を取得（バフとデバフ）
	virtual luck_t luck_modifier(void) const noexcept;

	// レベルアップ処理
	virtual void level_up(level_t amount) noexcept;

	// 回復処理
	virtual point_t heal(point_t amount) noexcept;
	// 被ダメージ処理
	virtual point_t take_damage(point_t amount) noexcept;

	// 最大HPの追加
	virtual void add_max_hp(point_t amount) noexcept;

	// 攻撃力の追加
	virtual void add_attack(attack_t amount) noexcept;
	// 防御力の追加
	virtual void add_defense(defense_t amount) noexcept;

	// 素早さの追加
	virtual void add_speed(speed_t amount) noexcept;
	// 運の追加
	virtual void add_luck(luck_t amount) noexcept;

	// 死亡処理
	virtual void die(float delta_time);
	// 死亡フラグ
	virtual bool is_dead(void) const noexcept;

	// お金の取得
	virtual gold_t gold(void) const noexcept;
	// お金の計算
	virtual gold_t calc_gold(gold_t amount) noexcept;

	// メッシュの取得
	AnimatedMesh& mesh(void) noexcept;
	// メッシュの取得
	const AnimatedMesh& mesh(void) const noexcept;

	// トランスフォームの取得
	GStransform& transform(void) noexcept;
	// トランスフォームの取得
	const GStransform& transform(void) const noexcept;

	// バフ・デバフの取得
	StatusModifiers& modifiers(void) noexcept;
	// バフ・デバフの取得
	const StatusModifiers& modifiers(void) const noexcept;

protected:
	// 名前
	std::string name_{ "" };

	// レベル
	level_t level_{ INITIAL_LEVEL };

	// HP
	point_t hp_{ INITIAL_HP };
	// 最大HP
	point_t max_hp_{ INITIAL_HP };

	// 攻撃力
	attack_t attack_{ INITIAL_ATTACK };
	// 防御力
	defense_t defense_{ INITIAL_DEFENSE };

	// 素早さ
	speed_t speed_{ INITIAL_SPEED };
	// 運
	luck_t luck_{ INITIAL_LUCK };

	// お金
	gold_t gold_{ INITIAL_GOLD };

	// アニメーションメッシュ
	AnimatedMesh mesh_;
	// トランスフォーム
	GStransform transform_;
	// バフ・デバフ
	StatusModifiers modifiers_;
};

#endif // CHARACTERS_CHARACTER_BASE_H_