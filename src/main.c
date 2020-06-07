#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "main.h"
#include "spaceship.h"
#include "asteroid.h"
#include "collision.h"
#include "text.h"
#include "debug.h"

const int kFPS = 60;
const int kScreenWidth = 640;
const int kScreenHeight = 480;

static void InitGame(void);
static void GameLoop(void);
static void Cleanup(void);
static void GameGraphics(void);
static void GameEngine(void);
static void KeepTrackOfAllObjects(void);

static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT event;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;
static ALLEGRO_TIMER *timer = NULL;
static Spaceship *ship = NULL;
static GameObjects *all_game_objects = NULL;

static void InitGame(void) {
	srand(time(NULL));
	if (!al_init())
		_ErrorMsg("Failed to initialize allegro");
	_InitAllegroFont();
	if (!al_init_primitives_addon())
		_ErrorMsg("Failed to initialize primitives addon");
	display = al_create_display(kScreenWidth, kScreenHeight);
	if (!display)
		_ErrorMsg("Failed to create display");
	if (!al_install_keyboard())
		_ErrorMsg("Failed to install keyboard");
	timer = al_create_timer(1.0 / kFPS);
	if (!timer)
		_ErrorMsg("Failed to create timer");
	event_queue = al_create_event_queue();
	if (!event_queue)
		_ErrorMsg("Failed to create event queue");

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);
	ship = CreateSpaceship();
	all_game_objects = malloc(sizeof(GameObjects));
}

static void Cleanup(void) {
	_DestroyAllegroFont();
	if (timer)
		al_destroy_timer(timer);
	if (display)
		al_destroy_display(display);
	if (event_queue)
		al_destroy_event_queue(event_queue);
	if (all_game_objects)
		free(all_game_objects);

	DestroySpaceship(ship);
	DestroyBlastArray();
	DestroyAsteroidList();
}

static void GameGraphics(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	DrawSpaceship(ship);
	DrawBlastArray();
	DrawAsteroidsList();
	DisplayScore(GetScore());
	DisplayLives(ship->lives);

	if (ship->lives == 0)
		DisplayGameOver();

	al_flip_display();
}

static void GameEngine(void) {
	SpaceshipControl(ship);
	BlastControl();
	AsteroidControl();
	KeepTrackOfAllObjects();
	DetectCollision(all_game_objects);
}

static void KeepTrackOfAllObjects(void) {
	all_game_objects->ship = ship;
	all_game_objects->asteroid_list = GetAsteroidList();
	all_game_objects->blast_array = GetBlastArray();
	all_game_objects->blast_count = GetBlastCount();
}

static void GameLoop(void) {
	bool b_continue_game = true;
	while (b_continue_game) {
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_KEY_DOWN
				|| event.type == ALLEGRO_EVENT_KEY_UP)
			b_continue_game = InterpretKeyboardEvent(&event);

		if (ship->lives == 0) {
			GameGraphics();
			continue;
		}

		if (event.type == ALLEGRO_EVENT_TIMER) {
			GameEngine();
			GameGraphics();
		}

	}
}

int main() {

	InitGame();
	GameLoop();
	Cleanup();

	return 0;
}
