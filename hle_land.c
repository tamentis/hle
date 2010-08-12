/*
 * The "land" is all the ground under your feet, it does not include all
 * the entities. In theory there should only be one land loaded in a given
 * app instance.
 */

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
	land->width = LAND_WIDTH;
	land->height = LAND_HEIGHT;
	land->data = hle_strcp("wwwssdddddddddddddddddddddddddddddddddddddddddddddddddddddddddddwwsddddddsswwsswwwssssssssssssswwwwwwwwwwpwwwpwdddddgdddddddgdddwssdddggggsswwwwwssssswssssssssswwwpwwwpwppwpwwddddddgsssssgddddwssddgggggswwwwwwwssswsgggggggssswwpwpwpwpwwppwddddddsspspssddddwsddddggggswswsswwwwwwsggggggggssswwpwpwwwpwpwwwdddddsspspssddddwssdddggggswdwdswwwwwwsggssssgggsssswwwwwwwwwwwpdddddsssssssddddwsssdddgggswwwwwwwwwwssggswwsgggggssssssswwwwwwwwddddsssssssddddwwssddddggsswwwwwwwwwsgggswwsggggggssssssswswssssdddddspppsdddddwwsssddddgggssswswsswsggggswsggggggggssssssssssssdddddpssspdddddwwwsssddddggggswswsswwsggggssgggggggggsssgsgssssssddddddddddddddwwwwsssddddgggssgsssswwgggggggggggggggggggggggggggsdddddppppppppwwwwwssdddddggggggggsswwggggggggggggggggggggggggggssdddppspspspswwwwwsssdddddggggggggssgwwgggggggggssgggggssssssssssddppppppppppwwwwwssssddddddggggggggggwwwwwwwwwwwssssssssssssssssgdpsppddddddwwwwwwwsssdddddddggggggggggggggggggwwwsssswwwwsssssgggpppdddddddwwwwwwwwsssddddddddddgsgggsggsgsgssggswwwwwwwsswssggggpspggdddddwwwwwwwwssssdddddddgggsggsgsgsssgsgsgsswwwwwwwwwssggggpppgdddwddwwwwwwwwwssssdddddddggsggsssgsgsgsgsgsswwwwwwwwwssggggpspgddgdddwwwwwwwwwwssddddddddggsggsgsgsgsgsgsgsswwwwwwwwwwsggggpppgddddddwwgwwwwwwwwdddddddddddssgsgsgsgsgssggsswwwwwwwwwsssgggpspgddddddwwgwwwwwssssdssssddddddgggggggggggggsssswwwwwwwwwwsgggpppggdddddwwgwwwwsswsssssssssdddddwwgwwwdgggggssssswwwwwwssssgggpspggdddddwpppwwswwwwwwwwsssssddddddwwwddgggggggssswwwwwwsggggggpppgggddddwpppwwwwwwwwwwwwsssssddddddddddgggggppppppwwwssssgggggpspgggddddwpppwwwwwwwwwwwwwssssssdddddddddgggppppppppssssgggggggpppgggddddwwpwwwwwwwwwwwwwwwwsssssddddddddddppppppppppsgggggggggpspggggdddpwpwpwwgwwgwwwwwwwwwsssssdddddddddppppggpppppppppppppppppgpggdddwpppwwggggggwwwwwwwwwssssssdddddddpppggggpspspspspspspsppggggdddwwpgwggwggwgwwwwwwwwwwwssssssdddddpppggggppppppppppppppppgggddddwwpwgwgggggggwgwwwwwwwwwwssssssdddppppggppppddgggggggggggggdddddwpwpwwggggggwgwwwwwwwwwwwwwsssssssdppppppppdddddddddggggggddddddwpwpwwwgwwgwwwwwwwwwwwwwwwwwssssssssppppppdddddddddddddddddddddd");

	land->alt = hle_malloc(sizeof(float) * LAND_WIDTH * LAND_HEIGHT * 4);

	/* Generate the random altitudes */
	for (i = 0; i < LAND_WIDTH * LAND_HEIGHT * 4; i++) {
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
	float lotsize = 20.0f;

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
		glTranslatef(0.0f, lotsize * i, 0.0f);

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
			
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.5f, 0, 0);
			glVertex3f( 0.0f,  0.0f, land->alt[offset * 4 + 0]);
			glVertex3f( 0.0f, lotsize, land->alt[offset * 4 + 1]);
			glVertex3f(lotsize,  0.0f, land->alt[offset * 4 + 2]);
			glVertex3f(lotsize, lotsize, land->alt[offset * 4 + 3]);
			glEnd();

			glTranslatef(-lotsize, 0.0f, 0.0f);
		}
	}

	glPopMatrix();
}
