#ifndef INTERFACES_ITILE_EVENT_H_
#define INTERFACES_ITILE_EVENT_H_

#include "Characters/CharacterBase.h"
#include "Interfaces/ICharacterMover.h"

// イベント種別
enum class EventType {
	// イベント無し
	None,

	/*固定イベント*/
	// 奈落落下イベント
	Fall,
	// モンスター遭遇イベント
	Encounter,
	// 強敵イベント
	MidBoss,

	/*イベントタイル用イベント*/
	// 宿屋イベント
	Inn,
	// 訓練イベント
	Training,
	// スキル獲得イベント
	SkillAcquisition,
	// 武器屋イベント
	WeaponShop,
};

/*定数*/
// 既定イベントウェイト
inline constexpr float DEFAULT_EVENT_WEIGHT{ 1.0f };

// タイルイベント抽象インターフェース
class ITileEvent {
public:
	virtual ~ITileEvent() = default;

	// 更新
	virtual void update_object(float delta_time) {}

	// 描画
	virtual void draw_object([[maybe_unused]] const GSmatrix4& matrix) const {}

	// 実行
	virtual void execute(float delta_time, CharacterBase& character) = 0;

	// 開始時処理
	virtual void on_enter(CharacterBase& character) {};
	// 終了時処理
	virtual void on_exit(CharacterBase& character) {};

	// イベント種別の取得
	virtual EventType type(void) const noexcept { return EventType::None; }
	// 重みの取得
	virtual float weight(void) const noexcept { return DEFAULT_EVENT_WEIGHT; }
	// アイコンの取得
	virtual GSuint icon(void) const noexcept { return NULL; }

	// 終了フラグ
	virtual bool is_finished(void) const noexcept = 0;
	// 移動中有効フラグ
	virtual bool is_activate_in_moving(CharacterBase& character) const noexcept { return false; }
	// 移動妨害フラグ
	virtual bool is_impede_movement(void) const noexcept { return false; }

	// 移動処理の取得
	virtual ICharacterMover* mover(void) noexcept { return nullptr; }
};

#endif // INTERFACES_ITILE_EVENT_H_