#ifndef SYSTEMS_CSV_READER_H_
#define SYSTEMS_CSV_READER_H_

#include <string>
#include <vector>

// csv行
using CsvRow = std::vector<std::string>;
// csv行列
using Csv = std::vector<CsvRow>;

// csvリーダークラス
class CsvReader final {
public:
	CsvReader(void) = default;
	CsvReader(const std::string& file_path);

	// csvファイルの読み込み
	void load(const std::string& file_path);

	// データの取得
	std::string get(int row, int column) const noexcept;
	// データの取得
	int geti(int row, int column) const;
	// データの取得
	float getf(int row, int column) const;

	// 行データの取得
	const CsvRow& get_row(int row) const;

	// 全データの取得
	const Csv& data(void) const noexcept;

	// 行数の取得
	int size(void) const noexcept;
	// 列数の取得
	int size(int row) const noexcept;

	// 消去
	void clear(void);

private:
	// csvデータ
	Csv data_;
};

#include "Inlines/CsvReader.inl"

#endif // SYSTEMS_CSV_READER_H_