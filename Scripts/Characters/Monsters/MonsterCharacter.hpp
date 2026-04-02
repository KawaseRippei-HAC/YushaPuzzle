#ifndef CHARACTERS_MONSTERS_MONSTER_CHARACTER_HPP_
#define CHARACTERS_MONSTERS_MONSTER_CHARACTER_HPP_

#include "Characters/Monsters/MonsterCharacter.h"

// 死亡アニメーションの時間
static constexpr float DEATH_ANIMATION_TIME{ 30.0f };

/*実装*/
// コンストラクタ
template<class TBattleStrategy, MonsterData data, level_t min_level>
MonsterCharacter<TBattleStrategy, data, min_level>::MonsterCharacter() :
	MonsterBase{ data.mesh_id } {

	// パラメータをテンプレート引数の定数データから初期化
	this->attack_ = data.params.attack;
	this->defense_ = data.params.defense;
	this->speed_ = data.params.speed;
	this->luck_ = data.params.luck;
	this->max_hp_ = data.params.max_hp;
	this->hp_ = data.params.max_hp;

	// 獲得経験値を初期化
	this->drop_exp_ = data.drop_exp;

	// Strategyの生成
	battle_strategy_ = std::make_unique<TBattleStrategy>(this);

	// 死亡フラグ
	is_dead_ = false;
	// タイマー
	timer_ = DEATH_ANIMATION_TIME;
}

// 更新
template<class TBattleStrategy, MonsterData data, level_t min_level>
void MonsterCharacter<TBattleStrategy, data, min_level>::update(float delta_time) {
	// メッシュの更新
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());
}

// 描画
template<class TBattleStrategy, MonsterData data, level_t min_level>
void MonsterCharacter<TBattleStrategy, data, min_level>::draw() const {
	// メッシュの描画
	mesh_.draw();

#ifdef _DEBUG
	glPushMatrix();
	{
		const GSvector3 pos = transform_.position();
		glTranslatef(pos.x, pos.y, pos.z);
		glutWireSphere(0.5f, 8, 8);
	}
	glPopMatrix();
#endif
}

// 攻撃力の取得
template<class TBattleStrategy, MonsterData data, level_t min_level>
attack_t MonsterCharacter<TBattleStrategy, data, min_level>::attack() const noexcept {
	return attack_;
}

// 防御力の取得
template<class TBattleStrategy, MonsterData data, level_t min_level>
defense_t MonsterCharacter<TBattleStrategy, data, min_level>::defense() const noexcept {
	return defense_;
}

// Strategyを動的に設定
template<class TBattleStrategy, MonsterData data, level_t min_level>
void MonsterCharacter<TBattleStrategy, data, min_level>::set_battle_strategy(std::unique_ptr<IBattleActor> strategy) {
	// strategyがnullの場合は、デフォルト（TBattleStrategy）で再生成
	battle_strategy_ = strategy ? std::move(strategy) : std::make_unique<TBattleStrategy>(this);
}

// 現在のStrategyオブジェクトを取得
template<class TBattleStrategy, MonsterData data, level_t min_level>
IBattleActor& MonsterCharacter<TBattleStrategy, data, min_level>::get_battle_actor() const noexcept {
	return *battle_strategy_;
}

// 獲得経験値の取得
template<class TBattleStrategy, MonsterData data, level_t min_level>
int MonsterCharacter<TBattleStrategy, data, min_level>::drop_exp(void) const noexcept {
	return drop_exp_;
}

// 死亡処理
template<class TBattleStrategy, MonsterData data, level_t min_level>
void MonsterCharacter<TBattleStrategy, data, min_level>::die(float delta_time) {
	// タイマーの減算
	timer_ -= delta_time;
	// 縮小処理
	const float t = 1.0f - (timer_ / 30);
	const float scale = LERP(t, 1.0f, 0.0f);
	this->transform().localScale(GSvector3::one() * scale);

	// モーションが終了したら、死亡フラグを立てる
	if (timer_ <= 0) {
		is_dead_ = true;
	}
}
// 死亡フラグ
template<class TBattleStrategy, MonsterData data, level_t min_level>
bool MonsterCharacter<TBattleStrategy, data, min_level>::is_dead(void) const noexcept {
	return is_dead_;
}

#endif // CHARACTERS_MONSTERS_MONSTER_CHARACTER_HPP_