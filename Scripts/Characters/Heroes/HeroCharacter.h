#ifndef CHARACTERS_HEROES_HERO_CHARACTER_H_
#define CHARACTERS_HEROES_HERO_CHARACTER_H_

#include "Defines/Assets.h"
#include "Tiles/TileManager.h"
#include "Animations/AnimatedMesh.h"
#include "Interfaces/IBattleActor.h"
#include "Characters/CharacterBase.h"
#include "Characters/Heroes/HeroDefine.h"
#include "UI/HeroUI.h"
#include "Skills/SkillActor.h"
#include "Characters/Heroes/HeroLevelUp.h"
#include "Animations/EffectPlayer.h"
#include "Events/EventTable.h"
#include "Animations/AcquisitionAnimation.h"

// 勇者キャラクタークラス
class HeroCharacter final :
	public CharacterBase, public IBattleActor {
public:
	explicit HeroCharacter(TileGraph& graph);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;
	// GUI描画
	void draw_gui(void) const;

	// 現在のタイルの取得
	const TileNode* current_tile(void) const noexcept;

	// 攻撃力の取得
	attack_t attack(void) const noexcept override;
	// 防御力の取得
	defense_t defense(void) const noexcept override;

	// お金の計算
	gold_t calc_gold(gold_t amount) noexcept override;

	// 死亡処理
	void die(float delta_time) override;
	// 死亡フラグ
	bool is_dead(void) const noexcept override;

	// 思考の開始
	void think_start(void) noexcept;
	// 移動の開始
	void move_start(void) noexcept;

	// 行動終了フラグ
	bool is_action_ended(void) const noexcept;

	// スキルのコンテキストを設定（スキルを持つ時だけオーバーライド）
	void battle_set_skill_context(CharacterBase* target, int tile_count) override;

	// バトル開始時の初期化
	void battle_start_initialize() override;
	// デバフを追加
	void battle_set_debuff(Modifier debuff) override;
	// デバフ解除判定
	void battle_check_debuffs_removal() override;

	// ターン開始スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
	Modifier battle_turn_start_skill() override;
	// 攻撃スキルを実行 相手にかけるデバフを返す（スキルを持つ時だけオーバーライド）
	Modifier battle_attack_skill() override;

	// バトルのターン開始時の処理
	void on_attack_turn_start(const bool is_critical) override;
	// バトルの防御ターン開始時の処理
	void on_defence_turn_start(const DefenderStatus status, const attack_t damage, const bool is_critical) override;

	// ターン終了
	void battle_turn_end()override;
	// 戦闘終了
	void battle_end() override;

	// 戦闘アクション
	void battle_action(float delta_time) override;
	// 戦闘リアクション開始処理
	void on_battle_reaction_start() override;
	// 戦闘リアクション
	void battle_reaction(float delta_time) override;

	// 素早さの取得
	speed_t agility(void) const noexcept override;

	// 回復処理
	void battle_heal() override;
	// 回復処理完了フラグ
	bool is_heal_completed() const noexcept override;

	// 戦闘コンテキストを取得
	AttackerBattleContext attacker_battle_context(void) const noexcept override;
	// 防御コンテキストを取得
	DefenderBattleContext defender_battle_context(void) const noexcept override;

	// 敗北フラグ
	bool is_defeated(void) const noexcept override;

	// スキルの追加
	void add_skill(const SkillId& add_id, int level = 1);
	// スキルアクター取得
	SkillActor* skill_actor(void) noexcept override;
	// ヒーローのレベルアップ処理取得
	HeroLevelUp& hero_levelup(void) noexcept;
	// 現在の経験値量を取得
	int experience(void) const noexcept;
	// 必要経験値量を取得
	int required_experience(void) const noexcept;
	// ステータスポイント取得
	int status_point(void) const noexcept;
	// 経験値の追加
	void add_experience(int amount) noexcept;
	// 必要経験値量を追加
	void add_required_experience(int amount) noexcept;
	// 経験値のリセット
	void reset_experience(void) noexcept;
	// ステータスポイント追加
	void add_status_point(int amount);

	// 武器をしまうかのフラグを設定
	void set_is_weapon_holstered(bool is_weapon_holstered);

	// エフェクトの更新処理
	void battle_effect_update(float delta_time) override;

	// イベント
	void set_event_type(GSuint event);

	GSuint event_type();

	/// <summary>
	/// 獲得アニメーションの追加
	/// </summary>
	/// <param name="texture_id">テクスチャID</param>
	/// <param name="ratio">画像の比率</param>
	/// <param name="offset">位置のオフセット</param>
	void add_acquisition_animation(GSuint texture_id, GSvector2 ratio, float scale = 1.0f);

private:
	// 全武器の描画
	void draw_all_weapons(void) const;
	// 武器の描画
	void draw_weapon(const GSuint mesh_id, const int bone, const float position[3], const float rotate[3]) const;

private:
	int exp_{ 0 };			// 現在の経験値
	int required_exp_{ 5 };	// 必要経験値
	int status_poitn_{ 0 };	// ステータス割り振りポイント

	// ゲームオーバーフラグ
	bool is_game_over_{ false };
	// 武器をしまっているか
	bool is_weapon_holstered_{ false };
	// 回復エフェクト再生中フラグ
	bool is_play_heal_effect_{ false };
	// 回復処理終了フラグ
	bool is_heal_completed_{ false };

	// 勇者コンテキスト
	HeroContext context_;
	// ステートマシン
	HeroStateMachine state_{ context_ };

	// 勇者UI
	HeroUI hud_;

	// スキルを所持
	SkillActor skill_actor_;
	// レベルアップ
	HeroLevelUp hero_levelup_{ this };

	// 回復エフェクト
	EffectPlayer heal_effect_;

	// 獲得アニメーション
	AcquisitionAnimation acquisition_animation_{};

	// どのイベントにいるか
	GSuint event_type_{ Event::FallInAbyss };
};

#endif // CHARACTERS_HEROES_HERO_CHARACTER_H_