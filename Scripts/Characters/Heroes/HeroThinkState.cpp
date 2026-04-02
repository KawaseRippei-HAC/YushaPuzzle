#include "Characters/Heroes/HeroThinkState.h"

#include <GSeasing.h>

#include "Events/InnEvent.h"
#include "Events/MidBossEncounterEvent.h"
#include "Tiles/TileManager.h"
#include "Animations/AnimatedMesh.h"
#include "Systems/LoggableRandomizer.h"
#include "Characters/Heroes/HeroContext.h"
#include "Characters/Heroes/HeroCharacter.h"

/*定数*/
// 勇者思考種別
enum HeroThink : unsigned char {
	AimForDemonKing,
	AimForMidBoss,
	ShouldRecover,
	DecideRandomTile,
};

// 最短思考時間
static constexpr float MIN_THINK_TIME{ 120.0f };
// 最長思考時間
static constexpr float MAX_THINK_TIME{ 240.0f };

// 思考モーション遷移時間
static constexpr float THINK_MOTION_LERP_TIME{ 30.0f };

// 最短移動距離
static constexpr int SHORTEST_TRAVEL_DISTANCE{ 2 };
// 最長移動距離
static constexpr int MAXIMUM_TRAVEL_DISTANCE{ 5 };

// 魔王適正レベルオフセット
static constexpr float DEMON_KING_LEVEL_OFFSET{ 0.05f };
// 魔王遭遇率
static constexpr float DEMON_KING_ENCOUNTER_RATE{ 15.0f };
// 強敵適正レベルオフセット
static constexpr float MID_BOSS_LEVEL_OFFSET{ 0.10f };
// 強敵遭遇率
static constexpr float MID_BOSS_ENCOUNTER_RATE{ 5.0f };

// HP要回復割合
static constexpr float SHOULD_RECOVER_RATE{ 0.4f };
// 奈落落下率
static constexpr float ABYSS_DROP_RATE{ 30.0f };

/*実装*/
// コンストラクタ
HeroThinkState::HeroThinkState() {
	// TODO: 勇者の目的地設定ビヘイビアツリーの正式実装
	// ビヘイビアツリーの初期化
	behavior_.register_root(new SelectorNode{});

	// HPが少ないか判定する条件ノード
	behavior_.add_node(HeroThink::ShouldRecover,
		new ConditionNode{ [this]() { return this->is_should_recover(); },
		new FunctionNode{ [this]() { return this->find_inn(); } } }
	);

	// 魔王に向かうか判定する関数ノード
	behavior_.add_node(HeroThink::AimForDemonKing,
		new FunctionNode{ [this]() { return this->aim_for_demon_king(); } }
	);

	// 強敵に向かうか判定する関数ノード
	behavior_.add_node(HeroThink::AimForMidBoss,
		new FunctionNode{ [this]() { return this->aim_for_mid_boss(); } }
	);

	// ランダムなタイルを決定する関数ノード
	behavior_.add_node(HeroThink::DecideRandomTile,
		new FunctionNode{ [this]() { this->decide_random_time(); return NodeStatus::Success; } }
	);
}

// 更新
void HeroThinkState::on_update(float delta_time, HeroContext& context) {
	// タイマーの減算
	think_timer_ -= delta_time;
	// 思考時間が完了したら、移動状態へ遷移
	if (think_timer_ <= 0.0f)
		(context.is_actioning) ? change_state(HeroState::Move) : change_state(HeroState::Idle);

	// パスを探索済みなら以降を無視
	if (!context.path.empty()) return;

	// 目的地選別と経路探索を、別々のフレームで行う
	if (!destination_) {
		// ビヘイビアツリーを更新
		behavior_.update(delta_time);
	}
	else {
		// 現在のタイルから目的地までの経路を探索
		context.path = context.graph->find_path(*context.current_tile, *destination_);
		if (context.path.empty()) { destination_ = nullptr; return; }

		// 移動距離が短すぎるか長すぎるなら経路を初期化
		std::stack<TileNode*> empty{};
		const size_t distance = context.path.size();
		if (distance < SHORTEST_TRAVEL_DISTANCE) context.path.swap(empty);
		else if (distance > MAXIMUM_TRAVEL_DISTANCE) context.path.swap(empty);

		// 経路が見つからなければ目的地をリセット
		if (context.path.empty()) destination_ = nullptr;
	}
}

// 遷移時処理
void HeroThinkState::on_enter(HeroContext& context) {
	context_ = &context;
	// 考える時間を決定
	LoggableRandomizer::log("<think initialize>");
	think_timer_ = LoggableRandomizer::generate(MIN_THINK_TIME, MAX_THINK_TIME);
	LoggableRandomizer::log("<think initialize end>");
	// モーションの変更
	context.character->mesh().change_motion(HeroMotion::Thinking, true, THINK_MOTION_LERP_TIME);
	// パスをリセットしておく
	std::stack<TileNode*> empty;
	context_->path.swap(empty);
}

// 終了時処理
void HeroThinkState::on_exit(HeroContext& context) {
	inns_.clear();
	inn_index_ = 0;
	mid_bosses_.clear();
	mid_boss_index_ = 0;
	is_demon_king_checked_ = false;
	is_mid_boss_checked_ = false;
	behavior_.reset();
	context.character->mesh().change_motion(HeroMotion::Idle, true, THINK_MOTION_LERP_TIME);
	this->IHeroState::on_exit(context);
}

// 要回復フラグ
bool HeroThinkState::is_should_recover() const noexcept {
	const point_t hp = context_->character->hp();
	const point_t max_hp = context_->character->max_hp();
	const float rate = static_cast<float>(hp) / static_cast<float>(max_hp);
	return rate <= SHOULD_RECOVER_RATE;
}

// 魔王タイル決定
NodeStatus HeroThinkState::aim_for_demon_king() {
	if (is_demon_king_checked_) return NodeStatus::Failure;

	this->is_demon_king_checked_ = true;

	const level_t level = context_->character->level();
	const float ratio = static_cast<float>(level) / static_cast<float>(MAX_LEVEL);
	const float eased = std::min(ratio + DEMON_KING_LEVEL_OFFSET, 1.0f);
	const float probability = gsEasingInOutQuart(eased);
	const bool should_go = Randomizer::generate(0.0f, 1.0f) <= probability;
	if (!should_go) return NodeStatus::Failure;
	destination_ = context_->graph->goal_tile();
	return NodeStatus::Success;
}

// 強敵タイル決定
NodeStatus HeroThinkState::aim_for_mid_boss() {
	if (is_mid_boss_checked_) return NodeStatus::Failure;

	if (mid_bosses_.empty()) {
		mid_bosses_ = context_->graph->find_tiles(
			[](const TileNode& tile) {
				return tile.type() == TileType::MidBoss;
			}
		);

		if (mid_bosses_.size() > 1) std::sort(mid_bosses_.begin(), mid_bosses_.end(),
			[this](const TileNode* lhs, const TileNode* rhs) {
				const float l_dist = TileGraph::heuristic(*context_->current_tile, *lhs);
				const float r_dist = TileGraph::heuristic(*context_->current_tile, *rhs);
				return l_dist < r_dist;
			}
		);
		return NodeStatus::Running;
	}

	this->is_mid_boss_checked_ = true;

	const level_t level = context_->character->level();
	const float ratio = static_cast<float>(level) / static_cast<float>(MAX_LEVEL);
	const float eased = std::min(ratio + MID_BOSS_LEVEL_OFFSET, 1.0f);
	const float probability = gsEasingInOutQuart(eased);
	const bool should_go = Randomizer::generate(0.0f, 1.0f) <= probability;
	if (!should_go) return NodeStatus::Failure;
	if (mid_boss_index_ >= mid_bosses_.size()) return NodeStatus::Failure;
	destination_ = mid_bosses_[mid_boss_index_++]; return NodeStatus::Success;
}

// 宿屋の検索
NodeStatus HeroThinkState::find_inn() {
	if (context_->character->gold() < ACCOMMODATION_FEE) return NodeStatus::Failure;

	if (inns_.empty()) {
		inns_ = context_->graph->find_tiles(
			[](const TileNode& tile) {
				if (tile.type() != TileType::Event) return false;
				if (InnEvent* inn = dynamic_cast<InnEvent*>(tile.event())) return true;
				return false;
			}
		);

		if (inns_.size() > 1) std::sort(inns_.begin(), inns_.end(),
			[this](const TileNode* lhs, const TileNode* rhs) {
				const float l_dist = TileGraph::heuristic(*context_->current_tile, *lhs);
				const float r_dist = TileGraph::heuristic(*context_->current_tile, *rhs);
				return l_dist < r_dist;
			}
		);
	}

	if (inn_index_ >= inns_.size()) return NodeStatus::Failure;
	destination_ = inns_[inn_index_++]; return NodeStatus::Success;
}

// ランダムタイル決定
void HeroThinkState::decide_random_time() {
	const int random_x = Randomizer::generate(0, context_->graph->width() - 1);
	// 目的地が後方なら、距離に応じた確率でリターン
	const int diff = random_x - context_->current_tile->x();
	if (diff < 0) {
		constexpr float max{ MAXIMUM_TRAVEL_DISTANCE };
		const float ratio = static_cast<float>(std::abs(diff)) / max;
		if (Randomizer::generate(0.0f, 1.0f) < ratio) return;
	}
	// ランダムなタイルを取得
	const int random_z = Randomizer::generate(0, context_->graph->depth() - 1);
	TileNode* tile = context_->graph->tiles()[random_z][random_x].get();
	if (!tile) return;

	// 運の進行度を取得
	constexpr float max_luck{ static_cast<float>(MAX_LUCK - INITIAL_LUCK) };
	const luck_t luck = context_->character->luck() + context_->character->luck_modifier();
	const float ratio = 1.0f - (static_cast<float>(luck - INITIAL_LUCK) / max_luck);

	// 到達地点タイルなら確率で設定
	if (tile == context_->graph->goal_tile())
		if (Randomizer::percentage(DEMON_KING_ENCOUNTER_RATE * ratio)) destination_ = tile;

	// 目的地が設定済みならリターン
	if (destination_) return;

	switch (tile->type()) {
		// 通常タイルならそのまま設定
		default: destination_ = tile; break;

		case TileType::Abyss:
			// 奈落タイルなら確率で設定
			if (Randomizer::percentage(ABYSS_DROP_RATE * ratio)) destination_ = tile; break;
		case TileType::MidBoss:
			// 強敵タイルなら確率で設定
			if (Randomizer::percentage(MID_BOSS_ENCOUNTER_RATE * ratio)) destination_ = tile; break;
	}
}