#include "Systems/EffectSystem.h"

#include <GSeffect.h>

/*À‘•*/
// ‰Šú‰»
void EffectSystem::initialize() noexcept {
	gsInitEffect();
	is_alive_ = true;
}

// I—¹
void EffectSystem::finalize() noexcept {
	if (!is_alive_) return;
	gsStopAllEffects();
	gsFinishEffect();
	is_alive_ = false;
}

// ¶‘¶ƒtƒ‰ƒO
bool EffectSystem::is_alive() noexcept {
	return is_alive_;
}