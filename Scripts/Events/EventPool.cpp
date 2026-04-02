#include "Events/EventPool.h"

#include "Events/EvecntContext.h"
#include "Events/InnEvent.h"
#include "Events/EncounterEvent.h"
#include "Events/FallInAbyssEvent.h"
#include "Events/EventTable.h"

/*実装*/
// イベントの生成
ITileEvent* EventPool::create(TileType type, EventContext& context) {
	// TODO: 必要なイベントだけ生成可能な、イベントプールの本実装
	switch (type) {
		case TileType::Abyss: return EventTable::create(Event::FallInAbyss, context);
		case TileType::Road: return EventTable::create(Event::Encounter, context);
		case TileType::Event: return random_create(context);
		case TileType::MidBoss: return EventTable::create(Event::MidBossEncounter, context);
	}
	return nullptr;
}