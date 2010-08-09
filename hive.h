#include <stdlib.h>

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

typedef struct hle_obj_model_t {
	double *v;
	int vcount;
	int *f;
	int fcount;
	double x;
	double y;
	double z;
	double rot_x;
	double rot_y;
	double rot_z;
} hle_obj_model;

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

typedef struct hle_land_t {
	unsigned int width;
	unsigned int height;
	char *data;
} hle_land;

typedef struct hle_app_t {
	hle_entity	*player;
	hle_entity	**entities;
	hle_land	*land;
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

