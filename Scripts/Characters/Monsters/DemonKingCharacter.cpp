#include "Characters/Monsters/DemonKingCharacter.h"
#include "Characters/Monsters/DemonKingDefaultBattleStrategy.h"

/*定義*/
// 魔王オーラスケール
#define DEMON_KING_AURA_SCALE GSvector3::one() * 1.5f
// 魔王オーラ色
#define DEMON_KING_AURA_COLOR GScolor{ 1.0f, 1.0f, 1.0f, 0.75f }

/*定数*/
// 死亡アニメーションの時間
static constexpr float DEATH_ANIMATION_TIME{ 30.0f };
// 魔王オーラオフセット
static const GSvector3 DEMON_KING_AURA_OFFSET{ GSvector3::up() * 1.5f };

/*実装*/
//コンストラクタ
DemonKingCharacter::DemonKingCharacter(const MonsterData& data) :
	CharacterBase{ data.mesh_id } {
	// パラメータを初期化
	attack_ = data.params.attack;
	defense_ = data.params.defense;
	speed_ = data.params.speed;
	luck_ = data.params.luck;
	max_hp_ = data.params.max_hp;
	hp_ = data.params.max_hp;

	// トランスフォームの初期化
	transform_.lookAt(GSvector3::left());

	// エフェクトの再生
	aura_effect_.play(Effect::AuraDemonKing)
		.scale(DEMON_KING_AURA_SCALE)
		.color(DEMON_KING_AURA_COLOR);

	// 死亡フラグ
	is_dead_ = false;
	// タイマー
	timer_ = DEATH_ANIMATION_TIME;
}

// 更新
void DemonKingCharacter::update(float delta_time) {
	// メッシュの更新
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());

	// エフェクトの更新
	aura_effect_.position(transform_.position() + DEMON_KING_AURA_OFFSET);
	aura_effect_.update(delta_time);
}

// 描画
void DemonKingCharacter::draw() const {
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
attack_t DemonKingCharacter::attack() const noexcept {
	return attack_;
}

// 防御力の取得
defense_t DemonKingCharacter::defense() const noexcept {
	return defense_;
}

// 死亡処理
void DemonKingCharacter::die(float delta_time) {
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
bool DemonKingCharacter::is_dead(void) const noexcept {
	return is_dead_;
}

// 現在の経験値量を取得
int DemonKingCharacter::experience(void) const noexcept {
	return exp_;
}
// 必要経験値量を取得
int DemonKingCharacter::required_experience(void) const noexcept {
	return required_exp_;
}
// 経験値の追加
void DemonKingCharacter::add_experience(int amount) noexcept {
	exp_ += amount;
}
// 必要経験値量を追加
void DemonKingCharacter::add_required_experience(int amount) noexcept {
	required_exp_ += amount;
}
// 経験値のリセット
void DemonKingCharacter::reset_experience() noexcept {
	exp_ = 0;
}

// レベルアップ実行
void DemonKingCharacter::level_up_execute(int tile_count) {
	demon_king_level_up_.execute(tile_count);
}

// Strategyを動的に設定
void DemonKingCharacter::set_battle_strategy(std::unique_ptr<IBattleActor>strategy) {
	battle_strategy_ = strategy ? std::move(strategy) :
		std::make_unique<DemonKingDefaultBattleStrategy>(this);
}

// 現在のStrategyオブジェクトを取得
IBattleActor& DemonKingCharacter::get_battle_actor() const noexcept {
	return *battle_strategy_;
}