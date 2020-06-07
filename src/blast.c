#include <math.h>
#include "main.h"
#include "spaceship.h"
#include "blast.h"
#include "debug.h"

static void _CreateBlastArray(Spaceship*);
static inline void _DestroyBlast(Blast*);
static void _DrawBlast(Blast*);
static int _EraseBlast(void);
static void _UpdateBlastPos(Blast*);
static void _ShuffleBlastArray(int);

static int blast_count = 0;
static const float kBlastSpeed = 10;
static Blast *blast_array[kNofBlasts];

int GetBlastCount(void) {
	return blast_count;
}
Blast** GetBlastArray(void) {
	return blast_array;
}
void FireBlast(Spaceship *ship) {
	if (blast_count < kNofBlasts) {
		_CreateBlastArray(ship);
	}
}
static void _CreateBlastArray(Spaceship *ship) {
	Blast *blast = malloc(sizeof(Blast));
	if (blast == NULL)
		_ErrorMsg("Out of memory");
	blast->x_pos = ship->x_pos + sin(ship->heading) * 15;
	blast->y_pos = ship->y_pos - cos(ship->heading) * 15;
	blast->heading = ship->heading;
	blast->speed = kBlastSpeed;
	blast->scale = 1;
	blast->is_gone = false;
	blast->bc = malloc(sizeof(BoundingCircle));
	blast->bc->x = &blast->x_pos;
	blast->bc->y = &blast->y_pos;
	blast->bc->r = 10 * blast->scale;
	blast->color = al_map_rgb(0, 255, 0);
	blast_array[blast_count++] = blast;
}
void DestroyBlastArray(void) {
	for (int i = 0; i < blast_count; i++) {
		_DestroyBlast(blast_array[i]);
	}
}
static inline void _DestroyBlast(Blast *blast) {
	free(blast->bc);
	free(blast);
}
void DrawBlastArray(void) {
	for (int i = 0; i < blast_count; i++)
		_DrawBlast(blast_array[i]);
}
static void _DrawBlast(Blast *blast) {
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_build_transform(&transform, blast->x_pos, blast->y_pos, blast->scale,
			blast->scale, blast->heading);
	al_use_transform(&transform);
	al_draw_line(0, 0, 0, 10, blast->color, 5.0f);
}
static int _EraseBlast(void) {
	int current_blast_count = blast_count;
	for (int i = 0; i < blast_count; i++)
		if (blast_array[i]->is_gone) {
			_DestroyBlast(blast_array[i]);
			blast_array[i] = NULL;
			current_blast_count--;
		}
	return current_blast_count;
}

void BlastControl(void) {
	int temp_blast_count = 0;
	Blast *temp_blast_array[blast_count];
	if (blast_count) {
		for (int i = 0; i < blast_count; i++) {
			if (blast_array[i]) {
				_UpdateBlastPos(blast_array[i]);
				if (blast_array[i]->is_gone) {
					_DestroyBlast(blast_array[i]);
					blast_array[i] = NULL;
				} else
					temp_blast_array[temp_blast_count++] = blast_array[i];
			}
		}
		blast_count = temp_blast_count;
		if (blast_count) {
			for (int i = 0; i < blast_count; i++)
				blast_array[i] = temp_blast_array[i];
		}
	}
}

static void _UpdateBlastPos(Blast *blast) {
	blast->x_pos += sin(blast->heading) * blast->speed;
	blast->y_pos -= cos(blast->heading) * blast->speed;
	if ((blast->x_pos > kScreenWidth || blast->x_pos < 0)
			|| (blast->y_pos > kScreenHeight || blast->y_pos < 0)) {
		blast->is_gone = true;
	}
}
