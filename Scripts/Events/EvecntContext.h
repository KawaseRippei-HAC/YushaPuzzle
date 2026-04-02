#ifndef EVENTS_EVENT_CONTEXT_H_
#define EVENTS_EVENT_CONTEXT_H_

// イベントコンテキスト構造体
struct EventContext {
	class MonsterPool* monster_pool_{ nullptr };
	class MidBossPool* mid_boss_pool_{ nullptr };
};

#endif // EVENTS_EVENT_CONTEXT_H_