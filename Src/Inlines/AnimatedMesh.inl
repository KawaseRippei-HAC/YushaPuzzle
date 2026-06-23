#ifndef INLINES_ANIMATED_MESH_INL_
#define INLINES_ANIMATED_MESH_INL_

#include "Animations/AnimatedMesh.h"

/*インライン関数実装*/
// レイヤーボーンの設定
inline void AnimatedMesh::register_layer(
	size_t layer, const size_t bone_array[], size_t count, GSuint motion, bool loop) {
	register_layer(layer, bone_array, count, mesh_, motion, loop);
}

// イベントの追加
inline void AnimatedMesh::add_event(
	GSuint target_motion, float trigger_time, const std::function<void()>& event_callback, size_t layer) {
	if (layer >= layers_.size()) return;
	layers_[layer].add_event(target_motion, trigger_time, event_callback);
}

// イベントの消去
inline void AnimatedMesh::clear_event(GSuint target_motion, size_t layer) {
	if (layer >= layers_.size()) return;
	layers_[layer].clear_event(target_motion);
}

// モーションの変更
inline void AnimatedMesh::change_motion(size_t layer, GSuint motion, bool loop, float lerp_time) {
	if (layer >= layers_.size()) return;
	layers_[layer].change_motion(motion, loop, lerp_time);
}

// モーションの強制変更
inline void AnimatedMesh::forced_change_motion(size_t layer, GSuint motion, bool loop, float lerp_time) {
	if (layer >= layers_.size()) return;
	layers_[layer].forced_change_motion(motion, loop, lerp_time);
}

// モーション時間の取得
inline float AnimatedMesh::motion_time(size_t layer) const noexcept {
	if (layer >= layers_.size()) return 0.0f;
	return layers_[layer].motion_time();
}

// モーション時間の設定
inline void AnimatedMesh::motion_time(float time, size_t layer) noexcept {
	if (layer >= layers_.size()) return;
	layers_[layer].motion_time(time);
}

// モーション速度の設定
inline void AnimatedMesh::motion_speed(size_t layer, float scale) noexcept {
	if (layer >= layers_.size()) return;
	layers_[layer].motion_speed(scale);
}

// モーション終了時間の取得
inline float AnimatedMesh::motion_end_time(size_t layer) const noexcept {
	if (layer >= layers_.size()) return 0.0f;
	return layers_[layer].motion_end_time();
}

// モーション終了フラグ
inline bool AnimatedMesh::is_motion_finished(size_t layer) const noexcept {
	if (layer >= layers_.size()) return true;
	return layers_[layer].is_motion_finished();
}

// ボーン変換行列の取得
inline GSmatrix4 AnimatedMesh::bone_matrix(int bone_no) const noexcept {
	if ((size_t)bone_no >= bone_matrices_.size()) return GS_MATRIX4_IDENTITY;
	return bone_matrices_[bone_no];
}

// ローカル用ボーン変換行列の取得
inline GSmatrix4 AnimatedMesh::local_bone_matrix(int bone_no) const noexcept {
	if ((size_t)bone_no >= local_bone_matrices_.size()) return GS_MATRIX4_IDENTITY;
	return local_bone_matrices_[bone_no];
}

#endif // INLINES_ANIMATED_MESH_INL_