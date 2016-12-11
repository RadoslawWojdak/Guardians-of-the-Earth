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

enum eBackgroundType
{
	BG_NONE = 0,				//Brak typu
	BG_GROUND_LYING = 1,		//Typ naziemny, le¿¹cy (np. krzak)
	BG_GROUND_LEVITATING = 2,	//Typ naziemny, lewituj¹cy (tu¿ nad gruntem) (np. œwietlik)
	BG_TRUNK = 3,				//Pieñ drzewa
	BG_TOP_TREE = 4,			//Góra (korona) drzewa
	BG_FLYING = 5,				//Typ lataj¹cy (np. chmury)
	BG_GROUND_INSIDE = 6,		//Typ wewn¹trz gruntu (np. kamienie)
	BG_WATER_LYING = 7,			//Typ wodny, le¿¹cy (np. wodorosty)
	BG_WATER_DIVE = 8,			//Typ wodny, nurkuj¹cy (np. b¹belek powietrza)
	BG_WATER_FLOAT = 9,			//Typ unosz¹cy siê na wodzie (np. kaczka)
	BG_CEILING = 10				//Typ wisz¹cy na suficie (np. nietoperz)
};

/*enum eType
{
	TYPE_GROUND = 0,		//Na ziemi
	TYPE_AIR = 1,			//W powietrzu
	TYPE_WATER = 2,			//W wodzie
	TYPE_GROUND_INSIDE = 3,			//Wewn¹trz ziemi
	TYPE_GROUND_WATER = 4	//W wodzie na ziemi
};*/

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
	WORLD_UNDERGROUND = 1,
	WORLD_UNDERWATER = 2,
	WORLD_ICE_LAND = 3,
	WORLD_DESERT = 4
};

enum eObjType	//typ objektu znajdujacego sie na sektorze - blok/ziemia/bonus/npc/...
{
	OBJECT_NONE = 0,
	OBJECT_GROUND = 1,
	OBJECT_BLOCK = 2,
	OBJECT_BONUS_BLOCK = 3,
	OBJECT_WATER = 4,
	OBJECT_TREASURE = 5,
	OBJECT_TRAMPOLINE = 6,
	OBJECT_POWER_UP = 7,
	OBJECT_LADDER = 8
};

//Typy szczegó³owe (dziêki nim ³atwiej okreœliæ, gdzie i w jaki sposób program ma rysowaæ obiekty w tle)
/*enum eBackgroundType
{
	BG_NONE = 0,				//Brak typu
	BG_GROUND_LYING = 1,		//Typ naziemny, le¿¹cy (np. krzak)
	BG_GROUND_LEVITATING = 2,	//Typ naziemny, lewituj¹cy (tu¿ nad gruntem) (np. œwietlik)
	BG_TRUNK = 3,				//Pieñ drzewa
	BG_TOP_TREE = 4,			//Góra (korona) drzewa
	BG_FLYING = 5,				//Typ lataj¹cy (np. chmury)
	BG_GROUND_INSIDE = 6,		//Typ wewn¹trz gruntu (np. kamienie)
	BG_WATER_LYING = 7,			//Typ wodny, le¿¹cy (np. wodorosty)
	BG_WATER_DIVE = 8,			//Typ wodny, nurkuj¹cy (np. b¹belek powietrza)
	BG_WATER_FLOAT = 9			//Typ unosz¹cy siê na wodzie (np. kaczka)
};*/

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