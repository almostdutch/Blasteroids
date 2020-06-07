#ifndef _COLLISION_H_
#define _COLLISION_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"

typedef struct {
	Spaceship *ship;
	AsteroidNode *asteroid_list;
	Blast **blast_array;
	int blast_count;
} GameObjects;

void DetectCollision(GameObjects *all_game_objects);

#ifdef __cplusplus
}
#endif
#endif
