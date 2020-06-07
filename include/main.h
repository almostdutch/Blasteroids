#ifndef _MAIN_H_
#define _MAIN_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	float *x;
	float *y;
	float r;
} BoundingCircle;

extern const int kFPS;
extern const int kScreenWidth;
extern const int kScreenHeight;

#ifdef __cplusplus
}
#endif
#endif
