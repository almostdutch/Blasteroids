#include <allegro5/allegro_image.h>
#include <math.h>
#include "spaceship.h"
#include "asteroid.h"
#include "debug.h"

static void _CreateAsteroidList(void);
static AsteroidNode* _GetAsteroidListLastNode(AsteroidNode*);
static AsteroidNode* _CreateNewNode(Asteroid*);
static Asteroid* _CreateAsteroid(Asteroid*);
static void _AddNewNode(AsteroidNode*);
static void _RemoveNode(AsteroidNode*);
static void _InitAsteroidPos(Asteroid*, Asteroid*);
static void _UpdateAsteroidPos(AsteroidNode*);
static void _DrawAsteroid(Asteroid *asteroid);
static void _DestroyAsteroid(AsteroidNode*);
static inline float FloatNumberGenerator(float, float);
static inline float DegreeToRad(float);
static void _AllegroLoadAsteroidImage(void);
static void _AllegroDestroyAsteroidImage(void);

const int kAsteroidPixelsBeyondScreen = 20;
const int kMaxAsteroidsOnScreen = 10;
const float kMinAsteroidScale = 0.2;
const float kMaxAsteroidScale = 0.5;
const float kMaxAsteroidSpeed = 1.0;
const float kMinAsteroidSpeed = 0.5;
const float kMaxAsteroidRotationSpeed = 0.05;
static int kAsteroidCount = 0;
static const float kPi = 3.14;
static AsteroidNode *p_asteroid_list = NULL;
static ALLEGRO_BITMAP *asteroid_image = NULL;
static float image_width = 0;
static float image_height = 0;

static void _CreateAsteroidList(void) {
	if (!asteroid_image)
		_AllegroLoadAsteroidImage();
	if (kAsteroidCount <= kMaxAsteroidsOnScreen) {
		AsteroidNode *new_node = _CreateNewNode(NULL);
		_AddNewNode(new_node);
	}
}
AsteroidNode* GetAsteroidList(void) {
	return p_asteroid_list;
}
static AsteroidNode* _GetAsteroidListLastNode(AsteroidNode *asteroid_list) {
	AsteroidNode *current = asteroid_list;
	AsteroidNode *last = NULL;
	while (current != NULL) {
		last = current;
		current = current->next_node;
	}
	return last;
}
void DestroyAsteroidList(void) {
	_AllegroDestroyAsteroidImage();
	if (p_asteroid_list != NULL) {
		AsteroidNode *current = p_asteroid_list;
		AsteroidNode *next = NULL;
		while (current != NULL) {
			next = current->next_node;
			_DestroyAsteroid(current);
			current = next;
		}
	}
}

static Asteroid* _CreateAsteroid(Asteroid *parent) {
	Asteroid *asteroid = malloc(sizeof(Asteroid));
	if (asteroid == NULL)
		_ErrorMsg("Out of memory");
	asteroid->speed = FloatNumberGenerator(kMinAsteroidSpeed,
			kMaxAsteroidSpeed);
	asteroid->scale = FloatNumberGenerator(kMinAsteroidScale,
			kMaxAsteroidScale);
	asteroid->rot_theta = 0;
	asteroid->is_gone = false;
	asteroid->bc = malloc(sizeof(BoundingCircle));
	asteroid->bc->x = &asteroid->x_pos;
	asteroid->bc->y = &asteroid->y_pos;
	asteroid->bc->r = image_height / 2 * asteroid->scale;
	asteroid->color = al_map_rgb(255, 255, 255);
	_InitAsteroidPos(asteroid, parent);
	kAsteroidCount++;
	return asteroid;
}
static void _DestroyAsteroid(AsteroidNode *destroy_this_asteroid) {
	if (destroy_this_asteroid != NULL) {
		_RemoveNode(destroy_this_asteroid);
		free(destroy_this_asteroid->asteroid->bc);
		free(destroy_this_asteroid->asteroid);
		free(destroy_this_asteroid);
		kAsteroidCount--;
	}
}

static AsteroidNode* _CreateNewNode(Asteroid *parent) {
	AsteroidNode *new_node = malloc(sizeof(AsteroidNode));
	if (new_node == NULL)
		_ErrorMsg("Out of memory");
	Asteroid *asteroid = _CreateAsteroid(parent);
	new_node->asteroid = asteroid;
	new_node->next_node = NULL;
	return new_node;
}
static void _AddNewNode(AsteroidNode *new_node) {
	if (p_asteroid_list == NULL)
		p_asteroid_list = new_node;
	else {
		AsteroidNode *last = _GetAsteroidListLastNode(p_asteroid_list);
		last->next_node = new_node;
	}
}
static void _RemoveNode(AsteroidNode *remove_this_node) {
	if (remove_this_node != p_asteroid_list) {
		AsteroidNode *current = p_asteroid_list, *previous = NULL;
		while (current != remove_this_node) {
			previous = current;
			current = current->next_node;
		}
		previous->next_node = remove_this_node->next_node;
	} else {
		p_asteroid_list = p_asteroid_list->next_node;
	}
}

static void _DrawAsteroid(Asteroid *asteroid) {
	/*
	 ALLEGRO_TRANSFORM transform;
	 al_identity_transform(&transform);
	 al_build_transform(&transform, asteroid->x_pos, asteroid->y_pos,
	 asteroid->scale, asteroid->scale, asteroid->rot_theta);
	 al_use_transform(&transform);
	 al_draw_line(-20, 20, -25, 5, asteroid->color, 2.0f);
	 al_draw_line(-25, 5, -25, -10, asteroid->color, 2.0f);
	 al_draw_line(-25, -10, -5, -10, asteroid->color, 2.0f);
	 al_draw_line(-5, -10, -10, -20, asteroid->color, 2.0f);
	 al_draw_line(-10, -20, 5, -20, asteroid->color, 2.0f);
	 al_draw_line(5, -20, 20, -10, asteroid->color, 2.0f);
	 al_draw_line(20, -10, 20, -5, asteroid->color, 2.0f);
	 al_draw_line(20, -5, 0, 0, asteroid->color, 2.0f);
	 al_draw_line(0, 0, 20, 10, asteroid->color, 2.0f);
	 al_draw_line(20, 10, 10, 20, asteroid->color, 2.0f);
	 al_draw_line(10, 20, 0, 15, asteroid->color, 2.0f);
	 al_draw_line(0, 15, -20, 20, asteroid->color, 2.0f);
	 */

	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_use_transform(&transform);
	al_draw_scaled_rotated_bitmap(asteroid_image, image_width / 2,
			image_height / 2, asteroid->x_pos, asteroid->y_pos, asteroid->scale,
			asteroid->scale, asteroid->rot_theta, 0);
}
void DrawAsteroidsList(void) {
	AsteroidNode *current = p_asteroid_list;
	while (current != NULL) {
		_DrawAsteroid(current->asteroid);
		current = current->next_node;
	}
}

void AsteroidControl(void) {
	_CreateAsteroidList();
	AsteroidNode *current = p_asteroid_list, *next = NULL;
	while (current != NULL) {
		next = current->next_node;
		_UpdateAsteroidPos(current);
		current = next;
	}
}

static void _InitAsteroidPos(Asteroid *asteroid, Asteroid *parent) {
	if (parent == NULL) {
		const float kAsteroidPixelsBeyondScreenCor = kAsteroidPixelsBeyondScreen
				* asteroid->scale;

		switch (rand() % 4) {
		case 0:
			asteroid->x_pos = 0 - kAsteroidPixelsBeyondScreenCor;
			asteroid->y_pos = FloatNumberGenerator(0, kScreenHeight);
			asteroid->heading = FloatNumberGenerator(DegreeToRad(45),
					DegreeToRad(135));
			break;
		case 1:
			asteroid->x_pos = kScreenWidth + kAsteroidPixelsBeyondScreenCor;
			asteroid->y_pos = FloatNumberGenerator(0, kScreenHeight);
			asteroid->heading = FloatNumberGenerator(DegreeToRad(225),
					DegreeToRad(315));
			break;
		case 2:
			asteroid->x_pos = FloatNumberGenerator(0, kScreenWidth);
			asteroid->y_pos = 0 - kAsteroidPixelsBeyondScreenCor;
			asteroid->heading = FloatNumberGenerator(DegreeToRad(135),
					DegreeToRad(225));
			break;
		case 3:
			asteroid->x_pos = FloatNumberGenerator(0, kScreenWidth);
			asteroid->y_pos = kScreenHeight + kAsteroidPixelsBeyondScreenCor;
			asteroid->heading = FloatNumberGenerator(DegreeToRad(-45),
					DegreeToRad(45));
			break;
		}
	} else {
		asteroid->x_pos = parent->x_pos;
		asteroid->y_pos = parent->y_pos;
		asteroid->heading = FloatNumberGenerator(
				(parent->heading - DegreeToRad(90)),
				(parent->heading + DegreeToRad(90)));
		asteroid->speed = parent->speed * 2;
		asteroid->scale = parent->scale / 2;
	}

	asteroid->rot_speed = FloatNumberGenerator(-2.0, 2.0)
			* FloatNumberGenerator(0, kMaxAsteroidRotationSpeed);
}
static void _UpdateAsteroidPos(AsteroidNode *node) {
	if (node->asteroid->is_gone) {
		_DestroyAsteroid(node);
		return;
	}

	node->asteroid->rot_theta += node->asteroid->rot_speed;
	node->asteroid->x_pos += sin(node->asteroid->heading)
			* node->asteroid->speed;
	node->asteroid->y_pos -= cos(node->asteroid->heading)
			* node->asteroid->speed;

	const float kAsteroidPixelsBeyondScreenCor = kAsteroidPixelsBeyondScreen
			* node->asteroid->scale;
	if (node->asteroid->x_pos > kScreenWidth + kAsteroidPixelsBeyondScreenCor)
		node->asteroid->x_pos = 0 - kAsteroidPixelsBeyondScreenCor;
	if (node->asteroid->x_pos < 0 - kAsteroidPixelsBeyondScreenCor)
		node->asteroid->x_pos = kScreenWidth + kAsteroidPixelsBeyondScreenCor;
	if (node->asteroid->y_pos > kScreenHeight + kAsteroidPixelsBeyondScreenCor)
		node->asteroid->y_pos = 0 - kAsteroidPixelsBeyondScreenCor;
	if (node->asteroid->y_pos < 0 - kAsteroidPixelsBeyondScreenCor)
		node->asteroid->y_pos = kScreenHeight + kAsteroidPixelsBeyondScreenCor;
}

void SplitAsteroid(AsteroidNode *node) {
	if (node != NULL) {
		if (node->asteroid->scale / 2 >= kMinAsteroidScale) {
			AsteroidNode *newnode1 = _CreateNewNode(node->asteroid);
			_AddNewNode(newnode1);
			AsteroidNode *newnode2 = _CreateNewNode(node->asteroid);
			_AddNewNode(newnode2);
		}
		node->asteroid->is_gone = true;
	}
}

static inline float FloatNumberGenerator(float min, float max) {
	float scale = rand() / (float) RAND_MAX;
	return min + scale * (max - min);
}
static inline float DegreeToRad(float theta) {
	return theta / (180 / kPi);
}

static void _AllegroLoadAsteroidImage(void) {
	if (!al_init_image_addon())
		_ErrorMsg("Failed to initialize allegro image addon");
	asteroid_image = al_load_bitmap("asteroid.jpg");
	if (!asteroid_image)
		_ErrorMsg("Can't load the image");
	image_width = al_get_bitmap_width(asteroid_image);
	image_height = al_get_bitmap_height(asteroid_image);
}

static void _AllegroDestroyAsteroidImage(void) {
	if (asteroid_image)
		al_destroy_bitmap(asteroid_image);
}

