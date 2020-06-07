#include <math.h>
#include "main.h"
#include "text.h"
#include "collision.h"

static bool _IsCollisionDetected(BoundingCircle*, BoundingCircle*);
static void _DoShipAsteroidCollision(Spaceship*, AsteroidNode*);
static void _DoBlastAsteroidCollision(Blast*, AsteroidNode*);
static void _DoAsteroidAsteroidCollision(AsteroidNode*, AsteroidNode*);

void DetectCollision(GameObjects *all_game_objects) {
	Spaceship *ship = all_game_objects->ship;
	AsteroidNode *asteroid_list = all_game_objects->asteroid_list;
	AsteroidNode *previous_node = NULL;
	Blast **blast_array = all_game_objects->blast_array;
	int blast_count = all_game_objects->blast_count;

	while (asteroid_list != NULL) {
		previous_node = asteroid_list;

		if (!ship->b_is_ghost)
			if (_IsCollisionDetected(ship->bc, asteroid_list->asteroid->bc)) {
				_DoShipAsteroidCollision(ship, asteroid_list);
			}

		for (int i = 0; i < blast_count; i++) {
			if (!blast_array[i]->is_gone)
				if (_IsCollisionDetected(blast_array[i]->bc,
						asteroid_list->asteroid->bc))
					_DoBlastAsteroidCollision(blast_array[i], asteroid_list);
		}
		asteroid_list = asteroid_list->next_node;

		/*
		if (previous_node && asteroid_list) {
			if (_IsCollisionDetected(previous_node->asteroid->bc,
					asteroid_list->asteroid->bc)) {
				_DoAsteroidAsteroidCollision(previous_node, asteroid_list);
			}
		}
		*/
	}
}

static bool _IsCollisionDetected(BoundingCircle *bc1, BoundingCircle *bc2) {
	float dx = *(bc1->x) - *(bc2->x);
	float dy = *(bc1->y) - *(bc2->y);
	float distance = sqrt(dx * dx + dy * dy);
	return distance < (bc1->r + bc2->r);
}
static void _DoShipAsteroidCollision(Spaceship *ship, AsteroidNode *asteroid) {
	ship->lives -= 1;
	ship->b_is_ghost = true;
	SplitAsteroid(asteroid);
}
static void _DoBlastAsteroidCollision(Blast *blast, AsteroidNode *asteroid) {
	blast->is_gone = true;
	_ChangeScore();
	SplitAsteroid(asteroid);
}
/*
static void _DoAsteroidAsteroidCollision(AsteroidNode *asteroid1,
		AsteroidNode *asteroid2) {
	SplitAsteroid(asteroid1);
	SplitAsteroid(asteroid2);
}
*/
