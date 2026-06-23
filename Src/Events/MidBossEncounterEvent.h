#ifndef EVENTS_MID_BOSS_ENCOUNTER_EVENT_H_
#define EVENTS_MID_BOSS_ENCOUNTER_EVENT_H_

#include <GStransform.h>

#include "Interfaces/ITileEvent.h"
#include "Characters/Monsters/MidBossCharacter.h"

// モンスター遭遇イベント
class MidBossEncounterEvent :
	public ITileEvent {
public:
	// コンストラクタ
	MidBossEncounterEvent(struct EventContext& context);

	// オブジェクト更新
	void update_object(float delta_time) override;
	// オブジェクト描画
	void draw_object(const GSmatrix4& matrix) const override;

	// 実行
	void execute(float delta_time, CharacterBase& character) override;

	// 開始時処理
	void on_enter(CharacterBase& character) override;

	// 終了フラグ
	bool is_finished(void) const noexcept override;
	// 移動中有効フラグ
	bool is_activate_in_moving(CharacterBase& character) const noexcept override;
	// 移動妨害フラグ
	bool is_impede_movement(void) const noexcept override;

	// 移動処理の取得
	ICharacterMover* mover(void) noexcept override;

private:
	// 終了フラグ
	bool is_finished_{ false };
	// バトル開始フラグ
	bool is_battle_start_{ false };
	// 遭遇したモンスター
	std::unique_ptr<MidBossCharacter> mid_boss_character_{ nullptr };
};

#endif // EVENTS_MID_BOSS_ENCOUNTER_EVENT_H_
