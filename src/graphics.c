#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "error.h"
#include "graphics.h"
#include "input.h"

// Blit a button on the main screen surface
int blit_button(struct s_context *cxt, const char *path, SDL_Rect pos, const char * text, const char *fpath)
{
	int t = strlen(text);

	SDL_Colour c;

	SDL_Rect txtpos;

	SDL_Surface *tmp;
	SDL_Surface *b = IMG_Load(path);

	TTF_Font *f = TTF_OpenFont(fpath, 36); // 16x16 pixels by character


	if(b == NULL)
	{
		log_error("Could not create button surface", "blit_button", IMG_GetError(), 0);
		return -1;
	}

	// Set text to fully opaque black (0x00A0A0)
	c.r = 0x00;
	c.g = 0x00;
	c.b = 0x00;
	c.a = 0xFF;

	tmp = TTF_RenderText_Blended(f, text, c);

	if(tmp == NULL)
	{
		log_error("Could not put text on button surface", "blit_button", TTF_GetError(), 0);
		return -1;
	}

	// The text must be less than 13 characters long
	// centralizes the string 'text' in the button
	txtpos.x = 105 - ((t * 16) / 2);
	txtpos.y = 30;

	SDL_BlitSurface(tmp, NULL, b, &txtpos);
	SDL_BlitSurface(b, NULL, cxt->screen, &pos);

	// Clean up the mess
	SDL_FreeSurface(tmp);
	SDL_FreeSurface(b);
	TTF_CloseFont(f);

	return 0;
}

int close_app(struct s_context *ctx)
{
	SDL_FreeSurface(ctx->screen);
	SDL_DestroyWindow(ctx->win);
	SDL_DestroyRenderer(ctx->ren);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}

int create_context(struct s_context *cxt, const char *title, const int sizex, const int sizey)
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		log_error("Could not initialize SDL", "create_context", SDL_GetError(), 1);
		close_app(cxt);
	}

	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		log_error("Could not initialize SDL_image", "create_context", IMG_GetError(), 1);
		close_app(cxt);
	}

	if(TTF_Init())
	{
		log_error("Could not initialize SDL_ttf", "create_context", TTF_GetError(), 1);
		close_app(cxt);
	}


	cxt->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizex, sizey, SDL_WINDOW_SHOWN);
	cxt->ren = SDL_CreateRenderer(cxt->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(cxt->win == NULL || cxt->ren == NULL)
	{
		log_error("Can't create window and/or renderer", "create_context", SDL_GetError(), 1);
		close_app(cxt);
	}

	// Set background as grey (0xD3D3D3)
	cxt->screen = SDL_GetWindowSurface(cxt->win);
	SDL_FillRect(cxt->screen, NULL, SDL_MapRGB(cxt->screen->format, 0xD3, 0xD3, 0xD3));

	return 0;
}

int show(struct s_context cxt)
{
	SDL_Texture *tmp = SDL_CreateTextureFromSurface(cxt.ren, cxt.screen);

	SDL_RenderCopy(cxt.ren, tmp, NULL, NULL);

	SDL_RenderPresent(cxt.ren);

	SDL_DestroyTexture(tmp);

	return 0;
}

int blit_text(struct s_context * cxt, const int size, const char *text, const char *fpath, SDL_Rect pos)
{
	SDL_Surface *tmp;

	SDL_Colour c;

	TTF_Font *f = NULL;

	f = TTF_OpenFont(fpath, size);

	// fully opaque black (0x000000)
	c.r = 0x00;
	c.g = 0x00;
	c.b = 0x00;
	c.a = 0xFF;

	// in case there's nothing to blit here
	if(strlen(text) == 0)
		return -1;

	tmp = TTF_RenderText_Blended(f, text, c);

	if(tmp == NULL)
	{
		log_error("Could not put text on temporary surface", "blit_text", TTF_GetError(), 0);
		return -3; // Coudn't render
	}

	SDL_BlitSurface(tmp, NULL, cxt->screen, &pos);

	SDL_FreeSurface(tmp);
	TTF_CloseFont(f);

	return 0;
}

int clear_screen(struct s_context * cxt)
{
	// Clean main surface
	cxt->screen = SDL_GetWindowSurface(cxt->win);
	SDL_FillRect(cxt->screen, NULL, SDL_MapRGB(cxt->screen->format, 0xD3, 0xD3, 0xD3));

	return 0;
}

int blit_element(struct s_context * cxt, const char * path, SDL_Rect pos)
{
	SDL_Surface *element = IMG_Load(path);

	if(element == NULL)
	{
		log_error("Coudn't load image file as surface", "blit_element", IMG_GetError(), 0);
		return -2; // Coudn't load resource
	}

	SDL_BlitSurface(element, NULL, cxt->screen, &pos);

	SDL_FreeSurface(element);

	return 0;
}
