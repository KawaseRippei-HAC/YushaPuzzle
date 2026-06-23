#include "Systems/AssetLoader.h"

#include <gslib.h>
#include <GSeffect.h>

/*静的メンバ変数*/
// csvデータ
CsvReader AssetLoader::csv_;
// ロード済みファイルパス
std::string AssetLoader::loaded_path_;

/*定数*/
// コメントアウト
static constexpr char COMMENT_HEADER{ '#' };

// アセットタイプインデックス
static constexpr size_t ASSET_TYPE{ 0 };
// アセットIDインデックス
static constexpr size_t ASSET_ID{ 1 };
// アセットパスインデックス
static constexpr size_t ASSET_PATH{ 2 };

// 最低データサイズ
static constexpr size_t MIN_DATA_SIZE{ 3 };

#pragma region // アセットハッシュ
// Exハッシュ
static constexpr size_t EX_HASH{ 'e' + 'x' };
// 固定ハッシュ
static constexpr size_t FIXED_HASH{ 'f' + 'i' + 'x' + 'e' + 'd' };

// テクスチャアセットハッシュ
static constexpr size_t TEXTURE_HASH{ 't' + 'e' + 'x' + 't' + 'u' + 'r' + 'e' };
// テクスチャExアセットハッシュ
static constexpr size_t TEXTURE_EX_HASH{ TEXTURE_HASH + EX_HASH };

// メッシュアセットハッシュ
static constexpr size_t MESH_HASH{ 'm' + 'e' + 's' + 'h' };
// メッシュExアセットハッシュ
static constexpr size_t MESH_EX_HASH{ MESH_HASH + EX_HASH };
// 固定メッシュアセットハッシュ
static constexpr size_t MESH_FIXED_HASH{ MESH_HASH + FIXED_HASH };

// スキンメッシュアセットハッシュ
static constexpr size_t SKINMESH_HASH{ 's' + 'k' + 'i' + 'n' + MESH_HASH };
// スキンメッシュExアセットハッシュ
static constexpr size_t SKINMESH_EX_HASH{ SKINMESH_HASH + EX_HASH };
// 固定スキンメッシュアセットハッシュ
static constexpr size_t SKINMESH_FIXED_HASH{ SKINMESH_HASH + FIXED_HASH };

// オクトリーアセットハッシュ
static constexpr size_t OCTREE_HASH{ 'o' + 'c' + 't' + 'r' + 'e' + 'e' };
// オクトリーExアセットハッシュ
static constexpr size_t OCTREE_EX_HASH{ OCTREE_HASH + EX_HASH };
// 固定オクトリーアセットハッシュ
static constexpr size_t OCTREE_FIXED_HASH{ OCTREE_HASH + FIXED_HASH };

// ライトマップアセットハッシュ
static constexpr size_t LIGHTMAP_HASH{ 'l' + 'i' + 'g' + 'h' + 't' + 'm' + 'a' + 'p' };

// リフレクションプローブアセットハッシュ
static constexpr size_t REFPROBE_HASH{ 'r' + 'e' + 'f' + 'p' + 'r' + 'o' + 'b' + 'e' };
// リフレクションプローブアセットハッシュ
static constexpr size_t REFLECTION_PROBE_HASH{ 'r' + 'e' + 'f' + 'l' + 'e' + 'c' + 't' + 'i' + 'o' + 'n' + 'p' + 'r' + 'o' + 'b' + 'e' };

// エフェクトアセットハッシュ
static constexpr size_t EFFECT_HASH{ 'e' + 'f' + 'f' + 'e' + 'c' + 't' };

// 効果音アセットハッシュ
static constexpr size_t SE_HASH{ 's' + 'e' };

// BGMアセットハッシュ
static constexpr size_t BGM_HASH{ 'b' + 'g' + 'm' };
#pragma endregion

#pragma region // その他
// 効果音同時再生数インデックス
static constexpr size_t SE_COUNT{ 3 };
// 効果音再生タイプフラグインデックス
static constexpr size_t SE_FLAG{ MIN_DATA_SIZE + 1 };
// 効果音データサイズ
static constexpr size_t SE_DATA_SIZE{ SE_FLAG };
// 効果音デフォルトフラグハッシュ
static constexpr size_t SE_DEFAULT_HASH{ 'd' + 'e' + 'f' + 'a' + 'u' + 'l' + 't' };
// 効果音ウェイトフラグハッシュ
static constexpr size_t SE_WAIT_HASH{ 'w' + 'a' + 'i' + 't' };
// 効果音ループフラグハッシュ
static constexpr size_t SE_LOOP_HASH{ 'l' + 'o' + 'o' + 'p' };

// BGMループフラグインデックス
static constexpr size_t BGM_LOOP{ 3 };
// BGMデータサイズ
static constexpr size_t BGM_DATA_SIZE{ BGM_LOOP + 1 };
#pragma endregion

/*実装*/
// アセットの読み込み
void AssetLoader::load(const std::string& file_path, const std::function<void()>& loaded_callback) {
	// 前回読み込んだパスと同じパスは読み込まない
	if (file_path == loaded_path_) return;
	// 前回読み込んだアセットを消去
	if (!loaded_path_.empty()) clear();

	// csvファイルの読み込み
	csv_.load(file_path);

	// ループカウンター
	int row_number = 0;
	// 全ての行を走査
	for (CsvRow row : csv_.data()) {
		row_number++;

		// 行自体が空、または行の最初のセルが空の場合は、不正な形式としてエラーをスロー
		if (row.empty() || row.front().empty()) {
			std::string error_message = file_path + " の "
				+ std::to_string(row_number) + " 行目: 不正な空行または不完全な行";
			throw std::runtime_error(error_message);
		}

		// コメントアウト行はスキップ
		if (row.front().front() == COMMENT_HEADER) continue;

		// アセットの読み込み
		if (load_asset(row)) {
			// 読み込み完了後コールバックを実行
			if (loaded_callback) loaded_callback();
		}
		else {
			clear();
			std::string error_massage = "Error in asset file '"
				+ file_path + "' at line " + std::to_string(row_number)
				+ ": Failed to load asset.";
			throw std::runtime_error(error_massage);
		}
	}

	// 読み込み済みファイルパスの設定
	loaded_path_ = file_path;
}

// 消去
void AssetLoader::clear() {
	// CSVデータがなければ何もしない
	if (csv_.data().empty()) return;

	// アセットを逆順に削除
	for (int i = csv_.size() - 1; i >= 0; --i) {
		const CsvRow row = csv_.get_row(i);
		// 安全性のため、行が空、または最初のセルが空の場合はスキップ
		// (リソース解放中にエラーで停止するのは避ける)
		if (row.empty() || row.front().empty()) continue;

		// コメントアウト行はスキップ (このチェックは上記の後なら安全)
		if (row.front().front() == COMMENT_HEADER) continue;

		delete_asset(row);
	}

	// 読み込み済みパスの消去
	loaded_path_.clear();
	// csvの消去
	csv_.clear();
}

// ハッシュ化
size_t AssetLoader::to_hash(const std::string& asset_type) {
	size_t result{ 0 };
	for (const char value : asset_type) {
		result += (size_t)std::tolower((int)value);
	}
	return result;
}

// アセットの読み込み
bool AssetLoader::load_asset(const CsvRow& csv_row) {
	// 要素が足りなければ失敗
	const size_t data_size = csv_row.size();
	if (data_size < MIN_DATA_SIZE) return false;

	// アセットハッシュの取得
	const size_t hash = to_hash(csv_row[ASSET_TYPE]);
	// アセットIDの取得
	const GSuint id = (GSuint)std::stoi(csv_row[ASSET_ID]);
	// パスの取得
	const char* path = csv_row[ASSET_PATH].c_str();

	// 効果音の読み込み
	if (hash == SE_HASH) {
		if (data_size < SE_DATA_SIZE) return false;

		// 同時再生数の取得
		const int count = std::stoi(csv_row[SE_COUNT]);

		// 再生時フラグの取得
		GWAVEFLAG se_flag{ GWAVE_DEFAULT };
		for (size_t i = SE_FLAG; i < csv_row.size(); ++i) {
			if (se_flag >= (GWAVE_WAIT | GWAVE_LOOP)) break;

			const size_t flag_hash = to_hash(csv_row[i]);
			if (flag_hash == SE_DEFAULT_HASH) break;
			switch (flag_hash) {
			case SE_WAIT_HASH: se_flag |= GWAVE_WAIT; break;
			case SE_LOOP_HASH: se_flag |= GWAVE_LOOP; break;
			}
		}

		return (gsLoadSE(id, path, count, se_flag) == GS_TRUE);
	}
	// BGMの読み込み
	else if (hash == BGM_HASH) {
		// ループフラグの取得
		bool loop{ false };
		if (csv_row.size() >= BGM_DATA_SIZE) {
			std::string is_loop = csv_row[BGM_LOOP];
			std::transform(is_loop.begin(), is_loop.end(), is_loop.begin(), std::tolower);
			loop = (is_loop == "true" || is_loop == "loop");
		}
		return (gsLoadBGM(id, path, (BOOL)loop) == GS_TRUE);
	}

	// その他の読み込み
	switch (hash) {
	case TEXTURE_HASH:
		// 通常テクスチャの読み込み
		return (gsLoadTexture(id, path) == GS_TRUE);

	case MESH_HASH:
		// 通常メッシュの読み込み
		return (gsLoadMesh(id, path) == GS_TRUE);
	case MESH_EX_HASH:
		// メッシュExの読み込み
		return (gsLoadMeshEx(id, path) == GS_TRUE);
	case MESH_FIXED_HASH:
		// 固定メッシュの読み込み
		return (gsLoadMeshFixed(id, path) == GS_TRUE);

	case SKINMESH_HASH:
		// 通常スキンメッシュの読み込み
		return (gsLoadSkinMesh(id, path) == GS_TRUE);
	case SKINMESH_EX_HASH:
		// スキンメッシュExの読み込み
		return (gsLoadSkinMeshEx(id, path) == GS_TRUE);
	case SKINMESH_FIXED_HASH:
		// 固定スキンメッシュの読み込み
		return (gsLoadSkinMeshFixed(id, path) == GS_TRUE);

	case OCTREE_HASH:
		// 通常オクトリーの読み込み
		return (gsLoadOctree(id, path) == GS_TRUE);
	case OCTREE_EX_HASH:
		// オクトリーExの読み込み
		return (gsLoadOctreeEx(id, path) == GS_TRUE);
	case OCTREE_FIXED_HASH:
		// 固定オクトリーの読み込み
		return (gsLoadOctreeFixed(id, path) == GS_TRUE);

	case LIGHTMAP_HASH:
		// ライトマップの読み込み
		return (gsLoadLightmap(id, path) == GS_TRUE);

	case REFPROBE_HASH:
	case REFLECTION_PROBE_HASH:
		// リフレクションプローブの読み込み
		return (gsLoadReflectionProbe(id, path) == GS_TRUE);

	case EFFECT_HASH:
		// エフェクトの読み込み
		return (gsLoadEffect(id, path) == GS_TRUE);
	}

	// 読み込みに失敗
	return false;
}

// アセットの削除
void AssetLoader::delete_asset(const CsvRow& csv_row) {
	// アセットハッシュの取得
	const size_t hash = to_hash(csv_row[ASSET_TYPE]);
	// アセットIDの取得
	const GSuint id = (GSuint)std::stoi(csv_row[ASSET_ID]);

	// アセットの削除
	switch (hash) {
	case TEXTURE_HASH:
	case TEXTURE_EX_HASH:
		gsDeleteTexture(id); return;
	case MESH_HASH:
	case MESH_EX_HASH:
	case MESH_FIXED_HASH:
		gsDeleteMesh(id); return;
	case SKINMESH_HASH:
	case SKINMESH_EX_HASH:
	case SKINMESH_FIXED_HASH:
		gsDeleteSkinMesh(id); return;
	case OCTREE_HASH:
	case OCTREE_EX_HASH:
	case OCTREE_FIXED_HASH:
		gsDeleteOctree(id); return;
	case LIGHTMAP_HASH:
		gsDeleteLightmap(id); return;
	case REFPROBE_HASH:
	case REFLECTION_PROBE_HASH:
		gsDeleteReflectionProbe(id); return;
	case EFFECT_HASH:
		gsDeleteEffect(id); return;
	case SE_HASH:
		gsDeleteSE(id); return;
	case BGM_HASH:
		gsDeleteBGM(id); return;
	}
}