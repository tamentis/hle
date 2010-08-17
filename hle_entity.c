#include "hive.h"

hle_entity *
hle_entity_new()
{
	hle_entity *e = hle_malloc(sizeof(hle_entity));
	e->x = 0.0f;
	e->y = 0.0f;
	e->rot = 0.0f;
	e->pan = 0.0f;
	e->type = ETYPE_UNKNOWN;
	e->model = NULL;
	e->model_path = NULL;
	return e;
}

/**
 * Increment the rotation and make sure we stay within boundaries.
 */
void
hle_entity_set_rot_relative(hle_entity *e, double value)
{
	e->rot += value;
	if (e->rot > 360.0f)
		e->rot -= 360.0f;
	else if (e->rot < 0.0f)
		e->rot += 360.0f;
}


void
hle_entity_set_model_path(hle_entity *e, char *model_path)
{
	if (e->model_path != NULL)
		hle_free(e->model_path);

	e->model_path = hle_strcp(model_path);

	if (e->model != NULL)
		hle_free(e->model);

	e->model = hle_load_faces_from_obj(model_path);
}


/**
 * Create a new sign.
 */
hle_entity *
hle_sign_new()
{
	hle_entity *e = hle_entity_new();
	e->type = ETYPE_SIGN;
	hle_entity_set_model_path(e, "data/models/dfsign.obj");
	e->model->rot_x = 90.0;
	return e;
}
