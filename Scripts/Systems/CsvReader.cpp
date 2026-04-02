#include "Systems/CsvReader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

/*実装*/
// コンストラクタ
CsvReader::CsvReader(const std::string& file_path) {
	this->load(file_path);
}

// csvファイルの読み込み
void CsvReader::load(const std::string& file_path) {
	// ファイルオープン
	std::ifstream file{ file_path };
	if (!file) throw std::runtime_error("csvファイルを開けませんでした。");

	// 既存データの消去
	if (!data_.empty()) data_.clear();

	// 1行ずつ取得
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss{ line };
		std::string cell;
		CsvRow row;
		// カンマで区切り、行に追加
		while (std::getline(ss, cell, ',')) {
			row.push_back(cell);
		}
		data_.push_back(row);
	}

	// ファイルクローズ
	file.close();
}

// 消去
void CsvReader::clear() {
	data_.clear();
}