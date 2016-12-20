#ifndef characterAnimation_h
#define characterAnimation_h

#include "animatedObjectLevel.h"

class cCharacterAnimation :public cAnimatedObjectLevel
{
protected:
	cCharacterAnimation(sf::Texture &texture, sf::Vector2f pos_on_map);

	void animationStanding();
	void animationWalking();
	void animationJumping();
	void animationClimbing(bool climbs_up);
	void animationSwimming();
};

#endif //!characterAnimation_h