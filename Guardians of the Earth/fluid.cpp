#include "fluid.h"

unsigned int cFluid::fluid_id = 0;
b2BodyDef cFluid::physics_body_def;
b2Body *cFluid::physics_body = NULL;

void fluidInit(b2World *physics_world)
{
	cFluid fluid;
	fluid.resetMainParameters(physics_world);
};

cFluid::cFluid()
{
	;
}

cFluid::cFluid(eWorld world_type, sf::Vector2f pos)
{
	int texture_id;

	switch (world_type)
	{
	case WORLD_ICE_LAND:
	{
		texture_id = 4;
		break;
	}
	case WORLD_DESERT:
	{
		texture_id = 6;
		break;
	}
	default:
	{
		texture_id = 1;
		break;
	}
	}

	this->adjustGraphicsParameters(t_object[texture_id], pos);

	//BOX2D
	b2ChainShape shape;
	const float32 x = pos.x * 0.02f;
	const float32 y = pos.y * 0.02f;
	
	b2Vec2 vs[4];
	vs[0].Set(x + 0.0f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[1].Set(x + 32 * 0.02f - 16 * 0.02f, y + 0.0f - 16 * 0.02f);
	vs[2].Set(x + 32 * 0.02f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);
	vs[3].Set(x + 0.0f - 16 * 0.02f, y + 32 * 0.02f - 16 * 0.02f);

	shape.CreateLoop(vs, 4);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = CATEGORY(CAT_FLUID);
	fd.friction = 0.0f;
	this->physics_body->CreateFixture(&fd);

	this->fluid_id++;
}

void cFluid::graphicsCustomize(eWorld world_type, sf::Vector2u map_size, bool *another_object_exists, sf::Vector2i grid_size)
{
	//Dobieranie ID tekstury ze wzglêdu na to czym dany p³yn jest
	int texture_id[2];
	switch (world_type)
	{
	case WORLD_ICE_LAND:
	{
		texture_id[0] = 4;
		texture_id[1] = 5;
		break;
	}
	case WORLD_DESERT:
	{
		texture_id[0] = 6;
		texture_id[1] = 7;
		break;
	}
	default:
	{
		texture_id[0] = 1;
		texture_id[1] = 2;
		break;
	}
	}
	//!Dobieranie ID tekstury ze wzglêdu na to czym dany p³yn jest


	//Sprawdzanie, czy powy¿ej znajduje siê inny p³yn
	//Pozycja wody
	int x = getPosition().x / 32;
	int y = getPosition().y / 32 - 1;

	//Czy ponad p³ynem istnieje inny p³yn?
	bool exists = false;
	if (y == 0 || another_object_exists[(y - 1) * grid_size.x + x])	//GÓRA
		exists = true;
	//!Sprawdzanie, czy powy¿ej znajduje siê inny p³yn


	//Dostosowywanie grafiki p³ynu do pozosta³ych p³ynów
	if (exists)
		this->setTexture(t_object[texture_id[0]]);	//P³yn pod innymi p³ynami
	else
		this->setTexture(t_object[texture_id[1]]);	//P³yn na samej górze
	//!Dostosowywanie grafiki p³ynu do pozosta³ych p³ynów
}

void cFluid::resetMainParameters(b2World *physics_world)
{
	this->fluid_id = 0;

	this->physics_body = physics_world->CreateBody(&(this->physics_body_def));
}