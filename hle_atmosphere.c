/*
 * The atmosphere is the big sphere around the player's view. It is halt
 * texture night/star, half textured sky.
 */
#include <math.h>

#include "hlegl.h"
#include "hive.h"

/* XXX */
SDL_Surface *surface = NULL;
GLuint texture;			// This is a handle to our texture object
float rot = 0.0f;

void
hle_atmosphere_load_texture()
{
	float mcolor[4];
	GLUquadricObj *quadratic;
	mcolor[3] = 1.0f;
	GLenum texture_format;
	GLint  nOfColors;
	
	if (surface != NULL) {
		glBindTexture( GL_TEXTURE_2D, texture );
		return;
	}
		

	surface = SDL_LoadBMP("data/sky.bmp");
 
	// Check that the image's width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("warning: image.bmp's width is not a power of 2\n");
	}
	
	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("warning: image.bmp's height is not a power of 2\n");
	}
 
        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
        
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &texture );
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture );
 
	// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
}

/**
 * Using the *data pointer, create all the necessary land pieces.
 */
void
hle_atmosphere_draw(hle_app *app)
{
	float mcolor[4];
	GLUquadricObj *quadratic;
	mcolor[3] = 1.0f;

	rot += 0.01f;
	if (rot > 360.0f)
		rot -= 360.0f;

	glRotatef(app->player->rot, 0, 1.0f, 0);
	glRotatef(270.0f, 1.0f, 0, 0);
	printf("rot=%f\n", rot);
	glRotatef(rot, 0, 0, 1.0f);
	glTranslatef(0, 0, -10.0f);

	hle_atmosphere_load_texture();

	float mcolor2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor2);

	/* Draw the atmosphere */
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	glBindTexture( GL_TEXTURE_2D, texture );
	gluSphere(quadratic, 100.0f, 32, 32);
	glNormal3f(0, 0, 1);

	gluDeleteQuadric(quadratic);
}

