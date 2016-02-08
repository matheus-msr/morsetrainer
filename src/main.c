#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Seed for rand()
#include <SDL2/SDL.h>

#include "input.h"
#include "graphics.h"
#include "error.h"

#define WINW 300
#define WINH 400
#define ASCIIMORSE 1
#define MORSEASCII 2

int main(void)
{
	int opt = 0;
	int char_pos = 0;

	char morse_in[32] = {""};
	char asciilist[26][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
	char morselist[26][6] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

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

	srand(time(NULL));

	if(create_context(&cxt, "Morse Trainer", WINW, WINH) < 0){
		log_error("Couldn't create context", "main", SDL_GetError(), 1);
		close_app(&cxt);
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

		if(clicked(ev, abc_to_morse)) {
			opt = ASCIIMORSE;
		} else if(clicked(ev, morse_to_abc)) {
			opt = MORSEASCII;
		} else if(ev.type == SDL_QUIT) {
			close_app(&cxt);
			return 0;
		}
	}

	clear_screen(&cxt);

	while(opt == ASCIIMORSE)
	{
		// get random number to define char to be trained
		char_pos = rand() % 26;

		if(get_text(&cxt, asciilist[char_pos], morse_in) == SDL_QUIT)
		{
			close_app(&cxt);
			return 0;
		}

		txt_pos.x = 70;
		txt_pos.y = 70;

		if(!strcmp(morselist[char_pos], morse_in)){
			clear_screen(&cxt);
			blit_text(&cxt, 30, "RIGHT ANSWER", "res/font.ttf", txt_pos);
		} else {
			clear_screen(&cxt);
			blit_text(&cxt, 30, "WRONG ANSWER", "res/font.ttf", txt_pos);
		}

		blit_button(&cxt, "res/base_100x200.png", ok, "OK", "res/font.ttf");

		show(cxt);

		while(!clicked(ev, ok))
		{
			// wait 100 ms for the user to release the return key pressed for inputting the text
			SDL_Delay(100);
			const Uint8 *keyboard =  SDL_GetKeyboardState(NULL);

			SDL_PollEvent(&ev);

			// is return pressed? if yes, leave loop, if no, stay in loop
			if(keyboard[SDL_SCANCODE_RETURN] != 0)
				break;

			if(ev.type == SDL_QUIT)
			{
				close_app(&cxt);
				return 0;
			}
		}
	}

	while(opt == MORSEASCII)
	{
	}

	close_app(&cxt);

	return 0;
}
