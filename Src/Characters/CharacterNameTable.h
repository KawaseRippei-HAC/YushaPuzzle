#ifndef CHARACTER_NAME_TABLE_H_
#define CHARACTER_NAME_TABLE_H_

#include <string>
#include <map>

// キャラクターIDの定義
enum CharacterID {
	HeroId = 1,
	SlimeId = 2,
	PhantomId = 3,
	GolemId = 4,
	GrilledChicken = 5,
	Orc = 6,
	Skeleton = 7
};

// IDと名前を対応させるクラス
class CharacterNameTable {
public:
	// IDに対応する名前を返す
	static std::string get_name(int id) {
		auto it = names_.find(id);
		if (it != names_.end()) {
			return it->second;
		}
		return "不明なキャラクター";
	}

private:
	// IDと名前の対応表
	inline static const std::unordered_map<int, std::string >names_{
		{CharacterID::HeroId, "勇者"},
		{CharacterID::SlimeId, "スライム"},
		{CharacterID::PhantomId,"ファントム"},
		{CharacterID::GolemId,"ゴーレム"},
		{CharacterID::GrilledChicken,"焼鳥"},
		{CharacterID::Orc,"オーク"},
		{CharacterID::Skeleton	,"スケルトン"}
	};
};

#endif // CHARACTER_NAME_TABLE_H_
