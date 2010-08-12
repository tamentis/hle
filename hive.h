#include <stdlib.h>
#include "SDL.h"

#define MAX_ENTITIES 256

enum etype {
	ETYPE_UNKNOWN = 0,
	ETYPE_PLAYER = 1,
	ETYPE_SIGN = 2
};

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define LAND_WIDTH 64
#define LAND_HEIGHT 32

#define MAXFPS 60

/*
 * Represent a model in the 3D space, its list of vertices (*v), it's list
 * of faces (*f) and its initial orientation. It will also contain 
 * the material.
 */
typedef struct hle_obj_model_t {
	int	 vcount;
	int	 fcount;
	int	*f;
	double	*v;
	double	 rot_x;
	double	 rot_y;
	double	 rot_z;
} hle_obj_model;

/*
 * An entity is a an object or a thing in the HLE world. It has a model, a
 * location and orientation in space.
 */
typedef struct hle_entity_t {
	unsigned int type;
	double x;
	double y;
	double z;
	double rot;
	double pan;
	hle_obj_model *model;
	char *model_path;
} hle_entity;

/*
 * Representation of the land mass of the game, imported from the site.
 */
typedef struct hle_land_t {
	unsigned int	 width;
	unsigned int	 height;
	char		*data;
	float		*alt;
} hle_land;

/*
 * Represent the current status of the input controls.
 */
typedef struct hle_controls_t {
	/* Keyboard Events */
	int		 turn_left;
	int		 turn_right;
	int		 jumping;
	int		 strafe_left;
	int		 strafe_right;
	int		 walk_forward;
	int		 walk_backward;
	/* Jump status */
	int		 jump_step_idx;
	/* Mouse Events */
	float		 mouse_rotate;
	float		 mouse_pan;
	/* Application Events */
	int		 exit;
	/* Time tickers */
	uint32_t	 last;
} hle_controls;

typedef struct hle_app_t {
	hle_entity	*player;
	hle_entity	**entities;
	hle_land	*land;
	hle_controls	*controls;
} hle_app;

/* strlcpy.c */
size_t		 strlcpy(char *, const char *, size_t);

/* memory.c */
void		*hle_malloc(size_t);
void		 hle_free(void *);
char		*hle_strcp(char *);
void		 hle_checkmem();

/* hle_entity.c */
hle_entity	*hle_entity_new();
void		 hle_entity_set_rot_relative(hle_entity *, double);
hle_entity	*hle_sign_new();

/* hle_app.c */
hle_app		*hle_app_new();
void		 hle_app_main_loop();

/* fatal.c */
void		*hle_fatal(char *fmt, ...);

