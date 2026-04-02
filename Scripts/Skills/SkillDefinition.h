#ifndef SKILLS_SKILLDEFINITION_H_
#define SKILLS_SKILLDEFINITION_H_

#include "Characters/CharacterBase.h"
#include "Modifiers/StatusModifiers.h"
#include "Defines/Assets.h"

// スキルのレベルの最大値
constexpr static int SKILL_LVEVL_MAX{ 3 };

// スキルIDの定義
enum SkillId : unsigned char {
	None,		// なし

	/* --- バフ --- */
	Blessing,		// 祝福（HP回復）
	Rigidity,		// 剛力（攻撃力アップ）
	ReinforcingBar,	// 鉄筋（防御力アップ）
	Agile,			// 俊敏（素早さアップ）
	GoodLuck,		// 幸運（運アップ）
	Strength,		// 怪力（未実装）
	Strong,			// 強靭（未実装）
	/* --- デバフ --- */
	Intimidation,	// 威圧感（攻撃ダウン）


	//AttackDown2,
	///* --- 攻撃スキル --- */
	//AttackSkill, // 攻撃スキル
	//AttackSkillDown,
};

// キャラクターが持つスキルの情報
struct SkillState {
	// スキルID
	SkillId id;
	// タイマ（攻撃スキルのみ使用）
	int cool_timer{ 0 };
	// レベル
	int level{ 1 };

	bool operator==(const SkillState& other) const {
		// IDが同じなら同じスキルとする
		return id == other.id;
	}
	bool operator==(const SkillId& other) const {
		// IDが同じなら同じスキルとする
		return id == other;
	}
};

// スキルコンテキスト(キャラクターが持つ用)
// スキルの条件に使う情報
struct SkillContext
{
	CharacterBase* owner{ nullptr };	// 所有者
	CharacterBase* target{ nullptr };	// 相手

	// マップのマス数
	int tile_count{ 0 };
};

// 結果を格納するコンテキス
struct SkillModifierContext {
	// バフ
	Modifier buffs;
	// 敵にかけるデバフ
	Modifier target_debuffs;

	SkillModifierContext operator+(const SkillModifierContext& other) const {
		SkillModifierContext result;
		result.buffs = buffs + other.buffs;
		result.target_debuffs = target_debuffs + other.target_debuffs;
		return result;
	}
	SkillModifierContext operator+=(const SkillModifierContext& other) {
		buffs += other.buffs;
		target_debuffs += other.target_debuffs;
		return *this;
	}
};

// スキルが発動するタイミング
enum class ActivationTiming {
	Passive,		// 常時発動するスキル（条件は無し）
	Conditional,	// 条件付きで発動するスキル（ターン開始時に呼ばれる）
	Attack			// 攻撃時に発動するスキル
};

// スキルの情報
class SkillDefinition {
public:
	// 名前
	std::string name{ "" };
	// 説明
	std::string description{ "" };
	// テクスチャ
	GSuint texture{ Texture::SkillRigidity };
	// 発動タイミング
	ActivationTiming timing{ ActivationTiming::Passive };
	// バフフラグ true:バフ(所持者に作用) false:デバフ(相手に作用)
	bool is_buff_skill{ true };
	// 効果
	std::function<Modifier(const SkillContext&)> effect{ [](SkillContext skill_context) {return Modifier{}; } };
	// 発動条件
	std::function<bool(const SkillContext&)> condition{ [](SkillContext skill_context) {return true; } };
	// クールタイム（攻撃スキルのみ使用）
	int max_cool_time{ 0 };

	bool operator==(const SkillDefinition& other) const {
		// 名前が同じなら同じスキルとする
		return name == other.name;
	}
};

#endif // SKILLS_SKILLDEFINITION_H_
