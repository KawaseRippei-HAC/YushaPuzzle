#ifndef EVENTS_EVENT_TABLE_H_
#define EVENTS_EVENT_TABLE_H_

#include "Events/InnEvent.h"
#include "Events/EncounterEvent.h"
#include "Events/FallInAbyssEvent.h"
#include "Events/MidBossEncounterEvent.h"
#include "Events/TrainingEvent.h"
#include "Events/SkillAcquisitionEvent.h"
#include "Utilities/ObjectTable.h"

// イベント種別
namespace Event {
	enum Event {
		// 宿屋イベント
		Inn,
		// モンスター遭遇イベント
		Encounter,
		// 奈落落下イベント
		FallInAbyss,
		// 強敵遭遇イベント
		MidBossEncounter,
		// 訓練イベント
		Training,
		// スキル獲得イベント
		SkillAcquisition,
	};
}

// イベントテーブル
using EventTable = ObjectTable <
	ITileEvent* (*)(struct EventContext&),
	InnEvent,
	EncounterEvent,
	FallInAbyssEvent,
	MidBossEncounterEvent,
	TrainingEvent,
	SkillAcquisitionEvent
>;

#endif // EVENTS_EVENT_TABLE_H_