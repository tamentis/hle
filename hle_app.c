#include <math.h>
#include "SDL.h"
#include "hlegl.h"
#include "hive.h"

extern hle_app *app;

/**
 * Constructor for the application. One global instance will be spawned.
 */
hle_app *
hle_app_new()
{
	int i;

	hle_app *app = hle_malloc(sizeof(hle_app));
	app->player = hle_player_new();

	/* Entity slots */
	app->entities = hle_malloc(sizeof(hle_entity *) * MAX_ENTITIES);
	for (i = 0; i < MAX_ENTITIES; i++)
		app->entities[i] = NULL;
	
	/* Land */
	app->land = hle_land_new();

	return app;
}


/**
 * Destructor for the app, should only be called once at the end of main.
 */
void
hle_app_kill()
{
	hle_free(app->player);
	hle_free(app);
}


/**
 * This is the main game loop.
 */
void
hle_app_main_loop()
{
	int done = 0;
	SDL_Event event;

	/* Walking event vars */
	int walk_forward = 0;
	int walk_backward = 0;
	double walk_speed = 0.05f;

	/* Straffing event vars */
	int strafe_right = 0;
	int strafe_left = 0;
	double strafe_speed = 0.05f;

	/* Turning event vars (keyboard) */
	int turn_left = 0;
	int turn_right = 0;
	double turn_speed = 0.1f;

	/* Mouse sensitivity */
	double mouse_sensitivity = 0.20f;
	double mouse_rotate;
	double mouse_pan;

	/* Create temp objects */
	hle_entity *sign1 = hle_sign_new();
	sign1->z = 1.0;
	hle_entity *sign2 = hle_sign_new();
	sign2->z = 3.0;
	sign2->x = 5.0;
	sign2->rot = 70.0;

	hle_app_register_entity(sign1);
	hle_app_register_entity(sign2);

	while (!done) {
		hle_app_draw();

		/* Mouse turn */
		if (mouse_rotate != 0.0f) {
			hle_entity_set_rot_relative(app->player,
					mouse_rotate * mouse_sensitivity);
			mouse_rotate = 0.0f;
		}

		/* Mouse pan */
		if (mouse_pan != 0.0f) {
			app->player->pan -= mouse_pan * mouse_sensitivity;
			mouse_pan = 0.0f;
		}

		/* Handle walking movements */
		if (walk_forward > walk_backward) {
			double rad_angle = (M_PI / 180.0) * app->player->rot;
			app->player->x -= sin(rad_angle) * walk_speed;
			app->player->y -= cos(rad_angle) * walk_speed;
		} else if (walk_backward > walk_forward) {
			double rad_angle = (M_PI / 180.0) * app->player->rot;
			app->player->x += sin(rad_angle) * walk_speed;
			app->player->y += cos(rad_angle) * walk_speed;
		}

		/* Handle turning movements */
		if (turn_left > turn_right) {
			hle_entity_set_rot_relative(app->player, -turn_speed);
		} else if (turn_right > turn_left) {
			hle_entity_set_rot_relative(app->player, turn_speed);
		}

		/* Handle strafe movements */
		if (strafe_left > strafe_right) {
			double rad_angle = (M_PI / 180.0) * (app->player->rot - 90.0);
			app->player->x -= sin(rad_angle) * strafe_speed;
			app->player->y -= cos(rad_angle) * strafe_speed;
		} else if (strafe_right > strafe_left) {
			double rad_angle = (M_PI / 180.0) * (app->player->rot - 90.0);
			app->player->x += sin(rad_angle) * strafe_speed;
			app->player->y += cos(rad_angle) * strafe_speed;
		}

		/* This could go in a separate function */
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				done = 1;
			
			if (event.type == SDL_MOUSEMOTION) {
				mouse_rotate = event.motion.xrel;
				mouse_pan = event.motion.yrel;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					done = 1;
					break;
				case SDLK_LEFT:
					turn_left = turn_right + 1;
					break;
				case SDLK_RIGHT:
					turn_right = turn_left + 1;
					break;
				case SDLK_w:
				case SDLK_UP:
					walk_forward = walk_backward + 1;
					break;
				case SDLK_s:
				case SDLK_DOWN:
					walk_backward = walk_forward + 1;
					break;
				case SDLK_a:
					strafe_left = strafe_right + 1;
					break;
				case SDLK_d:
					strafe_right = strafe_left + 1;
					break;
				case SDLK_l:
					glEnable(GL_LIGHTING);
					break;
				default:
					break;
				}
			}

			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_LEFT:
						turn_left = 0;
						break;
					case SDLK_RIGHT:
						turn_right = 0;
						break;
					case SDLK_w:
						walk_forward = 0;
						break;
					case SDLK_s:
						walk_backward = 0;
						break;
					case SDLK_a:
						strafe_left = 0;
						break;
					case SDLK_d:
						strafe_right = 0;
						break;
					default:
						break;
				}
			}
		}
	}
}


/**
 * Register one entity in the app, will cause this entity to be drawn and
 * updated in the normal game loop. When we reach the max, a fatal error
 * is thrown.
 */
void
hle_app_register_entity(hle_entity *e)
{
	int i;

	printf("REGISTER!\n");

	for (i = 0; i < MAX_ENTITIES; i++) {
		if (app->entities[i] == NULL) {
			app->entities[i] = e;
			return;
		}
	}

	hle_fatal("Reached MAX_ENTITIES (%d).", MAX_ENTITIES);
}


/**
 * Draw a single entity 
 */
void
hle_app_draw_entity(hle_entity *e)
{
	int i, j;
	int vidx;
	double x;
	double y;
	double z;
	double rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;

	glPushMatrix();

	rot_x += e->model->rot_x;
	rot_y += e->model->rot_y;
	rot_y += e->rot;
	rot_z += e->model->rot_z;

	if (rot_x != 0) glRotatef(rot_x, 1.0f, 0, 0);
	if (rot_y != 0) glRotatef(rot_y, 0, 1.0f, 0);
	if (rot_z != 0) glRotatef(rot_z, 0, 0, 1.0f);

	for (i = 0; i < e->model->fcount; i++) {
		glBegin(GL_POLYGON);
		for (j = 0; j < 4; j++) {
			vidx = e->model->f[i * 4 + j];
			x = e->model->v[vidx * 3 + 0];
			y = e->model->v[vidx * 3 + 1];
			z = e->model->v[vidx * 3 + 2];
			glVertex3f(x + e->x, y + e->y, z + e->z);
		}
		glEnd();
	}

	glPopMatrix();
}


/**
 * Draw all the entities
 */
void
hle_app_draw_entities()
{
	int i;
	hle_entity *e;
	float mcolor[] = { 0.25f, 0.14f, 0.09f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

	for (i = 0; i < MAX_ENTITIES; i++) {
		e = app->entities[i];
		if (e == NULL)
			continue;
		hle_app_draw_entity(e);
	}
}


void
hle_app_draw_lights()
{
	/* white ambient light at half intensity (rgba) */
	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	/* super bright, full intensity diffuse light. */
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	/* position of light (x, y, z, (position of light)) */
	GLfloat LightPosition[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	// set up light number 1.
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  // add lighting. (ambient)
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // add lighting. (diffuse).
	glLightfv(GL_LIGHT1, GL_SPECULAR,LightSpecular); // set light position.
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // set light position.
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}


void
switchToOrtho()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 10, 5, 0, 0, 1);       
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void
switchBackToFrustum ()
{
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void
hle_app_draw_hud()
{
	switchToOrtho();
	glTranslatef(0.0, 0.0, -1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5f, 0, 0);
	glVertex3f( 0.0f,  0.0f, 0.0f);
	glVertex3f( 0.0f,  1.0f, 0.0f);
	glVertex3f( 1.0f,  0.0f, 0.0f);
	glVertex3f( 1.0f,  1.0f, 0.0f);
	glEnd();
	switchBackToFrustum();
}


/**
 * The main drawing function, typically draws the world, then the entities,
 * then the HUD.
 */
void
hle_app_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	hle_app_draw_lights();

	/* The initial position of the world defines the view of the player. */
	glRotatef(app->player->rot, 0, 1.0f, 0); // x, y, z
	glTranslatef(app->player->x, -4.0f, -app->player->y);
	glRotatef(-90.0f, 1.0f, 0, 0);

	/* Draw the land */
	glPushMatrix();
		hle_land_draw(app->land);
	glPopMatrix();

	/* Draw entities */
	glPushMatrix();
		glTranslatef(0.0, 0.0, 4.0f);
		hle_app_draw_entities();
	glPopMatrix();

	/* Return to identity to draw the HUD */
	glPopMatrix();
	hle_app_draw_hud();

	SDL_GL_SwapBuffers();
}
