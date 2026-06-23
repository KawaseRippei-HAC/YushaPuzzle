#ifndef DATA_RESULT_DATA_H_
#define DATA_RESULT_DATA_H_

#include "Characters/CharacterParameter.h"
#include "Skills/SkillDefinition.h"

class ResultData {
public:
	// レベルを保存
	static void save_level(level_t level);
	// ステータスを保存
	static void save_status(point_t hp, point_t max_hp, attack_t attack, defense_t defense, speed_t speed, luck_t luck);
	// スキルを保存
	static void save_skills(std::vector<SkillState> skills);

	// レベルを取得
	static level_t level();

	// 最大HPを取得
	static point_t hp();
	// HPを取得
	static point_t max_hp();
	// 攻撃力を取得
	static attack_t attack();
	// 防御力を取得
	static defense_t defense();
	// 素早さを取得
	static speed_t speed();
	// 運を取得
	static luck_t luck();

	// スキルを取得
	static std::vector<SkillState> skills();

private:
	// レベル
	inline static level_t level_{ 0 };

	// パラメータ
	inline static point_t hp_{ 0 };
	inline static point_t max_hp_{ 0 };
	inline static attack_t attack_{ 0 };
	inline static defense_t defense_{ 0 };
	inline static speed_t speed_{ 0 };
	inline static luck_t luck_{ 0 };

	// スキル
	inline static std::vector<SkillState> skills_;
};


#endif
