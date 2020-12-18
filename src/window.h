#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "geometry.h"

#include <math.h>
#include "SDL2/SDL.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

/**
 * Définition des types
 */
typedef struct window {
	SDL_Window	*	sdlwindow;
	SDL_Renderer	*	renderer;
	SDL_Texture	*	texture;
	unsigned char	*	framebuffer;
	int			width;
	int			height;
	int			bpp;
	int			pitch;
}window_t;

typedef struct point {
	int x;
	int y;
	int z;
}point_t;	
/**
 * Définition des prototypes de fonctions
 */

/**
 * Initialise et ouvre une nouvelle fenêtre
 */
window_t	*	WindowInit		( int width, int height, int bpp );

/**
 * Ferme et detruit une fênetre
 */
void			WindowDestroy		( window_t * w );

/**
 * Efface une fenêtre avec la couleur souhaîtée
 */
void			WindowDrawClearColor	( window_t * w, unsigned char r, unsigned char g, unsigned char b );

/**
 * Met à jour le contenu de la fenêtre
 */
void			WindowUpdate		( window_t * w );

/**
 * Met à jour le titre de la fenêtre
 */
void 			WindowSetTitle		( window_t * w, const char * title );

/**
 * Dessine un point coloré dans la fenêtre
 */
void			WindowDrawPoint		( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b );

/**
 * Dessine une ligne colorée dans la fenêtre
 */
void			WindowDrawLine		( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b );
/**
 * Dessine un triangle colorée dans la fenêtre
 */

void WindowDrawTriangle( window_t * w, vec3i_t t0, vec3i_t t1, vec3i_t t2, Uint8 r, Uint8 g, Uint8 b );


//Notre preemier essai qui ne traçe pas tous triangles 

void WindowDrawTriangle_prof(window_t * w, float *z_buffer, vec3f_t t0, vec3f_t t1, vec3f_t t2, Uint8 r, Uint8 g, Uint8 b);
void WindowDrawTriangle_zbuffer_texture(window_t * w, float *z_buffer, vec3f_t t0, vec3f_t t1, vec3f_t t2, unsigned char * data_txt, int tga_width, int tga_height, vec3f_t * text_coord , float intensity);

//Deuxième essai pour le z_buffer: Utilisation de l'interpolation linéaire en calculant z depuis le main.c comme la moyenne des z des sommets

void WindowDrawPoint_zbuffer( window_t * w, int x, int y,float *z_buffer, float z, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha );
void WindowDrawLine_zbuffer( window_t * w, int x0, int y0, int x1, int y1,float *z_buffer, float z, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha );
void WindowDrawTriangle_zbuffer(window_t * w, float *z_buffer, vec3i_t t0, vec3i_t t1, vec3i_t t2, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);

//Troisième essai pour le z_buffer et texture: Utilisation de l'interpolation barycentrique

void WindowDrawPoint_zbuffer_text( window_t * w, int x, int y,vec3i_t *screen_coord,vec3f_t *real_coord, float *z_buffer, float z, vec3f_t *text_coord, unsigned char *data_txt,int tga_width, int tga_height, float intensity);
void WindowDrawLine_zbuffer_text( window_t * w, int x0, int y0, int x1, int y1, vec3i_t *screen_coord,vec3f_t *real_coord, float *z_buffer, float z, vec3f_t *text_coord, unsigned char *data_txt,int tga_width, int tga_height, float intensity );
void WindowDrawTriangle_zbuffer_text(window_t * w, float *z_buffer, vec3i_t *screen_coord, vec3f_t *real_coord, vec3f_t *text_coord, unsigned char *data_txt,int tga_width, int tga_height, float intensity);



#endif //__WINDOW_H__
