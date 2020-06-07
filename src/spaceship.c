#include <allegro5/allegro_image.h>
#include <math.h>
#include "main.h"
#include "spaceship.h"
#include "blast.h"
#include "debug.h"

static inline void _AccelerateSpaceship(Spaceship*);
static inline void _DecelerateSpaceship(Spaceship*);
static inline void _DriftSpaceship(Spaceship*);
static inline void _RotateSpaceship(Spaceship*, SpaceshipDirection);
static void _UpdateSpaceshipPos(Spaceship*);
static void _UpdateSpaceshipInvincibility(Spaceship*);
static void _ResetKeys(Keyboard*);
static void _AllegroLoadSpaceshipImageForControl(void);
static void _AllegroDestroySpaceshipImageForControl(void);

static Keyboard keys;
const int kSpaceshipPixelsBeyondScreen = 10;
const float kSpaceshipSpeedMax = 3;
const float kSpaceshipSpeedInc = 0.050;
const float kSpaceshipSpeedDec = 0.025;
const float kSpaceshipDriftingSpeedDec = 0.01;
const float kSpaceshipHeadingInc = 0.05;
const float kSpaceshipGhostDur = 250;
static ALLEGRO_BITMAP *spaceship_image = NULL;
static float image_width = 0;
static float image_height = 0;

Spaceship* CreateSpaceship(void) {
	if (!spaceship_image)
		_AllegroLoadSpaceshipImageForControl();
	Spaceship *ship = malloc(sizeof(Spaceship));
	if (!ship)
		_ErrorMsg("Out of memory");
	ship->x_pos = kScreenWidth / 2;
	ship->y_pos = kScreenHeight / 2;
	ship->heading = 0;
	ship->speed = 0.0;
	ship->lives = 3;
	ship->scale = 0.25;
	ship->b_is_ghost = false;
	ship->bc = malloc(sizeof(BoundingCircle));
	ship->bc->x = &ship->x_pos;
	ship->bc->y = &ship->y_pos;
	ship->bc->r = image_width / 2 * ship->scale;
	ship->color = al_map_rgb(0, 255, 0);
	return ship;
}
void DestroySpaceship(Spaceship *ship) {
	_AllegroDestroySpaceshipImageForControl();
	free(ship->bc);
	free(ship);
}

bool InterpretKeyboardEvent(ALLEGRO_EVENT *event) {
	switch (event->keyboard.keycode) {
	case ALLEGRO_KEY_UP:
		keys.up = (event->type == ALLEGRO_EVENT_KEY_DOWN);
		return true;
	case ALLEGRO_KEY_DOWN:
		keys.down = (event->type == ALLEGRO_EVENT_KEY_DOWN);
		return true;
	case ALLEGRO_KEY_LEFT:
		keys.left = (event->type == ALLEGRO_EVENT_KEY_DOWN);
		return true;
	case ALLEGRO_KEY_RIGHT:
		keys.right = (event->type == ALLEGRO_EVENT_KEY_DOWN);
		return true;
	case ALLEGRO_KEY_SPACE:
		keys.space = (event->type == ALLEGRO_EVENT_KEY_DOWN);
		return true;
	case ALLEGRO_KEY_ESCAPE:
		return false;
	default:
		return true;
	}
}
void SpaceshipControl(Spaceship *ship) {
	if (keys.up)
		_AccelerateSpaceship(ship);
	if (keys.down)
		_DecelerateSpaceship(ship);
	if (keys.left)
		_RotateSpaceship(ship, kLeft);
	if (keys.right)
		_RotateSpaceship(ship, kRight);
	if (keys.space)
		FireBlast(ship);
	else
		_DriftSpaceship(ship);
	_UpdateSpaceshipPos(ship);
	_UpdateSpaceshipInvincibility(ship);
	//_ResetKeys(&keys);
}

void DrawSpaceship(Spaceship *ship) {
	/*
	 ALLEGRO_TRANSFORM transform;
	 al_build_transform(&transform, ship->x_pos, ship->y_pos, ship->scale,
	 ship->scale, ship->heading);
	 al_use_transform(&transform);
	 al_draw_line(-8, 9, 0, -11, ship->color, 3.0f);
	 al_draw_line(0, -11, 8, 9, ship->color, 3.0f);
	 al_draw_line(-6, 4, -1, 4, ship->color, 3.0f);
	 al_draw_line(6, 4, 1, 4, ship->color, 3.0f);
	 */

	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_use_transform(&transform);

	if (ship->b_is_ghost == false) {
		al_draw_scaled_rotated_bitmap(spaceship_image, image_width / 2,
				image_height / 2, ship->x_pos, ship->y_pos, ship->scale,
				ship->scale, ship->heading, 0);
	} else {
		al_draw_tinted_scaled_rotated_bitmap(spaceship_image, ship->color,
				image_width / 2, image_height / 2, ship->x_pos, ship->y_pos,
				ship->scale, ship->scale, ship->heading, 0);
	}
}

static inline void _AccelerateSpaceship(Spaceship *ship) {
	ship->speed += kSpaceshipSpeedInc;
	if (ship->speed > kSpaceshipSpeedMax)
		ship->speed = kSpaceshipSpeedMax;
}
static inline void _DecelerateSpaceship(Spaceship *ship) {
	ship->speed -= kSpaceshipSpeedDec;
	if (ship->speed < 0)
		ship->speed = 0;
}
static inline void _DriftSpaceship(Spaceship *ship) {
	ship->speed -= kSpaceshipDriftingSpeedDec;
	if (ship->speed < 0)
		ship->speed = 0;
}
static inline void _RotateSpaceship(Spaceship *ship,
		SpaceshipDirection direction) {
	if (direction == kLeft)
		ship->heading -= kSpaceshipHeadingInc;
	else if (direction == kRight)
		ship->heading += kSpaceshipHeadingInc;
}
static void _UpdateSpaceshipPos(Spaceship *ship) {
	ship->x_pos += sin(ship->heading) * ship->speed;
	ship->y_pos -= cos(ship->heading) * ship->speed;

	if (ship->x_pos > kScreenWidth + kSpaceshipPixelsBeyondScreen)
		ship->x_pos = -kSpaceshipPixelsBeyondScreen;
	if (ship->x_pos < -kSpaceshipPixelsBeyondScreen)
		ship->x_pos = kScreenWidth + kSpaceshipPixelsBeyondScreen;
	if (ship->y_pos > kScreenHeight + kSpaceshipPixelsBeyondScreen)
		ship->y_pos = -kSpaceshipPixelsBeyondScreen;
	if (ship->y_pos < -kSpaceshipPixelsBeyondScreen)
		ship->y_pos = kScreenHeight + kSpaceshipPixelsBeyondScreen;
}
static void _UpdateSpaceshipInvincibility(Spaceship *ship) {
	static int ghost_time = 0;
	ship->color = al_map_rgb(0, 255, 0);
	if (ship->b_is_ghost) {
		if (ghost_time >= kSpaceshipGhostDur) {
			ghost_time = 0;
			ship->b_is_ghost = false;
		} else {
			ghost_time++;
			if (ghost_time > 0 && ghost_time % 5 == 0)
				ship->color = al_map_rgb(255, 0, 0);
		}
	}
}

/*
static void _ResetKeys(Keyboard *keys) {
	keys->space = false;
}
*/

static void _AllegroLoadSpaceshipImageForControl(void) {
	if (!al_init_image_addon())
		_ErrorMsg("Failed to initialize allegro image addon");
	spaceship_image = al_load_bitmap("spaceship.jpg");
	if (!spaceship_image)
		_ErrorMsg("Can't load the image");
	image_width = al_get_bitmap_width(spaceship_image);
	image_height = al_get_bitmap_height(spaceship_image);
}

static void _AllegroDestroySpaceshipImageForControl(void) {
	if (spaceship_image)
		al_destroy_bitmap(spaceship_image);
}

