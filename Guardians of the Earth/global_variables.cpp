#include "global_variables.h"
#include "character.h"

unsigned int g_width = 800, g_height = 600;
bool g_fullscreen = false;
int g_framerate_limit = 60;
float g_g = 0.5;
float g_score_multipler = 1.0f;
sFluidSpeedMultipler g_fluid_speed_multipler;

sControlKeys g_key[4];

void initControlKeys()
{
	//Player 1
	g_key[0].is_pad = false;
	g_key[0].pad = -1;

	g_key[0].up.key = sf::Keyboard::Key::Up;
	g_key[0].down.key = sf::Keyboard::Key::Down;
	g_key[0].left.key = sf::Keyboard::Key::Left;
	g_key[0].right.key = sf::Keyboard::Key::Right;
	g_key[0].jump.key = sf::Keyboard::Key::Space;
	g_key[0].fire.key = sf::Keyboard::Key::LControl;
	g_key[0].special1.key = sf::Keyboard::Key::RShift;
	
	//Player 2
	g_key[1].is_pad = false;
	g_key[1].pad = -1;

	g_key[1].up.key = sf::Keyboard::Key::W;
	g_key[1].down.key = sf::Keyboard::Key::S;
	g_key[1].left.key = sf::Keyboard::Key::A;
	g_key[1].right.key = sf::Keyboard::Key::D;
	g_key[1].jump.key = sf::Keyboard::Key::E;
	g_key[1].fire.key = sf::Keyboard::Key::Q;
	g_key[1].special1.key = sf::Keyboard::Key::F;
	
	//Player 3
	if (sf::Joystick::isConnected(0))
	{
		g_key[2].is_pad = true;
		g_key[2].pad = 0;
		
		g_key[2].jump.button = 2;
		g_key[2].fire.button = 3;
		g_key[2].special1.button = 5;
	}
	else
	{
		g_key[2].is_pad = false;
		g_key[2].pad = -1;

		g_key[2].up.key = sf::Keyboard::Key::U;
		g_key[2].down.key = sf::Keyboard::Key::J;
		g_key[2].left.key = sf::Keyboard::Key::H;
		g_key[2].right.key = sf::Keyboard::Key::K;
		g_key[2].jump.key = sf::Keyboard::Key::I;
		g_key[2].fire.key = sf::Keyboard::Key::Y;
		g_key[2].special1.key = sf::Keyboard::Key::L;
	}
	
	//Player 4
	if (sf::Joystick::isConnected(1))
	{
		g_key[3].is_pad = true;
		g_key[3].pad = 1;

		g_key[3].jump.button = 2;
		g_key[3].fire.button = 3;
		g_key[3].special1.button = 5;
	}
	else
	{
		g_key[3].is_pad = false;
		g_key[3].pad = -1;

		g_key[3].up.key = sf::Keyboard::Key::Numpad8;
		g_key[3].down.key = sf::Keyboard::Key::Numpad2;
		g_key[3].left.key = sf::Keyboard::Key::Numpad4;
		g_key[3].right.key = sf::Keyboard::Key::Numpad6;
		g_key[3].jump.key = sf::Keyboard::Key::Numpad9;
		g_key[3].fire.key = sf::Keyboard::Key::Numpad7;
		g_key[3].special1.key = sf::Keyboard::Key::Numpad1;
	}
}