#include "Characters/Monsters/MidBossCharacter.h"
#include "Characters/Monsters/MidBossDefaultBattleStrategy.h"

/*定義*/
// 強敵オーラスケール
#define MID_BOSS_AURA_SCALE GSvector3::one() * 1.5f
// 強敵オーラ色
#define MID_BOSS_AURA_COLOR GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }

/*定数*/
// 死亡アニメーションの時間
static constexpr float MID_BOSS_DEATH_ANIMATION_TIME{ 30.0f };
// 強敵オーラオフセット
static const GSvector3 MID_BOSS_AURA_OFFSET{ GSvector3::up() * 1.5f };

/*実装*/
//コンストラクタ
MidBossCharacter::MidBossCharacter(const MonsterData& data) :
	CharacterBase{ data.mesh_id } {
	// パラメータを初期化
	attack_ = data.params.attack;
	defense_ = data.params.defense;
	speed_ = data.params.speed;
	luck_ = data.params.luck;
	max_hp_ = data.params.max_hp;
	hp_ = data.params.max_hp;

	// エフェクトコンテキストの初期化
	effect_context_ = data.effect;

	// 獲得経験値を初期化
	drop_exp_ = data.drop_exp;

	// トランスフォームの初期化
	transform_.lookAt(GSvector3::left());

	// エフェクトの再生
	aura_effect_.play(Effect::AuraMidBoss)
		.scale(MID_BOSS_AURA_SCALE)
		.color(MID_BOSS_AURA_COLOR);

	// 死亡フラグ
	is_dead_ = false;
	// タイマー
	timer_ = MID_BOSS_DEATH_ANIMATION_TIME;
}

// 更新
void MidBossCharacter::update(float delta_time) {
	// メッシュの更新
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());
	// エフェクトの更新
	aura_effect_.position(transform_.position() + MID_BOSS_AURA_OFFSET);
	aura_effect_.update(delta_time);
}

// 描画
void MidBossCharacter::draw() const {
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
attack_t MidBossCharacter::attack() const noexcept {
	return attack_;
}

// 防御力の取得
defense_t MidBossCharacter::defense() const noexcept {
	return defense_;
}

// 獲得経験値の取得
int MidBossCharacter::drop_exp(void) const noexcept {
	return drop_exp_;
}

// Strategyを動的に設定
void MidBossCharacter::set_battle_strategy(std::unique_ptr<IBattleActor>strategy) {
	battle_strategy_ = strategy ? std::move(strategy) :
		std::make_unique<MidBossDefaultBattleStrategy>(this);
}

// 現在のStrategyオブジェクトを取得
IBattleActor& MidBossCharacter::get_battle_actor() const noexcept {
	return *battle_strategy_;
}

// 死亡処理
void MidBossCharacter::die(float delta_time) {
	// タイマーの減算
	timer_ -= delta_time;
	// 縮小処理
	const float t = 1.0f - (timer_ / 30);
	const GSvector3 scale = GSvector3::one() * LERP(t, 1.0f, 0.0f);
	this->transform().localScale(scale);
	aura_effect_.scale(scale);

	// モーションが終了したら、死亡フラグを立てる
	if (timer_ <= 0) {
		is_dead_ = true;
		aura_effect_.stop();
	}
}
// 死亡フラグ
bool MidBossCharacter::is_dead(void) const noexcept {
	return is_dead_;
}

// モンスターエフェクトコンテキストの取得
MonsterEffectContext MidBossCharacter::effect_context() const {
	return effect_context_;
}
