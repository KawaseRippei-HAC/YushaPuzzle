#ifndef SYSTEMS_GAME_MANAGER_H_
#define SYSTEMS_GAME_MANAGER_H_

#include <string>
#include <memory>

#include "UI/ButtonWidget.h"
#include "UI/PlaceableHighlight.h"
#include "Events/EventPool.h"
#include "Tiles/TileManager.h"
#include "Cards/CardManager.h"
#include "Cameras/Cinemachine.h"
#include "Animations/EffectPlayer.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Characters/Monsters/MonsterPool.h"
#include "Characters/Monsters/MidBossPool.h"
#include "Characters/Monsters/DemonKingCharacter.h"

// ゲームマネージャークラス
class GameManager {
public:
	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;
	// GUI描画
	void draw_gui(void) const;

	// 初期化
	void initialize(const std::string& stage_path);

	// 消去
	void clear(void);

	// ゲームクリアフラグ
	static bool is_game_clear(void) noexcept;
	// ゲームオーバーフラグ
	static bool is_game_over(void) noexcept;

	// 勇者キャラクターの取得
	HeroCharacter* get_hero(void) const noexcept;

private:
	// プレイヤーフェーズ開始処理
	void start_player_phase(void);
	// 勇者行動フェーズ開始処理
	void start_hero_phase(void);

	// カード設置フェーズの更新
	void update_cards_placement_phase(float delta_time);
	// 勇者行動フェーズの更新
	void update_heroes_action_phase(float delta_time);

	// タイル置換の試行
	bool try_replace_tile(TileData* data);

private:
	// タイマーの描画
	void draw_timer(void) const;
	// 手札入れ替えボタンの描画
	void draw_hand_replacement_button(void) const;

private:
	// ゲームクリアフラグ
	inline static bool is_game_clear_{ false };
	// ゲームオーバーフラグ
	inline static bool is_game_over_{ false };

	// フェーズタイマー
	float phase_timer_{ 0.0f };
	// プレイヤーフェーズ中フラグ
	bool is_in_player_phase_{ true };

	// シネマシーン
	std::unique_ptr<Cinemachine> cinemachine_{ nullptr };

	// イベントプール
	std::unique_ptr<EventPool> event_pool_{ nullptr };
	// モンスタープール
	std::unique_ptr<MonsterPool> monster_pool_{ nullptr };
	// 強敵プール
	std::unique_ptr<MidBossPool> mid_boss_pool_{ nullptr };

	// タイルマネージャー
	std::unique_ptr<TileManager> tile_manager_{ nullptr };
	// カードマネージャー
	std::unique_ptr<CardManager> card_manager_{ nullptr };
	// フェーズ終了ボタン
	std::unique_ptr<ButtonWidget> phase_end_button_{ nullptr };

	// 手札入れ替えチャージ
	int hand_replacable_charge_{ 0 };
	// 手札入れ替えボタン
	std::unique_ptr<ButtonWidget> hand_replacement_button_{ nullptr };

	// 勇者キャラクター
	std::unique_ptr<HeroCharacter> hero_character_{ nullptr };
	// 魔王キャラクター
	std::unique_ptr<DemonKingCharacter> demon_king_character_{ nullptr };

	// カード設置エフェクト
	EffectPlayer card_place_effect_;
	// 設置可能ハイライト
	std::unique_ptr<PlaceableHighlight> placeable_highlight_{ nullptr };
};

#endif // SYSTEMS_GAME_MANAGER_H_