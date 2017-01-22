#ifndef enums_h
#define enums_h

#define CATEGORY(num) (1 << (num))	//Kategorie do jakich nale¿¹ fikstury

enum eCategory
{
	CAT_EMPTY = -1,
	CAT_GROUND = 0,
	CAT_BLOCK = 1,
	CAT_BONUS_BLOCK = 2,
	CAT_TREASURE = 3,
	CAT_NPC = 4,
	CAT_TRAMPOLINE = 5,
	CAT_FLUID = 6,
	CAT_CHARACTER = 7,
	CAT_BULLET = 8
};

enum eCharacterAnimationType
{
	ANIMATION_STANDING = 0,
	ANIMATION_WALKING = 1,
	ANIMATION_JUMPING = 2,
	ANIMATION_CLIMBING = 3,
	ANIMATION_SWIMMING = 4,
	ANIMATION_SPECIAL1 = 5,
	ANIMATION_SPECIAL2 = 6
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
	BG_FLUID_LYING = 7,			//Typ p³ynowy, le¿¹cy (np. wodorosty)
	BG_FLUID_DIVE = 8,			//Typ p³ynowy, nurkuj¹cy (np. b¹belek powietrza)
	BG_FLUID_FLOAT = 9,			//Typ unosz¹cy siê na p³ynie (np. kaczka)
	BG_CEILING = 10				//Typ wisz¹cy na suficie (np. nietoperz)
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
	WORLD_UNDERGROUND = 1,
	WORLD_UNDERWATER = 2,
	WORLD_ICE_LAND = 3,
	WORLD_DESERT = 4
};

enum eCharacter
{
	CHARACTER_KNIGHT = 0,
	CHARACTER_ARCHER = 1,
	CHARACTER_SPY = 2,
	CHARACTER_SORCERESS = 3
};

enum eObjType	//typ objektu znajdujacego sie na sektorze - blok/ziemia/bonus/npc/...
{
	OBJECT_NONE = 0,
	OBJECT_GROUND = 1,
	OBJECT_BLOCK = 2,
	OBJECT_BONUS_BLOCK = 3,
	OBJECT_FLUID = 4,
	OBJECT_TREASURE = 5,
	OBJECT_TRAMPOLINE = 6,
	OBJECT_POWER_UP = 7,
	OBJECT_LADDER = 8
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

enum eFluidType
{
	FLUID_NONE = 0,
	FLUID_WATER = 1,
	FLUID_ICE = 2,
	FLUID_QUICKSAND = 3
};

enum eUnlockedType	//NIE ZMIENIAÆ WARTOŒCI (S¥ W KOLEJNOŒCI ZAPISYWANIA DO PLIKU)
{
	UNLOCKED_MODULATOR = 0,
	UNLOCKED_NPC = 1
};

enum eModulatorType
{
	MODULATOR_FACILIATING = 0,
	MODULATOR_NEUTRAL = 1,
	MODULATOR_OBSTRUCTING = 2
};

#endif //!enums_h