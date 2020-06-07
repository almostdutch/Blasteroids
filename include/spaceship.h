#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <allegro5/allegro_primitives.h>
#include "main.h"

typedef struct {
	float x_pos;
	float y_pos;
	float heading;
	float speed;
	int lives;
	float scale;
	bool b_is_ghost;
	BoundingCircle *bc;
	ALLEGRO_COLOR color;
} Spaceship;

typedef struct {
	bool left;
	bool right;
	bool up;
	bool down;
	bool space;
} Keyboard;

typedef enum {
	kLeft, kRight
} SpaceshipDirection;

Spaceship* CreateSpaceship(void);
Spaceship* GetSpaceship(void);
void DestroySpaceship(Spaceship*);
bool InterpretKeyboardEvent(ALLEGRO_EVENT*);
void DrawSpaceship(Spaceship*);
void SpaceshipControl(Spaceship*);

extern const int kSpaceshipPixelsBeyondScreen;
extern const float kSpaceshipSpeedMax;
extern const float kSpaceshipSpeedInc;
extern const float kSpaceshipSpeedDec;
extern const float kSpaceshipDriftingSpeedDec;
extern const float kSpaceshipHeadingInc;
extern const float kSpaceshipGhostDur;

#ifdef __cplusplus
}
#endif
#endif
