#include "animatedObjectLevel.h"
#include <iostream>
cAnimatedObjectLevel::cAnimatedObjectLevel(sf::Texture &texture, sf::Vector2f pos_on_map, sf::Vector2f size_of_one_frame_image, short time_between_frames)
	:cObjectLevel()
{
	this->adjustGraphicsParameters(texture, pos_on_map);

	this->start_position = sf::Vector2f(0.0f, 0.0f);
	this->size_of_one_frame_image = size_of_one_frame_image;
	this->frames = this->getTexture()->getSize().x / size_of_one_frame_image.x;
	this->time_between_frames = time_between_frames;

	this->current_frame = 0;
	this->time_to_next_frame = time_between_frames;
}

void cAnimatedObjectLevel::animation(bool from_behind)
{
	this->time_to_next_frame--;

	if (this->time_to_next_frame <= 0)
	{
		if (from_behind)
			this->prevFrame();
		else
			this->nextFrame();

		this->time_to_next_frame = this->time_between_frames;
	}
}

void cAnimatedObjectLevel::prevFrame()
{
	if (this->current_frame > 0)
		this->setFrame(this->current_frame - 1);
	else
		this->setFrame(this->frames - 1);
}

void cAnimatedObjectLevel::nextFrame()
{
	if (this->current_frame < this->frames - 1)
		this->setFrame(this->current_frame + 1);
	else
		this->setFrame(0);
}

void cAnimatedObjectLevel::setFrame(short frame)
{
	if (frame >= 0 && frame < this->frames)
	{
		this->current_frame = frame;
		this->setTextureRect(sf::IntRect(this->start_position.x + frame * this->size_of_one_frame_image.x, this->start_position.y, this->size_of_one_frame_image.x, this->size_of_one_frame_image.y));
	}
}

void cAnimatedObjectLevel::setAnimationStartPosition(sf::Vector2f pos, short time_between_frames)
{
	if (this->start_position != pos)
	{
		this->start_position = pos;
		this->frames = (this->getTexture()->getSize().x - this->start_position.x) / size_of_one_frame_image.x;
		this->setFrame(0);
		this->time_between_frames = time_between_frames;
		this->time_to_next_frame = time_between_frames - 1;
	}
}

void cAnimatedObjectLevel::setTimeBetweenFrames(short time)
{
	this->time_between_frames = time;
	this->time_to_next_frame = time;
}

sf::Vector2f cAnimatedObjectLevel::getAnimationStartPosition()
{
	return this->start_position;
}

short cAnimatedObjectLevel::getCurrentFrame()
{
	return this->current_frame;
}

bool cAnimatedObjectLevel::isAnimationBeginsAgain()
{
	if (this->current_frame == 0 && this->time_to_next_frame == this->time_between_frames)
		return true;
	return false;
}

bool cAnimatedObjectLevel::isEndOfLastFrame()
{
	if (this->current_frame == this->frames - 1 && this->time_to_next_frame == 1)
		return true;
	return false;
}