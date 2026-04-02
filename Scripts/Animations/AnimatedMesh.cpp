#include "Animations/AnimatedMesh.h"

#include <cmath>
#include <algorithm>

/*定義*/
// モーション最終フレーム
#define LAST_FRAME(end_time) (end_time - 1.0f)

/*定数*/
// 標準ボーン数
static constexpr size_t DEFAULT_NUM_BONES{ 256 };

/*実装*/
// コンストラクタ
AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint motion, bool loop) :
	AnimatedMesh{ mesh, mesh, mesh, motion, loop } {
}

// コンストラクタ
AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop) :
	mesh_{ mesh }, skeleton_{ skeleton } {
	// レイヤーの初期化
	layers_.emplace_back(Layer{ animation, motion, loop });

	// スケルトンの初期化
	const size_t bone_count = (gsGetSkeleton(skeleton_)) ? gsGetSkeletonNumBones(skeleton_) : DEFAULT_NUM_BONES;
	for (size_t i = 0; i < bone_count; ++i) {
		layer_indices_.emplace_back(0);
		bone_matrices_.emplace_back(GS_MATRIX4_IDENTITY);
		local_bone_matrices_.emplace_back(GS_MATRIX4_IDENTITY);
	}
}

// 更新
void AnimatedMesh::update(float delta_time) {
	for (Layer& layer : layers_) {
		layer.update(delta_time);
	}
}

// 描画
void AnimatedMesh::draw() const {
	// スケルトンの設定
	gsDisable(GS_CALC_SKELETON);
	gsBindSkeleton(skeleton_);
	gsSetMatrixSkeleton(bone_matrices_.data());

	// メッシュの描画
	glPushMatrix();
	{
		glMultMatrixf(transform_matrix_);
		gsDrawMesh(mesh_);
	}
	glPopMatrix();

	// スケルトンの自動計算有効化
	gsEnable(GS_CALC_SKELETON);
}

// レイヤーボーンの設定
void AnimatedMesh::register_layer(
	size_t layer, const size_t bone_array[], size_t count, GSuint animation, GSuint motion, bool loop) {
	if (layer >= layers_.size()) {
		layers_.emplace_back(Layer{ animation, motion, loop });
	}

	for (size_t i = 0; i < count; ++i) {
		const size_t bone = bone_array[i];
		if (bone < layer_indices_.size()) layer_indices_[bone] = layer;
	}
}

// モーションの変更
void AnimatedMesh::change_motion(GSuint motion, bool loop, float lerp_time) {
	for (Layer& layer : layers_) {
		layer.change_motion(motion, loop, lerp_time);
	}
}

// モーションの強制変更
void AnimatedMesh::forced_change_motion(GSuint motion, bool loop, float lerp_time) {
	for (Layer& layer : layers_) {
		layer.forced_change_motion(motion, loop, lerp_time);
	}
}

// 座標変換行列の設定
void AnimatedMesh::transform(const GSmatrix4& matrix) {
	transform_matrix_ = matrix;

	// 補完の計算
	const size_t bone_count = local_bone_matrices_.size();
	for (size_t i = 0; i < bone_count; ++i) {
		local_bone_matrices_[i] = layers_[layer_indices_[i]].local_bone_matrix(i);
	}

	// スケルトンの計算
	gsBindSkeleton(skeleton_);
	gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

// モーション速度の設定
void AnimatedMesh::motion_speed(float scale) {
	for (Layer& layer : layers_) {
		layer.motion_speed(scale);
	}
}