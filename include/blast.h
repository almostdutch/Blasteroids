#ifndef _BLAST_H_
#define _BLAST_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <allegro5/allegro_primitives.h>
#define kNofBlasts 3

typedef struct {
	float x_pos;
	float y_pos;
	float heading;
	float speed;
	float scale;
	bool is_gone;
	BoundingCircle *bc;
	ALLEGRO_COLOR color;
} Blast;

Blast** GetBlastArray(void);
int GetBlastCount(void);
void FireBlast(Spaceship*);
void DrawBlastArray(void);
void DestroyBlastArray(void);
void BlastControl(void);

#ifdef __cplusplus
}
#endif
#endif
