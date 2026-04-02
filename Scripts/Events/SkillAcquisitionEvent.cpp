#include "Events/SkillAcquisitionEvent.h"

#include <gslib.h>

#include "Defines/Assets.h"
#include "Events/BuildingMover.h"
#include "Characters/Heroes/HeroCharacter.h"
#include "Skills/SkillTable.h"
#include "Systems/LoggableRandomizer.h"
#include "Screens/ScreenManager.h"

// パラメータ上昇値
static constexpr int ADD_PARAMATER{ 3 };

// HPの消費量
static constexpr float HP_CONSUMPTION{ 0.3f };

// 実行までの時間
static constexpr int TIME_UNTIL_EXECUTE{ 60 };

/*実装*/
// オブジェクト描画
void SkillAcquisitionEvent::draw_object(const GSmatrix4& matrix) const {
	glPushMatrix();
	{
		const GSvector3 pos = matrix.position();
		glTranslatef(pos.x, pos.y, pos.z);
		gsDrawMesh(Mesh::MasterHouseBuilding);
	}
	glPopMatrix();
}

// 開始時処理
void SkillAcquisitionEvent::on_enter(CharacterBase& character) {
	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	hero_character->set_event_type(Event::SkillAcquisition);
	ScreenManager::push_screen(ScreenID::BuildingExplanation);

	timer_ = 0;
	is_finished_ = false;

	add_skill_id_ = add_skill_id(hero_character);

	// 獲得できるスキルがない時は実行しない
	if (add_skill_id_ == SkillId::None) {
		is_finished_ = true;
	}
	// HPが足りない時は実行しない
	else if (character.hp() <= character.max_hp() * HP_CONSUMPTION) {
		// 獲得アニメーションの追加
		hero_character->add_acquisition_animation(Texture::NotEnoughHP, RATIO_8X1, 0.5f);
		is_finished_ = true;
	}
}

// 実行
void SkillAcquisitionEvent::execute(float delta_time, CharacterBase& character) {
	if (is_finished_) return;
	timer_ += delta_time;

	if (timer_ >= TIME_UNTIL_EXECUTE) {
		// HPを消費
		character.take_damage(character.max_hp() * HP_CONSUMPTION);
		// 勇者取得
		HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
		// スキル獲得
		hero_character->skill_actor()->add_skill(add_skill_id_);
		// 獲得アニメーションの追加
		hero_character->add_acquisition_animation(SkillTable::get_skill_data(add_skill_id_)->texture, RATIO_1X1);

		// バフ設定
		hero_character->modifiers().set_buff(hero_character->skill_actor()->non_battle_skill().buffs);

		gsPlaySE(SE::SkillActivation);
		// 終了
		is_finished_ = true;
	}
}

// 終了時処理
void SkillAcquisitionEvent::on_exit(CharacterBase& character) {
}

// イベント種別の取得
EventType SkillAcquisitionEvent::type() const noexcept {
	return EventType::SkillAcquisition;
}

// アイコンの取得
GSuint SkillAcquisitionEvent::icon() const noexcept {
	return Texture::MasterHouseEventIcon;
}

// 終了フラグ
bool SkillAcquisitionEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動中有効フラグ
bool SkillAcquisitionEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	return true;
}

// 移動処理の取得
ICharacterMover* SkillAcquisitionEvent::mover() noexcept {
	return &building_mover;
}

// スキル獲得
SkillId SkillAcquisitionEvent::add_skill_id(HeroCharacter* hero_charcter) {
	// 全スキルID
	std::vector<SkillId> all_skills = {
		SkillId::Rigidity,
		SkillId::ReinforcingBar,
		SkillId::Blessing,
		SkillId::Agile,
		SkillId::GoodLuck,
		SkillId::Intimidation
	};

	// ランダムでスキルを追加
	if (!all_skills.empty()) {
		int max = all_skills.size() - 1;
		while (max >= 0)
		{
			// ランダムなスキル
			int skill_id = LoggableRandomizer::generate(0, max);
			SkillId find_skill = all_skills[skill_id];
			// スキル検索
			if (hero_charcter->skill_actor()->is_addable_skill(find_skill)) {
				return find_skill;
			}
			// スキルを追加出来なかった場合
			// 今回判定したスキルは削除
			all_skills[skill_id] = all_skills.back();
			all_skills.pop_back();
			--max;
		}
	}

	return SkillId::None;
}
