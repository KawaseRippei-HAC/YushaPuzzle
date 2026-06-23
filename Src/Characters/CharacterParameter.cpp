#include "Characters/CharacterParameter.h"
#include "Characters/CharacterNameTable.h"
#include <algorithm>

// コンストラクタ
CharacterParameter::CharacterParameter(int initial_id) :
	id_{ initial_id },
	name_{ CharacterNameTable::get_name(initial_id) }
{
}

// 被ダメージ処理
point_t CharacterParameter::take_damage(attack_t power)noexcept {
	// ダメージ = (攻撃者の合計攻撃力) - ((自身の合計防御力) / 防御力補正)
	const point_t damage = std::max(0, power - this->total_defense() / defense_adj_);

	// HPを減らす
	hp_ = std::max(0, hp_ - damage);

	return damage;
}

// 戦闘開始時にステータスを初期化する
void CharacterParameter::reset_battle_stats() {
	battle_hp_ = hp_;
	battle_attack_ = attack_;
	battle_defense_ = defense_;
}