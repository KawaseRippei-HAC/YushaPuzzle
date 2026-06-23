#include "Modifiers/StatusModifiers.h"
#include "Systems/LoggableRandomizer.h"

// デバフ解除の確率
const float REMOVAL_RATE{ 0.005f };
// デバフ解除の最大確率
const float REMOVAL_MAX_RATE{ 0.7f };

// バフ追加
void StatusModifiers::add_buff(Modifier& buff) {
	buffs_ += buff;
}
// デバフ追加
void StatusModifiers::add_debuff(Modifier& debuff) {
	debuffs_ += debuff;
}
// バフの設定
void StatusModifiers::set_buff(const Modifier& buff) {
	buffs_ = buff;
}
// デバフの設定
void StatusModifiers::set_debuff(const Modifier& debuff) {
	debuffs_ = debuff;
}

// デバフを判定
void StatusModifiers::check_debuffs_removal(luck_t luck) {

	// 運1で0.5% 最大70%
	float rate = luck * REMOVAL_RATE;
	// 最大値を超えないようにクランプ
	CLAMP(rate, 0.0f, REMOVAL_MAX_RATE);

	// 確率でデバフ解除
	if (LoggableRandomizer::generate(0.0f, 1.0f) <= rate) {
		// デバフを全て解除
		debuffs_ = Modifier{ 0, 0, 0, 0, 0 };
	}
	// 解除出来なかった場合は何もしない
}

// バフとデバフの合計値を取得
Modifier StatusModifiers::get_modifiers() const {
	return buffs_ + debuffs_;
}
