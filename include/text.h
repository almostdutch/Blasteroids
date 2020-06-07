#ifndef _TEXT_H_
#define _TEXT_H_
#ifdef __cplusplus
extern "C" {
#endif

int GetScore(void);
void _ChangeScore(void);
void _InitAllegroFont(void);
void _DestroyAllegroFont(void);
void DisplayScore(int);
void DisplayLives(int);
void DisplayGameOver(void);

#ifdef __cplusplus
}
#endif
#endif
