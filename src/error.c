#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "error.h"

void log_error(const char *type, const char *function, const char *sdlerror, const int fatal)
{
	if(type != NULL && function != NULL && sdlerror != NULL) {
		if(fatal == 0)
			fprintf(stderr, "[WARNING] %s: %s (SDL: %s)\n", function, type, sdlerror);
		else
			fprintf(stderr, "[ERROR] %s: %s (SDL: %s)\n- CLOSING APPLICATION -\n", function, type, sdlerror);

	} else {
		fprintf(stderr, "[WARNING] log_error: NULL pointer received as string\n");

	}
}
