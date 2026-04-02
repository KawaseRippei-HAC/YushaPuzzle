#ifndef SYSTEMS_ASSET_LOADER_H_
#define SYSTEMS_ASSET_LOADER_H_

#include <functional>

#include "Systems/CsvReader.h"

// アセット読み込みクラス
class AssetLoader final {
public:
	// 読み込み
	static void load(const std::string& file_path, const std::function<void(void)>& loaded_callback = nullptr);
	// 消去
	static void clear(void);

private:
	// ハッシュ化
	static size_t to_hash(const std::string& asset_type);
	// アセットの読み込み
	static bool load_asset(const CsvRow& csv_row);
	// アセットの削除
	static void delete_asset(const CsvRow& csv_row);

	// アセットタイプの比較
	static bool compare_type(const std::string& type_str, const char* asset_type) {
		return type_str.find(asset_type) != std::string::npos;
	}

private:
	/*インスタンス生成禁止*/
	AssetLoader(void) = delete;

	// csvデータ
	static CsvReader csv_;
	// ロード済みファイルパス
	static std::string loaded_path_;
};

#endif // SYSTEMS_ASSET_LOADER_H_