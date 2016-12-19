#include "characterAnimation.h"

const short TIME_ANIMATION_WALKING = 7;
const short TIME_ANIMATION_STANDING = 7;
const short TIME_ANIMATION_JUMPING = 7;
const short TIME_ANIMATION_CLIMBING = 7;
const short TIME_ANIMATION_SWIMMING = 7;

cCharacterAnimation::cCharacterAnimation(sf::Texture &texture, sf::Vector2f pos_on_map)
	:cAnimatedObjectLevel(texture, pos_on_map, sf::Vector2f(22.0f, 32.0f), TIME_ANIMATION_STANDING)
{
	;
}

void cCharacterAnimation::animationStanding()
{
	this->setAnimationStartPosition(sf::Vector2f(66.0f, 0.0f), TIME_ANIMATION_STANDING);
	this->animation();
}

void cCharacterAnimation::animationWalking()
{
	this->setAnimationStartPosition(sf::Vector2f(0.0f, 32.0f), TIME_ANIMATION_WALKING);
	this->animation();
}

void cCharacterAnimation::animationJumping()
{
	this->setAnimationStartPosition(sf::Vector2f(66.0f, 64.0f), TIME_ANIMATION_JUMPING);
	this->animation();
}

void cCharacterAnimation::animationClimbing(bool climbs_up)
{
	this->setAnimationStartPosition(sf::Vector2f(44.0f, 96.0f), TIME_ANIMATION_CLIMBING);
	this->animation(!climbs_up);
}

void cCharacterAnimation::animationSwimming()
{
	this->setAnimationStartPosition(sf::Vector2f(0.0f, 128.0f), TIME_ANIMATION_SWIMMING);
	this->animation();
}