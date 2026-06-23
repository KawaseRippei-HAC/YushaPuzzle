#ifndef TILES_TILE_RENDERER_H_
#define TILES_TILE_RENDERER_H_

#include <vector>
#include <utility>
#include <unordered_map>

#include <GStype.h>

#include "Tiles/TileNode.h"

// タイルレンダラークラス
class TileRenderer {
public:
	~TileRenderer();

	// 描画
	void draw(void) const;

	// インスタンスの追加
	void add_instance(GSuint mesh, const GSmatrix4& matrix);
	// インスタンスの更新
	void update_instances(GSuint mesh);

	// バッファの作成
	void create_buffers(size_t size);
	// バッファの消去
	void clear_buffers(void);

private:
	// バッファサイズ
	size_t buffer_size_{ 0 };
	// インスタンスマップ
	std::unordered_map<GSuint, std::vector<GSmatrix4>> instance_map_;
};

#endif // TILES_TILE_RENDERER_H_