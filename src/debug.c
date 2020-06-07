#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "debug.h"

void _ErrorMsg(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}
