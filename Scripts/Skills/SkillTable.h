#ifndef SKILL_TABLE_H_
#define SKILL_TABLE_H_

#include "Characters/CharacterParameter.h"
#include <map>

#include "Skills/SkillDefinition.h"

//// スキルコンテキスト
//// スキルによる増減値
//struct SkillContext
//{
//	point_t hp{ 0 };
//	attack_t attack{ 0 };
//	defense_t defense{ 0 };
//	speed_t speed{ 0 };
//	luck_t luck{ 0 };
//
//	SkillContext operator+(const SkillContext& other) const {
//		SkillContext result;
//		result.hp = hp + other.hp;
//		result.attack = attack + other.attack;
//		result.defense = defense + other.defense;
//		result.speed = speed + other.speed;
//		result.luck = luck + other.luck;
//		return result;
//	}
//	SkillContext& operator+=(const SkillContext& other) {
//		SkillContext result = *this;
//		result = result + other;
//		return result;
//	}
//};

//// スキルIDの定義
//enum SkillId : unsigned char {
//	HpUp = 1, // HPアップスキル
//	AttackUp = 2, // 攻撃力アップスキル
//	DefenseUp = 3, // 防御力アップスキル
//	AttackSkill = 4, // 攻撃スキル
//	AttackDown = 5,
//	AttackDown2,
//	AttackSkillDown,
//
//};

//
//// スキルが影響を与えるステータスの種類
//enum StatType {
//	HP,
//	Attack,
//	Defense,
//};
//
//// スキルが発動するタイミング
//enum ActivationTiming {
//	Always, // 常時
//	BattleStart, // 戦闘開始時
//	TurnStart,   // ターン開始時
//	Attack // 攻撃時
//};
//
//// 条件タイプ
//enum class ConditionType {
//	// パラメータ参照
//	// マス参照
//	// 敵の種類参照
//};
//
//
//
//// スキル1つ分のデータを格納する構造体
//struct SkillData {
//	SkillID id;
//	std::string name;
//	ActivationTiming timing;
//	/*StatType affected_stat;
//	int amount;*/
//
//	// 条件
//	std::function<bool(const CharacterBase&)> condition;
//	// 上昇量
//	std::function<SkillContext(const CharacterBase&)> effect;
//
//	// スキルのコンテキスト
//	SkillContext skill_context;
//
//	// レベル
//	int level{ 0 };
//};

// スキルのデータベース
class SkillTable {
public:
	// IDからスキルデータを取得
	static const SkillDefinition* get_skill_data(SkillId id) {
		auto it = skill_database_.find(id);
		if (it != skill_database_.end()) {
			return &it->second;
		}
		// 見つからなかった場合、NULLを返す
		return nullptr;
	}

	//// StatTypeからステータス名を取得する
	//static std::string get_stat_name(StatType stat) {
	//	switch (stat) {
	//	case StatType::HP: return "体力";
	//	case StatType::Attack: return "攻撃力";
	//	case StatType::Defense: return "防御力";
	//	default: return "不明なステータス";
	//	}
	//}

public:
	// ここにすべてのスキル情報を定義する 仮の実装
	inline static const std::unordered_map<SkillId, SkillDefinition>skill_database_ = {
		// 仮の数値
		{SkillId::Rigidity,{"剛力", "自分の攻撃力上昇", Texture::SkillRigidity, ActivationTiming::Passive, true,
		[](SkillContext skill_context) {return Modifier{ 0, 3, 0, 0, 0 }; }}},

		{SkillId::Blessing,{"祝福","戦闘終了時にHP回復", Texture::SkillBlessing, ActivationTiming::Passive, true,
		[](SkillContext skill_context) {return Modifier{ 30, 0, 0, 0, 0 }; }}},

		{SkillId::ReinforcingBar,{"鋼筋", "自分の防御力上昇", Texture::SkillReinforcingBar, ActivationTiming::Passive, true,
		[](SkillContext skill_context) {return Modifier{ 0, 0, 3, 0, 0 }; }}},

		{SkillId::Agile,{"俊敏", "自分の素早さ上昇", Texture::SkillAgile, ActivationTiming::Passive, true,
		[](SkillContext skill_context) {return Modifier{ 0, 0, 0, 3, 0 }; }}},

		{SkillId::GoodLuck,{"幸運", "自分の運上昇", Texture::SkillGoodLuck, ActivationTiming::Passive, true,
		[](SkillContext skill_context) {return Modifier{ 0, 0, 0, 0, 3 }; }}},

		// デバフ系
		{SkillId::Intimidation,{"威圧感","相手の攻撃力低下", Texture::SkillIntimidation, ActivationTiming::Passive, false,
		[](SkillContext skill_context) {return Modifier{ 0, -3, 0, 0, 0}; }}},

		//{SkillId::AttackSkill ,{"攻撃スキル","通常よりも強い攻撃を放つ", Texture::SkillRigidity, ActivationTiming::Attack, true,
		//[](SkillContext skill_context) {return Modifier{ 0, (attack_t)(skill_context.owner->attack() * 0.2f), 0, 0, 0}; },}},

		//{SkillId::AttackDown2,{"デバフ_条件スキル_攻撃力低下","HPが50%以下で発動。相手の攻撃力低下", Texture::SkillRigidity, ActivationTiming::Conditional, false,
		//[](SkillContext skill_context) {return Modifier{ 0, -10, 0, 0, 0}; },
		//[](SkillContext skill_context) {return skill_context.owner->hp() / skill_context.owner->max_hp() >= 0.5f; }}},

		//{SkillId::AttackSkillDown,{"攻撃スキル（攻撃力低下）","相手の攻撃力を低下させる攻撃を放つ", Texture::SkillRigidity, ActivationTiming::Attack, false,
		//[](SkillContext skill_context) {return Modifier{ 0, -10, 0, 0, 0}; },
		//[](SkillContext skill_context) {return skill_context.owner->hp() / skill_context.owner->max_hp() >= 0.5f; }}},

	};
};

#endif // SKILL_TABLE_H_