#include "Events/MidBossEncounterEvent.h"

#include <gslib.h>

#include "Tiles/TileManager.h"
#include "Events/EncountMover.h"
#include "Events/EvecntContext.h"
#include "Characters/Monsters/MidBossTable.h"
#include "Systems/BattleSystem.h"
#include "Characters/Heroes/HeroDefine.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Characters/Monsters/MidBossPool.h"

/*定数*/
// モンスターの出現位置
static constexpr float SPAWN_DISTANCE = TILE_SIZE * (2.0f / 3.0f);

// 魔王出現位置オフセット
#define MID_BOSS_OFFSET GSvector3::right() * (TILE_SIZE / 3.0f)

/*実装*/
// コンストラクタ
MidBossEncounterEvent::MidBossEncounterEvent(EventContext& context) {
	// モンスターを生成
	mid_boss_character_.reset(context.mid_boss_pool_->create());
}

// オブジェクト更新
void MidBossEncounterEvent::update_object(float delta_time) {
	if (mid_boss_character_) mid_boss_character_->update(delta_time);
}
// オブジェクト描画
void MidBossEncounterEvent::draw_object(const GSmatrix4& matrix) const {
	if (mid_boss_character_ && !mid_boss_character_->is_dead()) {
		const GSvector3 pos = matrix.position() + MID_BOSS_OFFSET;
		mid_boss_character_->transform().position(pos);
		mid_boss_character_->draw();
	}
}

// 実行
void MidBossEncounterEvent::execute(float delta_time, CharacterBase& character) {
	if (!mid_boss_character_ || is_finished_) return;

	// 勇者を取得
	IBattleActor* hero_actor = dynamic_cast<IBattleActor*>(&character);
	if (!hero_actor) {
		is_finished_ = true;
		mid_boss_character_ = nullptr;
		return;
	}

	// エンカウントモーションが終了したらバトルを開始
	if (!is_battle_start_) {
		if (character.mesh().is_motion_finished() && mid_boss_character_->mesh().is_motion_finished()) {
			is_battle_start_ = true;
		}
		else return;
	}

	// バトルシステムを更新
	BattleSystem::execute(delta_time, *hero_actor, mid_boss_character_->get_battle_actor());

	// バトルが終了したらフラグを立てる
	if (BattleSystem::is_finished()) {
		// 武器をしまう
		HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
		hero_character->set_is_weapon_holstered(true);

		// 獲得報酬
		// ヒーローに経験値を追加
		hero_character->hero_levelup().execute(mid_boss_character_->drop_exp());
		// お金の獲得
		hero_character->calc_gold(100);

		// BGMの再生
		gsPlayBGM(BGM::HeroMapMovement);
		is_finished_ = true;
	}
}

// 開始時処理
void MidBossEncounterEvent::on_enter(CharacterBase& character) {
	is_finished_ = false;
	is_battle_start_ = false;
	// BGMの再生
	gsPlayBGM(BGM::BattleStrongEnemy);

	// 勇者を取得
	IBattleActor* hero_actor = dynamic_cast<IBattleActor*>(&character);

	// モンスターを勇者の半タイル前に配置
	const GSvector3 hero_pos = character.transform().position();
	const GSvector3 hero_fwd = character.transform().forward();
	const GSvector3 monster_pos = hero_pos + hero_fwd * SPAWN_DISTANCE;

	mid_boss_character_->transform().position(monster_pos);
	mid_boss_character_->transform().lookAt(hero_pos);

	// プレイヤーモンスターの方を向く
	character.transform().lookAt(monster_pos);

	// 遭遇モーションを再生
	character.mesh().forced_change_motion(HeroMotion::Encounter, false);
	mid_boss_character_->mesh().forced_change_motion(MidBossMotion::Encounter, false);

	// 武器を出す
	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	hero_character->set_is_weapon_holstered(false);

	// スキルの条件に使うコンテキストを設定
	hero_actor->battle_set_skill_context(mid_boss_character_.get(), 0);
	mid_boss_character_->get_battle_actor().battle_set_skill_context(&character, 0);

	// バトルシステムの開始処理
	BattleSystem::initialize(*hero_actor, mid_boss_character_->get_battle_actor());
}

// 終了フラグ
bool MidBossEncounterEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動中有効フラグ
bool MidBossEncounterEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	// 中ボスが生きている時だけ有効
	return (mid_boss_character_) ? !mid_boss_character_->is_dead() : false;
}

// 移動妨害フラグ
bool MidBossEncounterEvent::is_impede_movement(void) const noexcept {
	return true;
}

// 移動処理の取得
ICharacterMover* MidBossEncounterEvent::mover() noexcept {
	return &encount_mover;
}
