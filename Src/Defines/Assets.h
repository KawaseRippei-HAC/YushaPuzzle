#ifndef DEFINES_ASSETS_H_
#define DEFINES_ASSETS_H_

#include <GStype.h>
#include <GSsound.h>

// テクスチャID
namespace Texture {
	enum : GSuint {
		/* ---共通--- */
		Number,          // 数字関連まとめたやつ
		Level,           // ｢Lv｣の文字

		/* ---ボタン--- */
		BackTitle,       // タイトルに戻るボタン
		BackStageSelect, // ステージ選択に戻るボタン
		BackGamePlay,    // ゲームプレイ画面に戻るボタン
		BackSettings,    // 設定画面に戻るボタン
		GameStart,       // ゲームスタートボタン
		GameEnd,         // ゲーム終了ボタン
		Retry,           // リトライボタン
		Setting,         // 設定ボタン
		UpBox,			 // ステータスアップボタン

		/* ---カード関連--- */
		CardBackGround,                      // カード背景
		CardTextGlassStraightRoad,           // 草むらの一本道text
		CardTextGlassCornerRoad,             // 草むらのL字路text
		CardTextGlassThreeWayIntersetion,    // 草むらの三叉路text
		CardTextGlassCrossRoads,             // 草むらの十字路text
		CardTextPavedOneWayRoad,             // 舗装されたの一本道text
		CardTextPavedJunction,               // 舗装されたのL字路text
		CardTextPavedThreeWayIntersetion,    // 舗装されたの三叉路text
		CardTextPavedCrossRoads,             // 舗装された十字路text
		CardTextThereIsAnEnemy,              // 敵がいるtext
		CardTextWithTreasureChest,           // 宝箱があるtext
		GlassCardBack,                       // 草むらカード背景
		PavedCardBack,                       // 舗装されたカード背景
		CardStraightRoad,                    // 直線の道
		CardCornerRoad,                      // L字の道
		CardThreeWayRoad,                    // 三叉路の道
		CardCrossRoadsRoad,                  // 十字路の道

		/* --- タイトル関連 --- */
		TitleLogo,         // タイトルロゴ
		TitleIllustration, // タイトルに描いてあるイラスト

		/* ---ゲームプレイシーン関連--- */
		YushaName,      // ｢勇者｣
		HpGauge,        // HPゲージ
		HpGauge_bg,     // HPゲージの背景
		HpGaugeFrame,   // HPゲージのフレーム
		ExpGauge,       // 経験値ゲージ
		ExpGauge_bg,    // 経験値ゲージ背景
		MoneyIcon,      // お金袋
		InventoryFrame, // 所持品フレーム
		BackGround,     // 背景	
		Banner,         // UIバナー
		Time,           // 砂時計

		/* ---ステージセレクト関連--- */

		/* --- リザルト --- */
		HeroVictoryGameClear,// ゲームクリアの勇者
		HeroDieGameOver,     // ゲームオーバー時に表示される背景
		GameClearLogo,       // ゲームクリアロゴ
		GameOverLogo,        // ゲームオーバーロゴ

		/* --- ポーズ画面--- */
		PauseBackGround, // ポーズ画面背景
		PauseSetting,    // ポーズ画面用の設定ボタン

		/* --- 音量設定関連 --- */
		SoundSettingLogo,
		BGM,
		SE,
		Nozzle,
		VolumeGauge,
		Speaker,
		Cross,
		Volume_1,
		Volume_2,
		Volume_3,

		/* --- 勇者ステータス --- */
		HeroStatus,
		StatusScreen,
		StatusAllocation,
		StatusPoint,

		/* --- スキル --- */
		SkillSelectFrame,	// スキル選択画面の外枠
		SkillComplete,		// スキルコンプリート画面
		SkillSelect,		// スキル選択
		SkillLevel,			// スキルのレベル
		SkillBlessing,		// 祝福
		SkillRigidity,		// 剛力
		SkillReinforcingBar,// 鉄筋
		SkillAgile,			// 俊敏
		SkillGoodLuck,		// 幸運
		SkillStrength,		// 怪力
		SkillStrong,		// 強靭
		SkillIntimidation,	// 威圧感

		// 勇者移動開始ボタンテクスチャ
		HeroMoveButton,
		// 手札入れ替えボタンテクスチャ
		HandReplacementButton,
		// 手札入れ替えボタン背景テクスチャ
		HandReplacementButtonBack,

		// 宿屋イベントアイコンテクスチャ
		InnEventIcon,
		// 訓練場イベントアイコンテクスチャ
		TrainingEventIcon,
		// マスターの家（スキル獲得）イベントアイコンテクスチャ
		MasterHouseEventIcon,

		// 宿屋イベントの説明テクスチャ
		InnEventExplanation,
		// 訓練場イベントの説明テクスチャ
		TrainingEventExplanation,
		// マスターの家（スキル獲得）イベントの説明テクスチャ
		MasterHouseEventExplanation,

		/* --- イベントテキスト --- */
		// HP UPテクスチャ
		HpUp,
		// 攻撃力 UPテクスチャ
		AttackUp,
		// 防御力 UPテクスチャ
		DefenseUp,
		// 素早さ UPテクスチャ
		SpeedUp,
		// 運 UPテクスチャ
		LuckUp,
		// お金が足りなかったテクスチャ
		NotEnoughGold,
		// HPが足りなかったテクスチャ
		NotEnoughHP,

		// スカイボックス
		Skybox,

		// デッキ50 ~ 25までの表示
		Deck50,
		// デッキ25 ~ 10までの表示
		Deck25,
		// デッキ10 ~ 0までの表示
		Deck10,

		// マウスカーソル
		MouseCursor,

		// ルール説明ボタン
		RuleExplanation,
		// ルール説明画面背景
		RuleExplanationBG,

		// ロード中背景
		LoadSceneBG,
		// ロード中アニメーション
		LoadSceneAnimation,
	};
}

// メッシュID
namespace Mesh {
	enum : GSuint {
		// 設置可能ハイライトメッシュ
		PlaceableHighlight,
		// 設置不可ハイライトメッシュ
		NonPlaceableHighlight,

		// 設置エリアコーナーメッシュ
		AreaCorner,

		// 勇者メッシュ
		Hero,
		// 剣
		Sword,
		// 盾
		Shield,

		// 壁タイルメッシュ
		WallTile,
		// 一本道タイルメッシュ
		StraightTile,
		// 曲がり角タイルメッシュ
		CornerTile,
		// 三叉路タイルメッシュ
		ThreeWayTile,
		// 十字路タイルメッシュ
		CrossRoadsTile,

		// 宿屋建物メッシュ
		InnBuilding,
		// 訓練場建物メッシュ
		TrainingBuilding,
		// マスターの家建物メッシュ（スキル獲得）
		MasterHouseBuilding,

		// スライム
		Slime,
		// リザード（強敵）
		LizardWarrior,
		// 魔王
		DemonKing,
		// ファントム
		Phantom,
		// ゴーレム
		Golem,
		// 焼き鳥
		GrilledChicken,
		// オーク
		Orc,
		// スケルトン
		Skeleton,
		// ドッペルゲンガー
		Doppelganger,
	};
}

// シェーダーID
namespace Shader {
	enum : GSuint {
		// タイル用インスタンスシェーダー
		TileInstanced,
	};
}

// オクトリーID
namespace Octree {
	enum : GSuint {
		// 描画用オクトリー
		Stage,
		// 衝突判定用オクトリー
		Collider,
	};
}

// エフェクトID
namespace Effect {
	enum : GSuint {
		CameraTransition,
		CardPlacement,
		LevelUp,
		Explosion,
		AuraBuff,
		AuraDebuff,
		AuraMidBoss,
		AuraDemonKing,
		HeroHeal,
		HeroSlashAttack,
		MonsterSlashAttack,
		MonsterClawAttack,
		MonsterMagicAttack,
		MonsterDead,
		HitBlow,
		HitSlash,
		HitGuard,
	};
}

// SE ID
namespace SE {
	enum : GSuint {
		Click,				// クリック
		Cursor,				// マウスオーバー
		Slash,				// 斬撃
		BattleStart,		// バトル開始
		BattleEnd,			// バトル終了音
		LevelUp,			// レベルアップ
		StatusUp,			// ステータスアップ
		CardReplenishment,  // カード補充
		CardDrag,			// カードドラック
		DamagePunch,		// 打撃音
		CriticalHit,		// クリティカル
		ItemGet,			// アイテム入手
		SkillActivation,	// スキル使用
		GameClear,			// ゲームクリア
		GameOver,			// ゲームオーバー
		FootStaps,			// 足音
		TilePlacement,		// タイル設置音
		Heal				// 回復音
	};
}

// BGM ID
namespace BGM {
	enum : GSuint {
		Title,
		StageSelect,
		BattleNormal,
		BattleStrongEnemy,
		BattleFinal,
		GameClear,
		GameOver,
		HeroMapMovement,
	};
}
#endif // DEFINES_ASSETS_H_