#ifndef MODIFIERS_STATUS_MODIFIERS_H_
#define MODIFIERS_STATUS_MODIFIERS_H_

#include"Characters/CharacterParameter.h"

// スキル修正値のコンテキスト
// スキルによる増減値を返す
struct Modifier
{
	point_t hp{ 0 };
	attack_t attack{ 0 };
	defense_t defense{ 0 };
	speed_t speed{ 0 };
	luck_t luck{ 0 };

	Modifier operator+(const Modifier& other) const {
		Modifier result;
		result.hp = hp + other.hp;
		result.attack = attack + other.attack;
		result.defense = defense + other.defense;
		result.speed = speed + other.speed;
		result.luck = luck + other.luck;
		return result;
	}
	Modifier& operator+=(const Modifier& other) {
		hp += other.hp;
		attack += other.attack;
		defense += other.defense;
		speed += other.speed;
		luck += other.luck;

		return *this;
	}
	Modifier operator*(const int value) {
		Modifier result;
		result.hp = hp * value;
		result.attack = attack * value;;
		result.defense = defense * value;;
		result.speed = speed * value;;
		result.luck = luck * value;
		return result;
	}
};

// バフ・デバフを管理するクラス
class StatusModifiers {
public:
	// バフ追加
	void add_buff(Modifier& buff);
	// デバフ追加
	void add_debuff(Modifier& debuff);
	// バフの設定
	void set_buff(const Modifier& buff);
	// デバフの設定
	void set_debuff(const Modifier& debuff);
	// デバフの解除
	void check_debuffs_removal(luck_t luck);

	// バフとデバフの合計値を取得
	Modifier get_modifiers() const;
private:
	// 持っているバフとデバフ
	Modifier buffs_;
	Modifier debuffs_;
};

#endif // MODIFIERS_STATUS_MODIFIERS_H_
