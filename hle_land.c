/*
 * The "land" is all the ground under your feet, it does not include all
 * the entities. In theory there should only be one land loaded in a given
 * app instance.
 */
#include <math.h>

#include "hlegl.h"
#include "hive.h"

/**
 * Constructor for the world.
 */
hle_land *
hle_land_new()
{
	int i;
	float v;
	hle_land *land = hle_malloc(sizeof(hle_land));
	land->zone_size = 500;
	land->zone_resolution = 100;
	land->width = 1;
	land->height = 1;
	/*
	land->width = LAND_WIDTH;
	land->height = LAND_HEIGHT;
	*/
	land->data = hle_strcp(
	    "wwwssddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"
	    "wwsddddddsswwsswwwssssssssssssswwwwwwwwwwpwwwpwdddddgdddddddgddd"
	    "wssdddggggsswwwwwssssswssssssssswwwpwwwpwppwpwwddddddgsssssgdddd"
	    "wssddgggggswwwwwwwssswsgggggggssswwpwpwpwpwwppwddddddsspspssdddd"
	    "wsddddggggswswsswwwwwwsggggggggssswwpwpwwwpwpwwwdddddsspspssdddd"
	    "wssdddggggswdwdswwwwwwsggssssgggsssswwwwwwwwwwwpdddddsssssssdddd"
	    "wsssdddgggswwwwwwwwwwssggswwsgggggssssssswwwwwwwwddddsssssssdddd"
	    "wwssddddggsswwwwwwwwwsgggswwsggggggssssssswswssssdddddspppsddddd"
	    "wwsssddddgggssswswsswsggggswsggggggggssssssssssssdddddpssspddddd"
	    "wwwsssddddggggswswsswwsggggssgggggggggsssgsgssssssdddddddddddddd"
	    "wwwwsssddddgggssgsssswwgggggggggggggggggggggggggggsdddddpppppppp"
	    "wwwwwssdddddggggggggsswwggggggggggggggggggggggggggssdddppspspsps"
	    "wwwwwsssdddddggggggggssgwwgggggggggssgggggssssssssssddpppppppppp"
	    "wwwwwssssddddddggggggggggwwwwwwwwwwwssssssssssssssssgdpsppdddddd"
	    "wwwwwwwsssdddddddggggggggggggggggggwwwsssswwwwsssssgggpppddddddd"
	    "wwwwwwwwsssddddddddddgsgggsggsgsgssggswwwwwwwsswssggggpspggddddd"
	    "wwwwwwwwssssdddddddgggsggsgsgsssgsgsgsswwwwwwwwwssggggpppgdddwdd"
	    "wwwwwwwwwssssdddddddggsggsssgsgsgsgsgsswwwwwwwwwssggggpspgddgddd"
	    "wwwwwwwwwwssddddddddggsggsgsgsgsgsgsgsswwwwwwwwwwsggggpppgdddddd"
	    "wwgwwwwwwwwdddddddddddssgsgsgsgsgssggsswwwwwwwwwsssgggpspgdddddd"
	    "wwgwwwwwssssdssssddddddgggggggggggggsssswwwwwwwwwwsgggpppggddddd"
	    "wwgwwwwsswsssssssssdddddwwgwwwdgggggssssswwwwwwssssgggpspggddddd"
	    "wpppwwswwwwwwwwsssssddddddwwwddgggggggssswwwwwwsggggggpppgggdddd"
	    "wpppwwwwwwwwwwwwsssssddddddddddgggggppppppwwwssssgggggpspgggdddd"
	    "wpppwwwwwwwwwwwwwssssssdddddddddgggppppppppssssgggggggpppgggdddd"
	    "wwpwwwwwwwwwwwwwwwwsssssddddddddddppppppppppsgggggggggpspggggddd"
	    "pwpwpwwgwwgwwwwwwwwwsssssdddddddddppppggpppppppppppppppppgpggddd"
	    "wpppwwggggggwwwwwwwwwssssssdddddddpppggggpspspspspspspsppggggddd"
	    "wwpgwggwggwgwwwwwwwwwwwssssssdddddpppggggppppppppppppppppgggdddd"
	    "wwpwgwgggggggwgwwwwwwwwwwssssssdddppppggppppddgggggggggggggddddd"
	    "wpwpwwggggggwgwwwwwwwwwwwwwsssssssdppppppppdddddddddggggggdddddd"
	    "wpwpwwwgwwgwwwwwwwwwwwwwwwwwssssssssppppppdddddddddddddddddddddd");

	land->alt = hle_malloc(sizeof(float) * land->zone_resolution * 
			land->zone_resolution);

	/* Generate the random altitudes */
	for (i = 0; i < land->zone_resolution * land->zone_resolution; i++) {
		land->alt[i] = (random() % 100) / 50.0f;
	}

	return land;
}


/**
 * This should ultimately load the data from the http service, in a separate
 * thread and will update the land.
 */
void
hle_land_load_data(char *data)
{

}


/**
 * Using the *data pointer, create all the necessary land pieces.
 */
void
hle_land_draw(hle_land *land)
{
	int i, j;
	char c;
	int offset;
	float mcolor[4];
	mcolor[3] = 1.0f;

	glPushMatrix();

	/* Base triangle, only here for fun */
	float mcolor2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor2);
	glBegin(GL_POLYGON);
	glVertex3f( 0.0f, 1.0f, 0.1f);
	glVertex3f( 1.0f,-1.0f, 0.1f);
	glVertex3f(-1.0f,-1.0f, 0.1f);
	glEnd();

	for (i = 0; i < land->height; i++) {
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0f, land->zone_size * i, 0.0f);

		for (j = 0; j < land->width; j++) {
			offset = land->width * i + j;
			c = land->data[offset];

			switch (c) {
			case 'w':
				mcolor[0] = 0.25f;
				mcolor[1] = 0.44f;
				mcolor[2] = 0.93f;
				break;
			case 'd':
				mcolor[0] = 0.50f;
				mcolor[1] = 0.28f;
				mcolor[2] = 0.16f;
				break;
			case 'g':
				mcolor[0] = 0.39f;
				mcolor[1] = 0.68f;
				mcolor[2] = 0.28f;
				break;
			case 'i':
				mcolor[0] = 0.91f;
				mcolor[1] = 0.95f;
				mcolor[2] = 0.95f;
				break;
			case 'k':
				mcolor[0] = 0.94f;
				mcolor[1] = 0.77f;
				mcolor[2] = 0.76f;
				break;
			case 'l':
				mcolor[0] = 0.93f;
				mcolor[1] = 0.28f;
				mcolor[2] = 0.19f;
				break;
			case 'p':
				mcolor[0] = 0.33f;
				mcolor[1] = 0.33f;
				mcolor[2] = 0.36f;
				break;
			case 's':
				mcolor[0] = 0.93f;
				mcolor[1] = 0.78f;
				mcolor[2] = 0.38f;
				break;
			case 'a':
			default:
				mcolor[0] = 0.00f;
				mcolor[1] = 0.00f;
				mcolor[2] = 0.00f;
				break;
			}
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
			
			hle_land_draw_zone(land);

			glTranslatef(-land->zone_size, 0.0f, 0.0f);
		}
	}

	glPopMatrix();
}

float
hle_land_get_alt(hle_land *land, int x, int y)
{
	if (y >= land->zone_resolution)
		y = land->zone_resolution - 1;
	if (x >= land->zone_resolution)
		y = land->zone_resolution - 1;
	return land->alt[y * land->zone_resolution + x];
}

void
hle_land_draw_zone(hle_land *land)
{
	float l = land->zone_size / land->zone_resolution;
	float w = land->zone_resolution;
	float x = 0, y = 0;

	glPushMatrix();

	for (y = 0; y < land->zone_resolution; y++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (x = 0; x < land->zone_resolution; x++) {
			glVertex3f(x * l, y * l,
					hle_land_get_alt(land, x, y));
			glVertex3f(x * l, y * l + l, 
					hle_land_get_alt(land, x, y + 1));
		}
		glEnd();
	}

	glPopMatrix();
}
