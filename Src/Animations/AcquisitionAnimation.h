#ifndef ANIMATIONS_ACQUISITION_ANIMATION_H_
#define ANIMATIONS_ACQUISITION_ANIMATION_H_

// 画像の比率x1:y1
#define RATIO_1X1 GSvector2{ 1.0f, 1.0f }

// 画像の比率x4:y1
#define RATIO_4X1 GSvector2{ 4.0f, 1.0f }

// 画像の比率x8:y1
#define RATIO_8X1 GSvector2{ 8.0f, 1.0f }

// アニメーション1つのコンテキスト
struct GetAnimationContext{
	// テクスチャID
	GSuint texture_id{ 0 };
	// 位置
	GSvector3 position{ 0.0f, 0.0f, 0.0f };
	// スプライトの矩形
	GSrect rect{ 0.0f, 0.0f, 0.0f, 0.0f };
	// 拡大縮小
	GSvector2 scale{ 1.0f, 1.0f };
	// カラー
	GScolor color{ 1.0f, 1.0f, 1.0f, 1.0f };
}; 

// スキルやパラメータ獲得時のアニメーションクラス
// 移動しながら透明になっていくアニメーション
class AcquisitionAnimation {
public:
	// コンストラクタ
	AcquisitionAnimation();
	// デストラクタ
	~AcquisitionAnimation();

	// 更新
	void update();
	// 描画
	void draw() const;

	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="texture_id">テクスチャID</param>
	/// <param name="ratio">画像の比率</param>
	/// <param name="position">位置</param>
	void add(GSuint texture_id, GSvector2 ratio, GSvector3 position, float scale = 1.0f);

private:
	// 削除
	void clear();

private:
	// 描画するアニメーション
	std::vector<GetAnimationContext*> draw_animations_;
};

#endif // ANIMATIONS_ACQUISITION_ANIMATION_H_
