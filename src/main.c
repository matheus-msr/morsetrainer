#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Seed for rand()
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "input.h"
#include "graphics.h"
#include "error.h"

#define WINW 300
#define WINH 400
#define ASCIIMORSE 1
#define MORSEASCII 2
#define MAIN_MENU 3
#define CLICKED_OK 4
#define CLICKED_BACK 5

int main(void)
{
	int opt = 0;
	int char_pos = 0;
	int ok_click = 0;

	char morse_in[32] = {""};
	char asciilist[36][2] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	char morselist[36][6] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};
	char filename[32] = {""};

	struct s_context cxt;

	Mix_Music *morsesound;

	SDL_Event ev;

	SDL_Rect txt_pos;
	SDL_Rect abc_to_morse;
	SDL_Rect morse_to_abc;
	SDL_Rect ok;
	SDL_Rect back;

	ok.h = 100;
	ok.w = 200;
	ok.x = 50;
	ok.y = 130;

	back.h = 100;
	back.w = 200;
	back.x = 50;
	back.y = 240;

	abc_to_morse.h = 100;
	abc_to_morse.w = 200;
	abc_to_morse.x = 50;
	abc_to_morse.y = 120;

	morse_to_abc.h = 100;
	morse_to_abc.w = 200;
	morse_to_abc.x = 50;
	morse_to_abc.y = 230;

	srand(time(NULL));

	if(create_context(&cxt, "Morse Trainer", WINW, WINH) < 0)
	{
		log_error("Couldn't create context", "main", SDL_GetError(), 1);
		close_app(&cxt);
	}

	if(Mix_Init(0))
		log_error("Couldn't init SDL_Mixer", "main", Mix_GetError(), 0);

	// Open audio device
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		log_error("Couldn't open audio device", "main", Mix_GetError(), 0);

	opt = MAIN_MENU;

	while(opt != 0)
	{
		// Main menu loop
		if(opt == MAIN_MENU)
		{
			txt_pos.x = 70;
			txt_pos.y = 25;

			blit_text(&cxt, 30, "MORSE TRAINER", "res/font.ttf", txt_pos);

			txt_pos.x = 70;
			txt_pos.y = 70;

			blit_text(&cxt, 30, "CHOOSE MODE:", "res/font.ttf", txt_pos);

			blit_button(&cxt, "res/base_100x200.png", abc_to_morse, "ASCII > MORSE", "res/font.ttf");
			blit_button(&cxt, "res/base_100x200.png", morse_to_abc, "MORSE > ASCII", "res/font.ttf");

			show(cxt);

			while(opt == MAIN_MENU)
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
		}

		// get random number to define char to be trained
		char_pos = rand() % 36;

		txt_pos.x = 70;
		txt_pos.y = 70;

		if(opt == MORSEASCII){

			// Play sound
			sprintf(filename, "res/snd/%c_morse_code.ogg", asciilist[char_pos][0]);
			morsesound = Mix_LoadMUS(filename);
			if(Mix_PlayMusic(morsesound, -1) < 0 || morsesound == NULL)
				log_error("Couldn't load and play sound", "main", Mix_GetError(), 0);

			if(get_text(&cxt, morselist[char_pos], morse_in) == SDL_QUIT)
			{
				close_app(&cxt);
				return 0;
			}

			// Stop sound
			Mix_HaltMusic();
			Mix_FreeMusic(morsesound);

			morse_in[0] = toupper(morse_in[0]);

			if(!strcmp(asciilist[char_pos], morse_in)){
				clear_screen(&cxt);
				blit_text(&cxt, 30, "RIGHT ANSWER", "res/font.ttf", txt_pos);
			} else {
				clear_screen(&cxt);
				blit_text(&cxt, 30, "WRONG ANSWER", "res/font.ttf", txt_pos);
			}

			blit_button(&cxt, "res/base_100x200.png", ok, "OK", "res/font.ttf");
			blit_button(&cxt, "res/base_100x200.png", back, "BACK", "res/font.ttf");

			show(cxt);

			strcpy(morse_in, "");

		} else if(opt == ASCIIMORSE) {
			if(get_text(&cxt, asciilist[char_pos], morse_in) == SDL_QUIT)
			{
				close_app(&cxt);
				return 0;
			}

			if(!strcmp(morselist[char_pos], morse_in)){
				clear_screen(&cxt);
				blit_text(&cxt, 30, "RIGHT ANSWER", "res/font.ttf", txt_pos);
			} else {
				clear_screen(&cxt);
				blit_text(&cxt, 30, "WRONG ANSWER", "res/font.ttf", txt_pos);
			}

			blit_button(&cxt, "res/base_100x200.png", ok, "OK", "res/font.ttf");
			blit_button(&cxt, "res/base_100x200.png", back, "BACK", "res/font.ttf");

			show(cxt);

			strcpy(morse_in, "");
		}

		ok_click = 0;

		while(ok_click != CLICKED_OK && ok_click != CLICKED_BACK)
		{
			// wait 100 ms for the user to release the return key pressed for inputting the text
			SDL_Delay(50);
			const Uint8 *keyboard =  SDL_GetKeyboardState(NULL);

			wait_unpress(&ev);
			SDL_PollEvent(&ev);

			// is return pressed? if yes, leave loop, if no, stay in loop
			if(keyboard[SDL_SCANCODE_RETURN] || clicked(ev, ok)){
				ok_click = CLICKED_OK;

			} else if(keyboard[SDL_SCANCODE_ESCAPE] || clicked(ev, back)) {
				clear_screen(&cxt);
				opt = MAIN_MENU;
				ok_click = CLICKED_BACK;
			}

			if(ev.type == SDL_QUIT)
			{
				close_app(&cxt);
				return 0;
			}
		}
	}

	close_app(&cxt);
	return 0;
}
