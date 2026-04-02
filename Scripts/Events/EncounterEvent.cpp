#include "Events/EncounterEvent.h"

#include <gslib.h>

#include "Tiles/TileManager.h"
#include "Events/EncountMover.h"
#include "Events/EvecntContext.h"
#include "Systems/BattleSystem.h"
#include "Characters/Heroes/HeroDefine.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Characters/Monsters/MonsterPool.h"
#include "Characters/Monsters/MonsterDefaultBattleStrategy.h"

/*定数*/
// 遭遇率の初期値
static constexpr float ENCOUNTER_RATE{ 0.3322f };
// 遭遇率の最大値
static constexpr float MAX_ENCOUNTER_RATE{ 0.6f };
// 遭遇率の運修正値
static constexpr float ENCOUNTER_RATE_MODIFIER{ 0.0089f };
// 遭遇率の修正値倍率（運いくつで倍率を増やすか）
static constexpr int ENCOUNTER_RATE_MODIFIER_LUCK{ 5 };
// モンスターの出現位置
static constexpr float MONSTER_SPAWN_DISTANCE{ TILE_SIZE * (2.0f / 3.0f) };

/*実装*/
// コンストラクタ
EncounterEvent::EncounterEvent(EventContext& context) :
	pool_{ context.monster_pool_ } {
}

// オブジェクト描画
void EncounterEvent::draw_object(const GSmatrix4& matrix) const {
	if (monster_) monster_->draw();
}

// 実行
void EncounterEvent::execute(float delta_time, CharacterBase& character) {
	if (!monster_ || is_finished_) return;

	// モンスターの更新
	monster_->update(delta_time);

	// 勇者を取得
	IBattleActor* hero_actor = dynamic_cast<IBattleActor*>(&character);
	if (!hero_actor) {
		is_finished_ = true;
		monster_ = nullptr;
		return;
	}

	IBattleActor& monster_actor = monster_->get_battle_actor();

	// エンカウントモーションが終了したらバトルを開始
	if (!is_battle_start_) {
		if (character.mesh().is_motion_finished() && monster_->mesh().is_motion_finished()) {
			is_battle_start_ = true;
		}
		else return;
	}

	// バトルシステムを更新
	// TODO: デルタタイムを追加
	BattleSystem::execute(delta_time, *hero_actor, monster_actor);

	// バトルが終了したらフラグを立てる
	if (BattleSystem::is_finished()) {
		// 武器をしまう
		HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
		hero_character->set_is_weapon_holstered(true);

		// 獲得報酬
		// ヒーローに経験値を追加
		hero_character->hero_levelup().execute(monster_->drop_exp());
		// お金の獲得
		hero_character->calc_gold(100);

		// BGMの再生
		gsPlayBGM(BGM::HeroMapMovement);
		is_finished_ = true;
		monster_.reset(nullptr);
	}
}

// 開始時処理
void EncounterEvent::on_enter(CharacterBase& character) {
	is_finished_ = false;
	is_battle_start_ = false;
	// BGMの再生
	gsPlayBGM(BGM::BattleNormal);

	// 勇者を取得
	IBattleActor* hero_actor = dynamic_cast<IBattleActor*>(&character);

	// モンスターを生成
	monster_.reset(pool_->create_with_level(character.level()));

	// モンスターを勇者の半タイル前に配置
	const GSvector3 hero_pos = character.transform().position();
	const GSvector3 hero_fwd = character.transform().forward();
	const GSvector3 monster_pos = hero_pos + hero_fwd * MONSTER_SPAWN_DISTANCE;

	monster_->transform().position(monster_pos);
	monster_->transform().lookAt(hero_pos);

	// プレイヤーモンスターの方を向く
	character.transform().lookAt(monster_pos);

	// 遭遇モーションを再生
	character.mesh().forced_change_motion(HeroMotion::Encounter, false);
	monster_->mesh().forced_change_motion(MonsterMotion::Encounter, false);

	// 武器を出す
	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	hero_character->set_is_weapon_holstered(false);

	// TODO: マスの数を取得
	// スキルの条件に使うコンテキストを設定
	hero_actor->battle_set_skill_context(monster_.get(), 0);
	monster_->get_battle_actor().battle_set_skill_context(&character, 0);

	// バトルシステムの開始処理
	BattleSystem::initialize(*hero_actor, monster_->get_battle_actor());
}

// 終了フラグ
bool EncounterEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動中有効フラグ
bool EncounterEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	// 遭遇率の修正値
	const float encounter_modifier = ENCOUNTER_RATE_MODIFIER * (character.luck() / ENCOUNTER_RATE_MODIFIER_LUCK);
	// 最終的な遭遇率
	const float encounter_rate = MIN(ENCOUNTER_RATE + encounter_modifier, MAX_ENCOUNTER_RATE);
	// この関数が呼ばれるたびに、遭遇率に合わせて返す真偽値を変える
	return gsRandf(0.0f, 1.0f) < encounter_rate;
}

// 移動処理の取得
ICharacterMover* EncounterEvent::mover() noexcept {
	return &encount_mover;
}