#ifndef INLINES_CHARACTER_PARAMETER_INL_
#define INLINES_CHARACTER_PARAMETER_INL_

#include "Characters/CharacterParameter.h"

/*ƒCƒ“ƒ‰ƒCƒ“ŠÖ”À‘•*/
// –¼‘O‚Ìæ“¾
inline const std::string& CharacterParameter::name() const noexcept {
	return name_;
}

// ƒŒƒxƒ‹‚Ìæ“¾
inline level_t CharacterParameter::level() const noexcept {
	return level_;
}

// HP‚Ìæ“¾
inline point_t CharacterParameter::hp() const noexcept {
	return hp_;
}

// Å‘åHP‚Ìæ“¾
inline point_t CharacterParameter::max_hp() const noexcept {
	return max_hp_;
}

// UŒ‚—Í‚Ìæ“¾
inline attack_t CharacterParameter::attack() const noexcept {
	return attack_;
}

// –hŒä—Í‚Ìæ“¾
inline defense_t CharacterParameter::defense() const noexcept {
	return defense_;
}

// MP‚Ìæ“¾
inline point_t CharacterParameter::mp() const noexcept {
	return mp_;
}

// Å‘åMP‚Ìæ“¾
inline point_t CharacterParameter::max_mp() const noexcept {
	return max_mp_;
}

// –‚–@UŒ‚—Í‚Ìæ“¾
inline attack_t CharacterParameter::magic_attack() const noexcept {
	return magic_attack_;
}

// –‚–@–hŒä—Í‚Ìæ“¾
inline defense_t CharacterParameter::magic_defense() const noexcept {
	return magic_defense_;
}

// ‘f‘‚³‚Ìæ“¾
inline speed_t CharacterParameter::speed() const noexcept {
	return speed_;
}

// ‰^‚Ìæ“¾
inline luck_t CharacterParameter::luck() const noexcept {
	return luck_;
}

// ‡ŒvUŒ‚—Í‚Ìæ“¾
inline attack_t CharacterParameter::total_attack(void) const noexcept {
	return attack_;
}

// ‡Œv–hŒä—Í‚Ìæ“¾
inline defense_t CharacterParameter::total_defense(void) const noexcept {
	return defense_;
}


#endif // INLINES_CHARACTER_PARAMETER_INL_