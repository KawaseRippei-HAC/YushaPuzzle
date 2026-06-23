#include "Events/InnEvent.h"

#include "Defines/Assets.h"
#include "Events/BuildingMover.h"
#include "Screens/ScreenManager.h"
#include "Characters/Heroes/HeroCharacter.h"

/*定数*/
// HP回復割合
static constexpr float HP_RECOVERY_RATE{ 0.7f };

// 実行までの時間
static constexpr int TIME_UNTIL_EXECUTE{ 60 };

/*実装*/
// 開始時処理
void InnEvent::on_enter(CharacterBase& character) {
	is_finished_ = false;

	std::cout << "宿屋に入った" << std::endl;

	HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
	hero_character->set_event_type(Event::Inn);

	// 説明画面表示
	ScreenManager::push_screen(ScreenID::BuildingExplanation);

	//HPが満タンか
	if (character.hp() == character.max_hp()) {
		state_ = HpMax;
	}
	// お金が払えるか
	else if (character.gold() < ACCOMMODATION_FEE) {
		state_ = LowOnGold;
	}
	else {
		state_ = CanPay;
	}
}

// 実行
void InnEvent::execute(float delta_time, CharacterBase& character) {
	if (is_finished_) return;
	timer_ += delta_time;


	if (timer_ >= TIME_UNTIL_EXECUTE) {
		HeroCharacter* hero_character = dynamic_cast<HeroCharacter*>(&character);
		switch (state_)
		{
		case HpMax: hp_max_execute(delta_time, character); break;
		case LowOnGold:
			low_on_gold_execute(delta_time, character);
			hero_character->add_acquisition_animation(Texture::NotEnoughGold, RATIO_8X1, 0.5f);
			break;
		case CanPay: can_pay_execute(delta_time, character); break;
		default: break;
		}


		is_finished_ = true;
	}
}

// 終了時処理
void InnEvent::on_exit(CharacterBase& character) {
	std::cout << "宿屋を出た" << std::endl;
}

// オブジェクトの描画
void InnEvent::draw_object(const GSmatrix4& matrix) const {
	glPushMatrix();
	{
		const GSvector3 pos = matrix.position();
		glTranslatef(pos.x, pos.y, pos.z);
		gsDrawMesh(Mesh::InnBuilding);
	}
	glPopMatrix();
}

// イベント種別の取得
EventType InnEvent::type() const noexcept {
	return EventType::Inn;
}

// アイコンの取得
GSuint InnEvent::icon() const noexcept {
	return Texture::InnEventIcon;
}

// 終了フラグ
bool InnEvent::is_finished() const noexcept {
	return is_finished_;
}

// 移動中有効フラグ
bool InnEvent::is_activate_in_moving(CharacterBase& character) const noexcept {
	return true;
}

// 移動処理の取得
ICharacterMover* InnEvent::mover() noexcept {
	return &building_mover;
}

void InnEvent::hp_max_execute(float delta_time, CharacterBase& character) {
	std::cout << "HPが満タン" << std::endl;
}

void InnEvent::low_on_gold_execute(float delta_time, CharacterBase& character) {
	std::cout << "ゴールドが足りない" << std::endl;
}

void InnEvent::can_pay_execute(float delta_time, CharacterBase& character) {
	point_t healed_hp = 0;
	// HP回復
	healed_hp = character.heal(character.max_hp() * HP_RECOVERY_RATE);
	gsPlaySE(SE::Heal);
	// お金を払う
	character.calc_gold(-ACCOMMODATION_FEE);
	std::cout << ACCOMMODATION_FEE << "G 払って " << healed_hp << "回復した" << std::endl;
}