#ifndef INPUT_H
#define INPUT_H

#include "graphics.h"

#define CLICKED 1
#define NOT_CLICKED 0

int clicked(SDL_Event ev, SDL_Rect pos);

int wait_unpress(SDL_Event * ev);

int get_text(struct s_context *cxt, const char * msg, char * in_text);

#endif
