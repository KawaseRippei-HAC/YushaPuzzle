#ifndef CHARACTERS_MONSTERS_MONSTER_TABLE_
#define CHARACTERS_MONSTERS_MONSTER_TABLE_

#include "Defines/Assets.h"
#include "Utilities/ObjectTable.h"
#include "Characters/Monsters/MonsterData.h"
#include "Characters/Monsters/MonsterCharacter.h"
#include "Characters/Monsters/MonsterDefaultBattleStrategy.h"

template<class TFactory, class ...TDerived>
class MonsterTableTemplate :
	public ObjectTable<TFactory, TDerived...> {
	using Table = ObjectTable<TFactory, TDerived...>;
public:
	// 最小レベルの取得
	static constexpr level_t get_min_level(size_t index) noexcept {
		if (index >= Table::count) return MAX_LEVEL;
		return levels_[index];
	}

private:
	// 最小レベルの作成
	template<class T>
	static consteval level_t make_min_level(void) noexcept {
		if constexpr (requires { T::MIN_LEVEL; }) { return T::MIN_LEVEL; }
		else { return 0; }
	}

	// 最小レベルテーブル
	static inline constexpr std::array<level_t, Table::count> levels_{ make_min_level<TDerived>()... };
};

// スライム
using SlimeCharacter = MonsterCharacter<MonsterDefaultBattleStrategy, SLIME_DATA, SLIME_SPAWN_REQUIRE_LEVEL>;
// ファントム
using PhantomCharacter = MonsterCharacter<MonsterDefaultBattleStrategy, PHANTOM_DATA, PHANTOM_SPAWN_REQUIRE_LEVEL>;
// ゴーレム
using GolemCharacter = MonsterCharacter<MonsterDefaultBattleStrategy, GOLEM_DATA, GOLEM_SPAWN_REQUIRE_LEVEL>;
// 焼き鳥
using GrilledChickenCharacter = MonsterCharacter<MonsterDefaultBattleStrategy, GRILLED_CHICKEN_DATA, GRILLED_CHICKEN_SPAWN_REQUIRE_LEVEL>;
// オーク
using OrcCharacter = MonsterCharacter<MonsterDefaultBattleStrategy, ORC_DATA, ORC_SPAWN_REQUIRE_LEVEL>;
// スケルトン
using SkeletonCharacter = MonsterCharacter<MonsterDefaultBattleStrategy, SKELETON_DATA, SKELETON_SPAWN_REQUIRE_LEVEL>;

using MonsterTable = MonsterTableTemplate<
	MonsterBase* (*)(),
	SlimeCharacter,
	PhantomCharacter,
	GolemCharacter,
	GrilledChickenCharacter,
	OrcCharacter,
	SkeletonCharacter
>;

#endif // CHARACTERS_MONSTERS_MONSTER_TABLE_