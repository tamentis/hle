#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>	// Header File For The OpenGL32 Library
#include <OpenGL/glu.h>	// Header File For The GLu32 Library
#else
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#endif
#include "SDL.h"
#include "hive.h"

hle_app *app;

void
init_gl(int width, int height)
{
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

int
main(int argc, char **argv) 
{  
	/* Initialize SDL for video output */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* Create  screen */
	if (SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_OPENGL) == NULL) {
		fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	/* Set the title bar in environments that support it */
	SDL_WM_SetCaption("Hiveland Explorer", NULL);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(0);

	init_gl(WINDOW_WIDTH, WINDOW_HEIGHT);

	app = hle_app_new();
	hle_app_main_loop(app);

	hle_app_kill();
	hle_checkmem();
	SDL_Quit();
	return 1;
}
