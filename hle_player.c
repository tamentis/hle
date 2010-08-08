#include "hive.h"

hle_entity *
hle_player_new()
{
	hle_entity *e = hle_entity_new();
	e->type = ETYPE_PLAYER;
	return e;
}
