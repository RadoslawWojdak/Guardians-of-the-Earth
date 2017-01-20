#ifndef characterAnimation_h
#define characterAnimation_h

#include "animatedObjectLevel.h"
#include "enums.h"

class cCharacterAnimation :public cAnimatedObjectLevel
{
protected:
	cCharacterAnimation(sf::Texture &texture, sf::Vector2f pos_on_map);

	void animationStanding();
	void animationWalking();
	void animationJumping();
	void animationClimbing(bool climbs_up);
	void animationSwimming();
	void animationSpecial1(short time_between_frames);
	void animationSpecial2(short time_between_frames);

	eCharacterAnimationType getAnimationType();
};

#endif //!characterAnimation_h