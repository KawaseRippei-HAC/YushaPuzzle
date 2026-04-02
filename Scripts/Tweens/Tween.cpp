#include "Tweens/Tween.h"

/*静的メンバ変数*/
// Tweenユニットリスト
std::list<TweenUnit*> Tween::unit_list_;

/*実装*/
// 更新
void Tween::update(float delta_time) {
	const auto end_it = unit_list_.end();
	for (auto unit_it = unit_list_.begin(); unit_it != end_it;) {
		(*unit_it)->update(delta_time);

		if ((*unit_it)->is_finished()) {
			delete* unit_it;
			*unit_it = nullptr;
			unit_it = unit_list_.erase(unit_it);
		}
		else ++unit_it;
	}
}

// 消去
void Tween::clear() {
	const auto end_it = unit_list_.rend();
	for (auto unit_it = unit_list_.rbegin(); unit_it != end_it; ++unit_it) {
		delete* unit_it;
		*unit_it = nullptr;
	}
	unit_list_.clear();
}

// キャンセル
void Tween::cancel(const TweenUnit& unit) {
	const auto end_it = unit_list_.end();
	for (auto unit_it = unit_list_.begin(); unit_it != end_it; ++unit_it) {
		if (*unit_it != &unit) continue;
		delete* unit_it;
		*unit_it = nullptr;
		unit_list_.erase(unit_it);
		return;
	}
}

// キャンセル
void Tween::cancel(const std::string& name) {
	const auto end_it = unit_list_.end();
	for (auto unit_it = unit_list_.begin(); unit_it != end_it;) {
		if ((*unit_it)->name() == name) {
			delete* unit_it;
			*unit_it = nullptr;
			unit_it = unit_list_.erase(unit_it);
		}
		else ++unit_it;
	}
}