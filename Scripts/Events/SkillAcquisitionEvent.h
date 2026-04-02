#ifndef EVENTS_SKILL_ACQUISITION_EVENT_H_
#define EVENTS_SKILL_ACQUISITION_EVENT_H_

#include <GStransform.h>

#include "Interfaces/ITileEvent.h"
#include "Skills/SkillDefinition.h"
class HeroCharacter;

// スキル獲得イベント
class SkillAcquisitionEvent :
	public ITileEvent {
public:
	SkillAcquisitionEvent(struct EventContext&) {}

	// オブジェクト描画
	void draw_object(const GSmatrix4& matrix) const override;

	// 実行
	void execute(float delta_time, CharacterBase& character) override;

	// 開始時処理
	void on_enter(CharacterBase& character) override;
	// 終了時処理
	void on_exit(CharacterBase& character) override;

	// イベント種別の取得
	EventType type(void) const noexcept override;

	// アイコンの取得
	GSuint icon() const noexcept override;

	// 終了フラグ
	bool is_finished(void) const noexcept override;
	// 移動中有効フラグ
	bool is_activate_in_moving(CharacterBase& character) const noexcept override;

	// 移動処理の取得
	ICharacterMover* mover(void) noexcept override;

private:
	// スキル獲得
	SkillId add_skill_id(HeroCharacter* hero_charcter);
private:
	// 終了フラグ
	bool is_finished_{ false };

	// タイマ
	float timer_{ 0 };

	SkillId add_skill_id_{ 0 };
};

#endif // EVENTS_SKILL_ACQUISITION_EVENT_H_
