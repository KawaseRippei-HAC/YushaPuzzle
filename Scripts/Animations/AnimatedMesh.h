#ifndef ANIMATIONS_ANIMATED_MESH_H_
#define ANIMATIONS_ANIMATED_MESH_H_

#include "Animations/AnimationLayer.h"

// アニメーションメッシュクラス
class AnimatedMesh final {
private:
	// アニメーションレイヤー
	using Layer = AnimationLayer;
public:
	// コンストラクタ
	AnimatedMesh(GSuint mesh, GSuint motion = 0, bool loop = true);
	// コンストラクタ
	AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

	// レイヤーの登録
	void register_layer(
		size_t layer, const size_t bone_array[], size_t count, GSuint motion = 0, bool loop = true);
	// レイヤーの登録
	void register_layer(
		size_t layer, const size_t bone_array[], size_t count, GSuint animation, GSuint motion, bool loop);

	// イベントの追加
	void add_event(
		GSuint target_motion, float trigger_time, const std::function<void(void)>& event_callback, size_t layer = 0);
	// イベントの消去
	void clear_event(GSuint target_motion, size_t layer = 0);

	// モーションの変更
	void change_motion(GSuint motion, bool loop = true, float lerp_time = DEFAULT_LERP_TIME);
	// モーションの変更
	void change_motion(size_t layer, GSuint motion, bool loop = true, float lerp_time = DEFAULT_LERP_TIME);
	// モーションの強制変更
	void forced_change_motion(GSuint motion, bool loop = true, float lerp_time = DEFAULT_LERP_TIME);
	// モーションの強制変更
	void forced_change_motion(size_t layer, GSuint motion, bool loop = true, float lerp_time = DEFAULT_LERP_TIME);

	// 座標変換行列の設定
	void transform(const GSmatrix4& matrix);

	// モーション時間の取得
	float motion_time(size_t layer = 0) const noexcept;
	// モーション時間の設定
	void motion_time(float time, size_t layer = 0) noexcept;

	// モーション速度の設定
	void motion_speed(float scale);
	// モーション速度の設定
	void motion_speed(size_t layer, float scale) noexcept;

	// モーション終了時間の取得
	float motion_end_time(size_t layer = 0) const noexcept;
	// モーション終了フラグ
	bool is_motion_finished(size_t layer = 0) const noexcept;

	// ボーン変換行列の取得
	GSmatrix4 bone_matrix(int bone_no) const noexcept;
	// ローカル用ボーン変換行列の取得
	GSmatrix4 local_bone_matrix(int bone_no) const noexcept;

private:
	// メッシュID
	GSuint mesh_{ 0 };
	// スケルトンID
	GSuint skeleton_{ 0 };

	// 座標変換行列
	GSmatrix4 transform_matrix_{ GS_MATRIX4_IDENTITY };

	// アニメーションレイヤー配列
	std::vector<Layer> layers_;
	// アニメーションレイヤーインデックス配列
	std::vector<size_t> layer_indices_;

	// ボーン変換行列配列
	std::vector<GSmatrix4> bone_matrices_;
	// ローカル用ボーン変換行列配列
	std::vector<GSmatrix4> local_bone_matrices_;
};

#include "Inlines/AnimatedMesh.inl"

#endif // ANIMATIONS_ANIMATED_MESH_H_