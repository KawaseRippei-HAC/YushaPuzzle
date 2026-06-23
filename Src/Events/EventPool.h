#ifndef EVENTS_EVENT_POOL_H_
#define EVENTS_EVENT_POOL_H_

#include "Tiles/TileData.h"
#include "Events/EventTable.h"
#include "Utilities/ObjectPool.h"

// イベントプールクラス
class EventPool final :
	public ObjectPool<EventTable> {
public:
	ITileEvent* create(TileType type, struct EventContext& constext);
};

#endif // EVENTS_EVENT_POOL_H_