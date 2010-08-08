/* This is rudimentary, only loads three vertices faces and returns an
 * array of all theses faces. */

#include <stdio.h>
#include "hive.h"

#define MAX_VERTICES	12288
#define MAX_FACES	4096
#define MAX_LINE_WIDTH	80

hle_obj_model *
hle_load_faces_from_obj(char *filename)
{
	FILE *fp = fopen(filename, "r");
	char line[MAX_LINE_WIDTH] = "";
	int i;
	char c;
	double x, y, z;
	int v1, v2, v3, v4, m1, m2, m3, m4;

	double vertices[MAX_VERTICES * 3];
	int vertex_count = 0;

	int faces[MAX_FACES * 4];
	int face_count = 0;

	i = 0;
	while (!feof(fp)) {
		c = fgetc(fp);
		if (c == '\r')
			continue;
		if (i < MAX_LINE_WIDTH)
			line[i] = c;
		if (c == '\n' || c == EOF) {
			if (i >= MAX_LINE_WIDTH)
				i = MAX_LINE_WIDTH - 1;
			line[i] = '\0';

			if (line[1] != ' ') {
				i = 0;
				continue;
			}

			switch (line[0]) {
			case '#':
				break;
			case 'v':
				sscanf(line + 2, "%lf %lf %lf", &x, &y, &z);
				vertices[vertex_count * 3 + 0] = x;
				vertices[vertex_count * 3 + 1] = y;
				vertices[vertex_count * 3 + 2] = z;
				vertex_count++;
				break;
			case 'f':
				sscanf(line + 2, "%d/%d %d/%d %d/%d %d/%d",
						&v1, &m1, &v2, &m2,
						&v3, &m3, &v4, &m4);
				faces[face_count * 4 + 0] = v1 - 1;
				faces[face_count * 4 + 1] = v2 - 1;
				faces[face_count * 4 + 2] = v3 - 1;
				faces[face_count * 4 + 3] = v4 - 1;
				face_count++;
				break;
			default:
				break;
			}
			i = 0;
			continue;
		}
		i++;
	}

	fclose(fp);

	hle_obj_model *model = hle_malloc(sizeof(hle_obj_model));
	model->x = 0;
	model->y = 0;
	model->z = 0;
	model->rot_x = 0;
	model->rot_y = 0;
	model->rot_z = 0;
	model->v = hle_malloc(sizeof(double) * vertex_count * 3);
	model->vcount = vertex_count;
	model->f = hle_malloc(sizeof(int) * face_count * 4);
	model->fcount = face_count;

	for (i = 0; i < vertex_count * 3; i++)
		model->v[i] = vertices[i];
	for (i = 0; i < face_count * 4; i++)
		model->f[i] = faces[i];

	return model;
}

void
hle_obj_model_kill(hle_obj_model *model)
{
	hle_free(model->v);
	hle_free(model->f);
	hle_free(model);
}
