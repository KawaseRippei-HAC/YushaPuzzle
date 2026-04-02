#ifndef EVENTS_DEMON_KING_ENCOUNTER_EVENT_H_
#define EVENTS_DEMON_KING_ENCOUNTER_EVENT_H_

#include <GStransform.h>

#include "Interfaces/ITileEvent.h"
#include "Characters/Monsters/DemonKingCharacter.h"

// モンスター遭遇イベント
class DemonKingEncounterEvent :
	public ITileEvent {
public:
	// コンストラクタ
	DemonKingEncounterEvent(DemonKingCharacter& demon_king_character);

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

	// 移動処理の取得
	ICharacterMover* mover(void) noexcept override;

private:
	// 終了フラグ
	bool is_finished_{ false };
	// バトル開始フラグ
	bool is_battle_start_{ false };
	// 遭遇したモンスター
	DemonKingCharacter* demon_king_character_{ nullptr };
};

#endif // EVENTS_DEMON_KING_ENCOUNTER_EVENT_H_