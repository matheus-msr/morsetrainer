#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "input.h"
#include "graphics.h"

#define WINW 300
#define WINH 400
#define ASCIIMORSE 1
#define MORSEASCII 2

int main(void)
{
	int opt = 0;

	char morse_in[32] = {""};
	char asciilist[26][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m"};
	char morselist[26][6] = {};
	char result[16] = {""};

	struct s_context cxt;

	SDL_Event ev;

	SDL_Rect txt_pos;
	SDL_Rect abc_to_morse;
	SDL_Rect morse_to_abc;
	SDL_Rect ok;

	ok.h = 100;
	ok.w = 200;
	ok.x = 50;
	ok.y = 230;

	abc_to_morse.h = 100;
	abc_to_morse.w = 200;
	abc_to_morse.x = 50;
	abc_to_morse.y = 120;

	morse_to_abc.h = 100;
	morse_to_abc.w = 200;
	morse_to_abc.x = 50;
	morse_to_abc.y = 230;

	// temporary, remove later
	strcpy(morselist[12], "..-.");

	if(create_context(&cxt, "Morse Trainer", WINW, WINH) < 0){
		// deal with error here
	}

	txt_pos.x = 70;
	txt_pos.y = 25;

	blit_text(&cxt, 30, "MORSE TRAINER", "res/font.ttf", txt_pos);

	txt_pos.x = 70;
	txt_pos.y = 70;

	blit_text(&cxt, 30, "CHOOSE MODE:", "res/font.ttf", txt_pos);

	blit_button(&cxt, "res/base_100x200.png", abc_to_morse, "ASCII > MORSE", "res/font.ttf");
	blit_button(&cxt, "res/base_100x200.png", morse_to_abc, "MORSE > ASCII", "res/font.ttf");

	show(cxt);

	// Main menu loop
	while(opt == 0)
	{
		SDL_PollEvent(&ev);

		if(clicked(ev, abc_to_morse))
			opt = ASCIIMORSE;
		else if(clicked(ev, morse_to_abc))
			opt = MORSEASCII;
		else if(ev.type == SDL_QUIT) {
			close_app(&cxt);
			return 0;
		}
	}

	clear_screen(&cxt);

	while(opt == ASCIIMORSE)
	{
		get_text(&cxt, asciilist[12], morse_in);

		if(!strcmp(morselist[12], morse_in)){
			clear_screen(&cxt);
			strcpy(result, "RIGHT ANSWER");
		} else {
			clear_screen(&cxt);
			strcpy(result, "WRONG ANSWER");
		}

		txt_pos.x = 70;
		txt_pos.y = 70;

		blit_text(&cxt, 30, result, "res/font.ttf", txt_pos);

		blit_button(&cxt, "res/base_100x200.png", ok, "OK", "res/font.ttf");

		show(cxt);

		while(!clicked(ev, ok))
		{
			SDL_PollEvent(&ev);

			if(ev.type == SDL_QUIT)
			{
				close_app(&cxt);
				return 0;
			}
		}

		opt = 0;
		break;
	}

	while(opt == MORSEASCII)
	{
	}

	close_app(&cxt);

	return 0;
}
