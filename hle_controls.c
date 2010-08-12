#include "SDL.h"
#include "hive.h"

hle_controls *
hle_controls_new()
{
	hle_controls *c = hle_malloc(sizeof(hle_controls));

	/* Keyboard events */
	c->turn_left = 0;
	c->turn_right = 0;
	c->jumping = 0;
	c->strafe_left = 0;
	c->strafe_right = 0;
	c->walk_forward = 0;
	c->walk_backward = 0;

	/* Jump handling */
	c->jump_step_idx = 0;

	/* Mouse events */
	c->mouse_rotate = 0.0f;
	c->mouse_pan = 0.0f;

	/* Application events */
	c->exit = 0;

	/* Tickers */
	c->last = SDL_GetTicks();
	return c;
}

void
hle_controls_read_from_event(hle_controls *c, SDL_Event event)
{
	if (event.type == SDL_QUIT)
		c->exit = 1;
	
	if (event.type == SDL_MOUSEMOTION) {
		c->mouse_rotate = event.motion.xrel;
		c->mouse_pan = event.motion.yrel;
	}

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			c->exit = 1;
			break;
		case SDLK_LEFT:
			c->turn_left = c->turn_right + 1;
			break;
		case SDLK_RIGHT:
			c->turn_right = c->turn_left + 1;
			break;
		case SDLK_SPACE:
			c->jumping = 1;
			break;
		case SDLK_w:
		case SDLK_UP:
			c->walk_forward = c->walk_backward + 1;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			c->walk_backward = c->walk_forward + 1;
			break;
		case SDLK_a:
			c->strafe_left = c->strafe_right + 1;
			break;
		case SDLK_d:
			c->strafe_right = c->strafe_left + 1;
			break;
		default:
			break;
		}
	}

	if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
			c->turn_left = 0;
			break;
		case SDLK_RIGHT:
			c->turn_right = 0;
			break;
		case SDLK_SPACE:
			// c->jumping = 0;
			break;
		case SDLK_w:
			c->walk_forward = 0;
			break;
		case SDLK_s:
			c->walk_backward = 0;
			break;
		case SDLK_a:
			c->strafe_left = 0;
			break;
		case SDLK_d:
			c->strafe_right = 0;
			break;
		default:
			break;
		}
	}
}
