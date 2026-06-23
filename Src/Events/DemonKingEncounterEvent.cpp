#include "Events/DemonKingEncounterEvent.h"

#include <gslib.h>

#include "Tiles/TileManager.h"
#include "Events/EncountMover.h"
#include "Systems/BattleSystem.h"
#include "Characters/Heroes/HeroDefine.h"
#include "Characters/Heroes/HeroCharacter.h"

/*定数*/
// モンスターの出現位置
static constexpr float SPAWN_DISTANCE = TILE_SIZE * (2.0f / 3.0f);

/*実装*/
// コンストラクタ
DemonKingEncounterEvent::DemonKingEncounterEvent(DemonKingCharacter& demon_king_character) :
	demon_king_character_{ &demon_king_character } {
}

// オブジェクト描画
void DemonKingEncounterEvent::draw_object(const GSmatrix4& matrix) const {
	if (demon_king_character_) demon_king_character_->draw();
}

// 実行
void DemonKingEncounterEvent::execute(float delta_time, CharacterBase& character) {
	if (!demon_king_character_ || is_finished_) return;

	// モンスターの更新
	demon_king_character_->update(delta_time);

	// 勇者を取得
	IBattleActor* hero_actor = dynamic_cast<IBattleActor*>(&character);
	if (!hero_actor) {
		is_finished_ = true;
		demon_king_character_ = nullptr;
		return;
	}

	// エンカウントモーションが終了したらバトルを開始
	if (!is_battle_start_) {
		if (character.mesh().is_motion_finished() && demon_king_character_->mesh().is_motion_finished()) {
			is_battle_start_ = true;
		}
		else return;
	}

	// バトルシステムを更新
	BattleSystem::execute(delta_time, *hero_actor, demon_king_character_->get_battle_actor());

	// バトルが終了したらフラグを立てる
	if (BattleSystem::is_finished()) {
		// 武器をしまう
		HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
		hero_character->set_is_weapon_holstered(true);

		// BGMの再生
		gsPlayBGM(BGM::HeroMapMovement);
		is_finished_ = true;
	}
}

// 開始時処理
void DemonKingEncounterEvent::on_enter(CharacterBase& character) {
	is_finished_ = false;
	is_battle_start_ = false;
	// BGMの再生
	gsPlayBGM(BGM::BattleFinal);

	// 勇者を取得
	IBattleActor* hero_actor = dynamic_cast<IBattleActor*>(&character);

	// モンスターを勇者の半タイル前に配置
	const GSvector3 hero_pos = character.transform().position();
	const GSvector3 hero_fwd = character.transform().forward();
	const GSvector3 monster_pos = hero_pos + hero_fwd * SPAWN_DISTANCE;

	demon_king_character_->transform().position(monster_pos);
	demon_king_character_->transform().lookAt(hero_pos);

	// プレイヤーモンスターの方を向く
	character.transform().lookAt(monster_pos);

	// 遭遇モーションを再生
	character.mesh().forced_change_motion(HeroMotion::Encounter, false);
	demon_king_character_->mesh().forced_change_motion(DemonKingMotion::Encounter, false);

	// 武器を出す
	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	hero_character->set_is_weapon_holstered(false);

	// スキルの条件に使うコンテキストを設定
	hero_actor->battle_set_skill_context(demon_king_character_, 0);
	demon_king_character_->get_battle_actor().battle_set_skill_context(&character, 0);

	// バトルシステムの開始処理
	BattleSystem::initialize(*hero_actor, demon_king_character_->get_battle_actor());
}

// 終了フラグ
bool DemonKingEncounterEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動中有効フラグ
bool DemonKingEncounterEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	return true;
}

// 移動処理の取得
ICharacterMover* DemonKingEncounterEvent::mover() noexcept {
	return &encount_mover;
}