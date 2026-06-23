#ifndef INLINES_CSV_READER_INL_
#define INLINES_CSV_READER_INL_

#include "Systems/CsvReader.h"

/*インライン関数実装*/
// データの取得
inline std::string CsvReader::get(int row, int column) const noexcept {
	if (row > size() || column > size(row)) return "";
	return data_[row][column];
}

// データの取得
inline int CsvReader::geti(int row, int column) const {
	return std::stoi(get(row, column));
}

// データの取得
inline float CsvReader::getf(int row, int column) const {
	return std::stof(get(row, column));
}

// 行データの取得
inline const CsvRow& CsvReader::get_row(int row) const {
	return data_[row];
}

// 全データの取得
inline const Csv& CsvReader::data() const noexcept {
	return data_;
}

// 行数の取得
inline int CsvReader::size() const noexcept {
	return (int)data_.size();
}

// 列数の取得
inline int CsvReader::size(int row) const noexcept {
	return (int)data_[row].size();
}

#endif // INLINES_CSV_READER_INL_