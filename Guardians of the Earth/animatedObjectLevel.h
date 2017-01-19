#ifndef animatedObjectLevel_h
#define animatedObjectLevel_h

#include "objectlevel.h"

class cAnimatedObjectLevel :public cObjectLevel
{
	sf::Vector2f size_of_one_frame_image;
	sf::Vector2f start_position;		//Pozycja pocz¹tkowa (taka od której zaczyna siê animacja (prawy-górny róg))
	short frames;
	short current_frame;
	short time_between_frames;
	short time_to_next_frame;

protected:
	cAnimatedObjectLevel(sf::Texture &texture, sf::Vector2f pos_on_map, sf::Vector2f size_of_one_frame_image, short time_between_frames);

	void animation(bool from_behind = false);

	void setFrame(short frame);
	void prevFrame();
	void nextFrame();

	void setAnimationStartPosition(sf::Vector2f pos, short time_between_frames);
	void setTimeBetweenFrames(short time);

	sf::Vector2f getAnimationStartPosition();
	short getCurrentFrame();
	bool isAnimationBeginsAgain();
	bool isEndOfLastFrame();
};

#endif //!animatedObjectLevel_h