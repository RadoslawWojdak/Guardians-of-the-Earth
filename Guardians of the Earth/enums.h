#ifndef enums_h
#define enums_h

#define CATEGORY(num) (1 << (num))	//Kategorie do jakich nale¿¹ fikstury

enum eCategory
{
	CAT_GROUND = 0,
	CAT_BLOCK = 1,
	CAT_BONUS_BLOCK = 2,
	CAT_TREASURE = 3,
	CAT_NPC = 4,
	CAT_TRAMPOLINE = 5,
	CAT_WATER = 6
};

enum eType
{
	TYPE_GROUND = 0,		//Na ziemi
	TYPE_AIR = 1,			//W powietrzu
	TYPE_WATER = 2,			//W wodzie
	TYPE_SOIL = 3,			//Wewn¹trz ziemi
	TYPE_GROUND_WATER = 4	//W wodzie na ziemi
};

enum eDirection
{
	DIR_UP = 0,
	DIR_DOWN = 1,
	DIR_LEFT = 2,
	DIR_RIGHT = 3
};

enum eWorld
{
	WORLD_OVERWORLD = 0,
	WORLD_UNDERGROUND = 1
};

enum eObjType	//typ objektu znajdujacego sie na sektorze - blok/ziemia/bonus/npc/...
{
	none = 0,
	ground = 1,
	block = 2,
	bonus_block = 3,
	water = 4,
	treasure = 5,
	trampoline = 6
};

enum eTreasure
{
	T_HEART = -1,
	T_COPPER_COIN = 1,
	T_SILVER_COIN = 2,
	T_GOLDEN_COIN = 3,
	T_GOLD_BAR = 5,
	T_RED_GEM = 10,
	T_SCEPTER = 20
};

#endif //!enums_h