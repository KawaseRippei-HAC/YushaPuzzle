#include "Tiles/TileRenderer.h"

#include <gslib.h>

#include "Defines/Assets.h"

/*定数*/
// 最大メッシュ数
static constexpr size_t MAX_MESH_QUANTITY{ 10 };
// 角ブロック数
static constexpr size_t CORNER_BLOCK_QUANTITY{ 4 };

/*実装*/
// デストラクタ
TileRenderer::~TileRenderer() {
	this->clear_buffers();
}

// 描画
void TileRenderer::draw() const {
	// 現在のシェーダーを退避
	const GSuint shader_tmp = gsGetBindDefaultMeshShader();

	// インスタンスドメッシュの描画
	gsBindDefaultMeshShader(Shader::TileInstanced);
	for (const auto& [mesh, matrices] : instance_map_) {
		gsBindStorageBuffer(mesh, NULL);
		// 描画サイズをクランプ
		const GSuint num_instance = std::min(matrices.size(), buffer_size_);
		gsDrawMeshInstanced(mesh, num_instance);
	}
	gsBindDefaultMeshShader(shader_tmp);
}

// インスタンスの追加
void TileRenderer::add_instance(GSuint mesh, const GSmatrix4& matrix) {
	instance_map_[mesh].push_back(matrix);
}

// インスタンスの更新
void TileRenderer::update_instances(GSuint mesh) {
	gsUpdateStorageBuffer(mesh, instance_map_[mesh].data());
}

// バッファの作成
void TileRenderer::create_buffers(size_t size) {
	buffer_size_ = size;
	for (const auto& [mesh, matrices] : instance_map_) {
		gsCreateStorageBuffer(mesh, sizeof(GSmatrix4) * size, matrices.data());
	}
}

// バッファの消去
void TileRenderer::clear_buffers() {
	for (const auto& [mesh, matrices] : instance_map_) {
		gsDeleteStorageBuffer(mesh);
	}
	buffer_size_ = 0;
}