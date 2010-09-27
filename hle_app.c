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

	/* Controls */
	app->controls = hle_controls_new();

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
	SDL_Event	event;
	uint32_t	start,		// when the game loop started
			now,		// time for each loop
			framecount = 0, // numbers of frames drawn in a tick
			lastframe = 0;	// whenever the last frame was drawn

	hle_app_setup_game_world();

	start = SDL_GetTicks();
	while (!app->controls->exit) {
		now = SDL_GetTicks();

		/* Pump events into the control object. */
		while (SDL_PollEvent(&event))
			hle_controls_read_from_event(app->controls, event);

		/* Handle user inputs */
		hle_app_handle_events(now);

		/* Actual game logic */
		hle_app_game_logic(now);

		/* Render at maximum MAXFPS */
		if (lastframe < (now - 1000/MAXFPS)) {
			framecount++;
			lastframe = now;
			hle_app_draw();
		}
	}
}


/**
 * Setup a few initial objects.
 */
void
hle_app_setup_game_world()
{
	hle_entity *sign1,
		   *sign2;
	
	sign1 = hle_sign_new();
	sign1->z = 1.0;
	hle_app_register_entity(sign1);

	sign2 = hle_sign_new();
	sign2->z = 13.0;
	sign2->x = 5.0;
	sign2->rot = 70.0;
	hle_app_register_entity(sign2);

	/* Setup initial player entity */
	hle_entity_set_model_path(app->player, "data/models/dfsign.obj");
	app->player->model->rot_x = 90.0;
	hle_app_register_entity(app->player);
}


/**
 * Handle all the entity movements.
 */
void
hle_app_game_logic(uint32_t now)
{
}


/**
 * Handle all the input and alter the control object of the application.
 *
 * Some events should only be triggered on a frame basis, this will
 * make sure everybody walks at the same speed on all hardware, but
 * it will also avoid a one hit button from being lost between two
 * frames.
 */
void
hle_app_handle_events(uint32_t now)
{
	hle_controls *c = app->controls;
	int event_frame_size = 8;

	if (now - event_frame_size > c->last) {
		hle_app_handle_framed_events(now);
		c->last = now;
	}

	if (c->show_console == HLE_TRUE) {
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_ShowCursor(1);
	} else {
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(0);
	}

	hle_app_handle_hit_events(now);
}


/**
 * Handle all the events that need to be handled at time of keypress (hit
 * based).
 */
void
hle_app_handle_hit_events(uint32_t now)
{
}


/**
 * Handle all the events that need to be triggered on a time basis (per frame).
 */
void
hle_app_handle_framed_events(uint32_t now)
{
	hle_controls *c = app->controls;

	float walk_speed = 0.5f;
	float strafe_speed = 0.5f;
	float turn_speed = 0.1f;
	/*
	 * from math import sin
	 * ", ".join(["%.2f" % (sin(x/12.0) * 4.0) for x in range(40)] + [0])
	 */
	float jump_steps[] = {
		0.00, 0.33, 0.66, 0.99, 1.31, 1.62, 1.92, 2.20, 2.47, 2.73,
		2.96, 3.17, 3.37, 3.53, 3.68, 3.80, 3.89, 3.95, 3.99, 4.00,
		3.98, 3.94, 3.86, 3.76, 3.64, 3.49, 3.31, 3.11, 2.89, 2.65,
		2.39, 2.12, 1.83, 1.53, 1.21, 0.89, 0.56, 0.23, -0.10, -0.43,
		0 };
	float mouse_sensitivity = 0.20f;
	float rad_angle;

	/* Mouse turn */
	if (c->mouse_rotate != 0.0f) {
		hle_entity_set_rot_relative(app->player,
				c->mouse_rotate * mouse_sensitivity);
		c->mouse_rotate = 0.0f;
	}

	/* Mouse pan */
	if (c->mouse_pan != 0.0f) {
		app->player->pan -= c->mouse_pan * mouse_sensitivity;
		c->mouse_pan = 0.0f;
	}

	/* Handle walking movements */
	if (c->walk_forward > c->walk_backward) {
		rad_angle = (M_PI / 180.0) * app->player->rot;
		app->player->x -= sin(rad_angle) * walk_speed;
		app->player->y -= cos(rad_angle) * walk_speed;
	} else if (c->walk_backward > c->walk_forward) {
		rad_angle = (M_PI / 180.0) * app->player->rot;
		app->player->x += sin(rad_angle) * walk_speed;
		app->player->y += cos(rad_angle) * walk_speed;
	}

	/* Handle turning movements */
	if (c->turn_left > c->turn_right) {
		hle_entity_set_rot_relative(app->player, -turn_speed);
	} else if (c->turn_right > c->turn_left) {
		hle_entity_set_rot_relative(app->player, turn_speed);
	}

	/* Handle strafe movements */
	if (c->strafe_left > c->strafe_right) {
		rad_angle = (M_PI / 180.0) * (app->player->rot - 90.0);
		app->player->x -= sin(rad_angle) * strafe_speed;
		app->player->y -= cos(rad_angle) * strafe_speed;
	} else if (c->strafe_right > c->strafe_left) {
		rad_angle = (M_PI / 180.0) * (app->player->rot - 90.0);
		app->player->x += sin(rad_angle) * strafe_speed;
		app->player->y += cos(rad_angle) * strafe_speed;
	}

	/* Handle jumping */
	if (c->jumping) {
		app->player->z = jump_steps[c->jump_step_idx];
		c->jump_step_idx++;
		if (jump_steps[c->jump_step_idx] == 0) {
			c->jump_step_idx = 0;
			c->jumping = 0;
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
	float rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f;

	glPushMatrix();

	rot_x += e->model->rot_x;
	rot_y += e->model->rot_y;
	if (e->invert_rotation == HLE_TRUE)
		rot_y -= e->rot;
	else
		rot_y += e->rot;
	rot_z += e->model->rot_z;

	/* Move the cursor at the entity's location */
	glTranslatef(-1.0f * e->x, -1.0f * e->y, e->z);

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
			glVertex3f(x, y, z);
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
	GLfloat LightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	/* position of light (x, y, z, (position of light)) */
	GLfloat LightPosition[] = { - app->player->x, - app->player->y,
		12.0f + app->player->z, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  // add lighting. (ambient)
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // add lighting. (diffuse).
	glLightfv(GL_LIGHT1, GL_SPECULAR,LightSpecular); // set light position.
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // set light position.
	glEnable(GL_LIGHT1);

	/*
	GLfloat Light2Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light2Position[] = { app->player->x, app->player->y,
		30.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_DIFFUSE, Light2Diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION,Light2Position);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.08f);

	glEnable(GL_LIGHT2);
	*/

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

	if (app->controls->show_console) {
		glTranslatef(1.5, 0.0, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(0.5f, 0, 0);
		glVertex3f( 0.0f,  0.0f, 0.0f);
		glVertex3f( 0.0f,  2.0f, 0.0f);
		glVertex3f( 8.0f,  0.0f, 0.0f);
		glVertex3f( 8.0f,  2.0f, 0.0f);
		glEnd();
	}

	switchBackToFrustum();
}

/**
 * The main drawing function, typically draws the world, then the entities,
 * then the HUD.
 */
void
hle_app_draw()
{
	float cam_x, cam_y, rad_angle;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	/* Draw the atmosphere */
	glPushMatrix();
		hle_atmosphere_draw(app);
	glPopMatrix();

	/* 
	 * In first person view, the initial position of the world defines 
	 * the view of the player.
	 */
	cam_x = app->player->x;
	cam_y = -app->player->y;

	if (app->controls->view_mode == VMODE_FIRST_PERSON) {
		cam_x = app->player->x;
		cam_y = -app->player->y;
	} else {
		rad_angle = (M_PI / 180.0) * app->player->rot;
		cam_x += sin(rad_angle) * 10.0f;
		cam_y -= cos(rad_angle) * 10.0f;
	}
	glRotatef(app->player->rot, 0, 1.0f, 0);
	glTranslatef(cam_x, -4.0f - app->player->z, cam_y);
	glRotatef(-90.0f, 1.0f, 0, 0);

	hle_app_draw_lights();

	/* Draw the land */
	glPushMatrix();
		hle_land_draw(app->land);
	glPopMatrix();

	/* Draw entities */
	glPushMatrix();
		hle_app_draw_entities();
	glPopMatrix();

	/* Return to identity to draw the HUD and Console */
	glPopMatrix();
	hle_app_draw_hud();

	SDL_GL_SwapBuffers();
}
