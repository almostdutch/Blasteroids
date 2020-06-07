#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "main.h"
#include "text.h"
#include "debug.h"

static void _AllegroLoadSpaceshipImageLives(void);
static void _AllegroDestroySpaceshipImageLives(void);

static int game_points = 0;
static const int pRewardPoints = 100;
static ALLEGRO_FONT *allegro_font = NULL;
static ALLEGRO_BITMAP *spaceship_image_lives = NULL;
static float image_width = 0;
static float image_height = 0;

void _InitAllegroFont(void) {
	if (!spaceship_image_lives)
		_AllegroLoadSpaceshipImageLives();
	if (!al_init_font_addon())
		_ErrorMsg("Failed to initialize allegro font addon");
	allegro_font = al_create_builtin_font();
	if (!allegro_font)
		_ErrorMsg("Failed to create builtin font");
}
void _DestroyAllegroFont(void) {
	_AllegroDestroySpaceshipImageLives();
	if (allegro_font)
		al_destroy_font(allegro_font);
}

int GetScore(void) {
	return game_points;
}
void _ChangeScore(void) {
	game_points += pRewardPoints;
}

void DisplayScore(int score) {
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_build_transform(&transform, 10, 30, 5, 5, 0);
	al_use_transform(&transform);
	al_draw_textf(allegro_font, al_map_rgb(200, 144, 152), 0, 0,
			ALLEGRO_ALIGN_LEFT, "%d", score);
}

void DisplayLives(int lives) {
	for (int i = 0; i < lives; i++) {
		/*
		 ALLEGRO_TRANSFORM transform;
		 al_identity_transform(&transform);
		 al_translate_transform(&transform, 30 + 40 * i, 90);
		 al_use_transform(&transform);
		 al_draw_line(-8, 9, 0, -11, al_map_rgb(0, 255, 0), 3.0f);
		 al_draw_line(0, -11, 8, 9, al_map_rgb(0, 255, 0), 3.0f);
		 al_draw_line(-6, 4, -1, 4, al_map_rgb(0, 255, 0), 3.0f);
		 al_draw_line(6, 4, 1, 4, al_map_rgb(0, 255, 0), 3.0f);
		 */

		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_use_transform(&transform);
		al_draw_scaled_rotated_bitmap(spaceship_image_lives, image_width / 2,
				image_height / 2, 20 + 25 * i, 90, 0.25, 0.25f, 0, 1);
	}
}

void DisplayGameOver(void) {
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_scale_transform(&transform, 5.0f, 5.0f);
	al_translate_transform(&transform, kScreenWidth / 2, kScreenHeight / 2);
	al_use_transform(&transform);
	al_draw_text(allegro_font, al_map_rgb(255, 0, 0), 0, 0,
			ALLEGRO_ALIGN_CENTER, "Game Over!");
}

static void _AllegroLoadSpaceshipImageLives(void) {
	if (!al_init_image_addon())
		_ErrorMsg("Failed to initialize allegro image addon");
	spaceship_image_lives = al_load_bitmap("spaceship.jpg");
	if (!spaceship_image_lives)
		_ErrorMsg("Can't load the image");
	image_width = al_get_bitmap_width(spaceship_image_lives);
	image_height = al_get_bitmap_height(spaceship_image_lives);
}

static void _AllegroDestroySpaceshipImageLives(void) {
	if (spaceship_image_lives)
		al_destroy_bitmap(spaceship_image_lives);
}
