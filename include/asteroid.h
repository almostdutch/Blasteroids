#ifndef _ASTEROID_H_
#define _ASTEROID_H_
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
	float rot_theta;
	float rot_speed;
	float scale;
	bool is_gone;
	BoundingCircle *bc;
	ALLEGRO_COLOR color;
} Asteroid;

typedef struct AsteroidNode {
	Asteroid *asteroid;
	struct AsteroidNode *next_node;
} AsteroidNode;

AsteroidNode* GetAsteroidList(void);
void DestroyAsteroidList(void);
void SplitAsteroid(AsteroidNode *);
void AsteroidControl(void);
void DrawAsteroidsList(void);

extern const int kAsteroidPixelsBeyondScreen;
extern const int kMaxAsteroidsOnScreen;
extern const float kMinAsteroidScale;
extern const float kMaxAsteroidScale;
extern const float kMinAsteroidSpeed;
extern const float kMaxAsteroidSpeed;
extern const float kMaxAsteroidRotationSpeed;

#ifdef __cplusplus
}
#endif
#endif
