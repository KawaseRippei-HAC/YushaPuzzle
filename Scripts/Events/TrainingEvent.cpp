#include "Events/TrainingEvent.h"

#include <gslib.h>

#include "Defines/Assets.h"
#include "Events/BuildingMover.h"
#include "Systems/LoggableRandomizer.h"
#include "Screens/ScreenManager.h"
#include "Characters/Heroes/HeroCharacter.h"

// パラメータ上昇値
static constexpr int ADD_PARAMATER{ 3 };

// 訓練に必要な費用
static constexpr int TRAINING_FEES{ 100 };

// 実行までの時間
static constexpr int TIME_UNTIL_EXECUTE{ 60 };

/*実装*/
// オブジェクト描画
void TrainingEvent::draw_object(const GSmatrix4& matrix) const {
	glPushMatrix();
	{
		const GSvector3 pos = matrix.position();
		glTranslatef(pos.x, pos.y, pos.z);
		gsDrawMesh(Mesh::TrainingBuilding);
	}
	glPopMatrix();
}

// 開始時処理
void TrainingEvent::on_enter(CharacterBase& character) {
	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	hero_character->set_event_type(Event::Training);

	ScreenManager::push_screen(ScreenID::BuildingExplanation);

	timer_ = 0;

	// 勇者のお金が足りているか
	const bool is_enough_money = character.gold() >= TRAINING_FEES;
	// 足りていれば実行
	is_finished_ = (is_enough_money) ? false: true;

	// 獲得アニメーションの追加
	if(!is_enough_money) hero_character->add_acquisition_animation(Texture::NotEnoughGold, RATIO_8X1, 0.5f);
}

// 実行
void TrainingEvent::execute(float delta_time, CharacterBase& character) {
	if (is_finished_) return;
	timer_ += delta_time;

	if (timer_ >= TIME_UNTIL_EXECUTE) {
		// お金の消費
		character.calc_gold(-TRAINING_FEES);
		// パラメータ上昇
		parameter_up(character);

		gsPlaySE(SE::StatusUp);

		// 終了
		is_finished_ = true;
	}
}

// 終了時処理
void TrainingEvent::on_exit(CharacterBase& character) {
}

// イベント種別の取得
EventType TrainingEvent::type() const noexcept {
	return EventType::Training;
}

// アイコンの取得
GSuint TrainingEvent::icon() const noexcept {
	return Texture::TrainingEventIcon;
}

// 終了フラグ
bool TrainingEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動中有効フラグ
bool TrainingEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	return true;
}

// 移動処理の取得
ICharacterMover* TrainingEvent::mover() noexcept {
	return &building_mover;
}

// パラメータ上昇
void TrainingEvent::parameter_up(CharacterBase& character) {

	// ランダムなパラメータ上昇
	const int parameter = LoggableRandomizer::generate(0, 4);

	GSuint animation_texture_id = Texture::HpUp;

	switch (parameter)
	{
	case 0: character.add_max_hp(ADD_PARAMATER * 10); animation_texture_id = Texture::HpUp; break;
	case 1: character.add_attack(ADD_PARAMATER); animation_texture_id = Texture::AttackUp; break;
	case 2: character.add_defense(ADD_PARAMATER); animation_texture_id = Texture::DefenseUp; break;
	case 3: character.add_speed(ADD_PARAMATER); animation_texture_id = Texture::SpeedUp; break;
	case 4: character.add_luck(ADD_PARAMATER); animation_texture_id = Texture::LuckUp; break;
	}

	// 勇者取得
	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	// 獲得アニメーションの追加
	hero_character->add_acquisition_animation(animation_texture_id, RATIO_4X1, 0.5f);
}
