#include "Characters/Heroes/HeroIdleState.h"

#include "Characters/Heroes/HeroCharacter.h"

/*実装*/
// 更新時処理
void HeroIdleState::on_update([[maybe_unused]] float delta_time, [[maybe_unused]] HeroContext& context) {

}

// 遷移時処理
void HeroIdleState::on_enter(HeroContext& context) {
	// 待機状態に入ったら行動終了
	context.is_actioning = false;
	// 待機モーションの再生
	context.character->mesh().change_motion(HeroMotion::Idle);
}