#ifndef CHARACTERS_CHARACTER_PARAMETER_H_
#define CHARACTERS_CHARACTER_PARAMETER_H_

#include <string>
#include <vector>

/*エイリアス宣言*/
using name_t = char;
using level_t = char;
using point_t = short int;
using attack_t = short int;
using defense_t = short int;
using speed_t = short int;
using luck_t = short int;
using magic_id_t = unsigned char;
using experience_t = unsigned short int;
using gold_t = int;

/*定数*/
// 初期名前のID
inline constexpr name_t INITIAL_NAME_ID{ 0 };

// 初期レベル
inline constexpr level_t INITIAL_LEVEL{ 1 };
// 最大レベル
inline constexpr level_t MAX_LEVEL{ 100 };

// 初期HP
inline constexpr point_t INITIAL_HP{ 100 };
// 最大HP
inline constexpr point_t MAX_HP{ 10000 };

// 初期攻撃力
inline constexpr attack_t INITIAL_ATTACK{ 10 };
// 最大攻撃力
inline constexpr attack_t MAX_ATTACK{ 1000 };

// 初期防御力
inline constexpr defense_t INITIAL_DEFENSE{ 10 };
// 最大防御力
inline constexpr defense_t MAX_DEFENSE{ 1000 };
// 防御力補正
inline constexpr defense_t DEFENSE_ADJUSTMENT{ 10 };

// 初期MP
inline constexpr point_t INITIAL_MP{ 100 };
// 最大MP
inline constexpr point_t MAX_MP{ 10000 };

// 初期魔法攻撃力
inline constexpr attack_t INITIAL_MAGIC_ATTACK{ 10 };
// 最大魔法攻撃力
inline constexpr attack_t MAX_MAGIC_ATTACK{ 100 };

// 初期魔法防御力
inline constexpr defense_t INITIAL_MAGIC_DEFENSE{ 10 };
// 最大魔法防御力
inline constexpr defense_t MAX_MAGIC_DEFENSE{ 100 };

// 初期素早さ
inline constexpr speed_t INITIAL_SPEED{ 10 };
// 最大素早さ
inline constexpr speed_t MAX_SPEED{ 1000 };

// 初期運
inline constexpr luck_t INITIAL_LUCK{ 10 };
// 最大運
inline constexpr luck_t MAX_LUCK{ 1000 };

// 初期お金
inline constexpr gold_t INITIAL_GOLD{ 0 };
static constexpr gold_t MAX_GOLD{ 999999 };

// キャラクター共通パラメータークラス
class CharacterParameter {
public:
	// コンストラクタでIDを受け取る
	CharacterParameter(int initial_id);
	CharacterParameter() = default;

	// 名前の取得
	const std::string& name(void) const noexcept;
	// レベルの取得
	level_t level(void) const noexcept;

	// HPの取得
	point_t hp(void) const noexcept;
	// 最大HPの取得
	point_t max_hp(void) const noexcept;
	// 攻撃力の取得
	attack_t attack(void) const noexcept;
	// 防御力の取得
	defense_t defense(void) const noexcept;

	// MPの取得
	point_t mp(void) const noexcept;
	// 最大MPの取得
	point_t max_mp(void) const noexcept;
	// 魔法攻撃力の取得
	attack_t magic_attack(void) const noexcept;
	// 魔法防御力の取得
	defense_t magic_defense(void) const noexcept;

	// 素早さの取得
	speed_t speed(void) const noexcept;
	// 運の取得
	luck_t luck(void) const noexcept;

	// 被ダメージ処理
	virtual point_t take_damage(attack_t power) noexcept;

	// 合計攻撃力の取得
	virtual attack_t total_attack(void) const noexcept;
	// 合計防御力の取得
	virtual defense_t total_defense(void) const noexcept;

	// 戦闘開始時にステータスを初期化する
	void reset_battle_stats();

protected:
	// 名前
	int id_{ INITIAL_NAME_ID };
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

	// MP
	point_t mp_{ INITIAL_MP };
	// 最大MP
	point_t max_mp_{ INITIAL_MP };
	// 魔法攻撃力
	attack_t magic_attack_{ INITIAL_MAGIC_ATTACK };
	// 魔法防御力
	defense_t magic_defense_{ INITIAL_MAGIC_DEFENSE };

	// 素早さ
	speed_t speed_{ INITIAL_SPEED };
	// 運
	luck_t luck_{ INITIAL_LUCK };

	// 防御力補正
	defense_t defense_adj_{ DEFENSE_ADJUSTMENT };

	// 戦闘中、スキルなどで変動したHP
	point_t battle_hp_{ 0 };
	// 戦闘中、スキルなどで変動した攻撃力
	attack_t battle_attack_{ 0 };
	// 戦闘中、スキルなどで変動した攻撃力
	defense_t battle_defense_{ 0 };

	// 所持魔法配列
	std::vector<magic_id_t> magics_;
};

#include "Inlines/CharacterParameter.inl"

#endif // CHARACTERS_CHARACTER_PARAMETER_H_