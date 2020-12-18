#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <float.h>
#include <time.h> 
#include <unistd.h>

int main( int argc, char ** argv ) 

{

	
	const int width		= 1024;
	const int height	= 768;
	
	//int r,g,b,alpha;
	//unsigned char* pixelOffset;
	//vec3i_t tab_text_coord;
	/*	vec3i_t t0={300, 150 ,0};
	 	vec3i_t t1={100, 300 ,0};
	 	vec3i_t t2={400, 200 ,0};

     	vec3i_t D={200, 60 ,0};
	
		vec3f_t bary=Barycentre3(t0,t1,t2,D);
		printf("%f  %f  %f\n",bary.x,bary.y,bary.z);
		vec3i_t M = Vec3i(bary.x*t0.x+bary.y*t1.x+bary.z*t2.x , bary.x*t0.y+bary.y*t1.y+bary.z*t2.y, 0);
		printf("Mx = %d My= %d\n", M.x, M.y);
	*/
	char fichier[12];
	char fichier_txt[25];
	if (argc<2)
	{
		printf("Erreur: ./bin/eirb3d [nom_du_fichier]\n");
		return 0;
	}
	sprintf(fichier,"./bin/data/%s.obj",argv[1]);
	
	ModelLoad(fichier);
	vector_t * f = ModelFaces();
	vector_t * Vertex = ModelVertices();
	vector_t * texture = ModelTexcoords();

	vec3f_t direction = {0,0,2};
	direction=Vec3fNormalize(direction);
	vec3f_t cot1,cot2;

	float z_buffer[width*height];
    for (int i=0; i<width*height; i++) {
        z_buffer[i] = -FLT_MAX;
    }
    //printf("zbuff= %f\n",z_buffer[0] );

    //Textures
    int tga_width,tga_height,tga_comp;
	sprintf(fichier_txt,"./bin/data/%s_diffuse.tga",argv[1]);

    unsigned char *data_txt=stbi_load(fichier_txt,&tga_width,&tga_height,&tga_comp,STBI_rgb_alpha);
    printf("width = %d height= %d  lentgh= %d comp= %#.1X\n",tga_width,tga_height,sizeof(*data_txt)/sizeof(char),tga_comp );
	//vector_t * f = ModelFaces();
	
	//printf("%d\n", g->count);
	//printf("%d\n", g->size);
	//void *xx = g->data[0];

	//float f = *(float*)VectorGetFromIdx( ModelVertices(), 0 );
	
	//printf("%f\n", f);

	
	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );

	int done = false;
	//WindowDrawClearColor( mainwindow,0,0,0);
	WindowDrawClearColor( mainwindow,255,255,255);
	// Tant que l'utilisateur de ferme pas la fenêtre
	while ( !done ) {	
		
		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );
		int i = 0;
		

		// vec3f_t normale = Vec3fCross(t2,t1);
		// normale=Vec3fNormalize(normale);

		// float intensity = Vec3fScal(normale,direction);
		// printf("%f\n",intensity );
		//WindowDrawTriangle(mainwindow,screen[0],screen[1],screen[2],255,255,255);
		//WindowDrawPoint( mainwindow, width / 2, height / 2, 255, 0, 0 );

		while(i<f->count)
		{
			int *ix;
			float *x,*txt;
			//Notez bien que dans certaines fonctions, screen est de vec3f_t. Alors, il faut le changer d'ici
			vec3i_t * screen = (vec3i_t *)malloc(sizeof(vec3i_t)*3);
			vec3f_t * real_coord = (vec3f_t *)malloc(sizeof(vec3f_t)*3);
			vec3f_t * text_coord = (vec3f_t *)malloc(sizeof(vec3f_t)*3);
				
			ix=(int*)VectorGetFromIdx( f, i);

			for (int j = 0; j < 3; ++j)
			{
				
				x = (float*)VectorGetFromIdx( Vertex, ix[j*3]-1);
				txt = (float*)VectorGetFromIdx( texture, ix[j*3+1]-1);
				screen[j].x=(x[0]+1)*(mainwindow->width)/2;
				real_coord[j].x=(x[0]+1)*(mainwindow->width)/2;
				text_coord[j].x=txt[0];

				screen[j].y=(x[1]+1)*(mainwindow->height)/2;
				real_coord[j].y=(x[1]+1)*(mainwindow->height)/2;
				text_coord[j].y=txt[1];

				screen[j].z=x[2];
				real_coord[j].z=x[2];
				text_coord[j].z=txt[2];
			}
			//Texture
			//Interpolation des coordonnées de texture dans le triangle

         	//tab_text_coord.x = (text_coord[0].x + text_coord[1].x + text_coord[2].x)*tga_height/3; 
        	//tab_text_coord.y = (text_coord[0].y + text_coord[1].y + text_coord[2].y)*tga_width/3;
        	
        	//pixelOffset = data_txt + (tab_text_coord.x+tab_text_coord.y*tga_width)* 4;
			//r = pixelOffset[0];
			//g = pixelOffset[1];
			//b = pixelOffset[2];
			//alpha= pixelOffset[3];

			//Source de la lumière
			cot1 = Vec3fSub(real_coord[1],real_coord[0]);
			cot2 = Vec3fSub(real_coord[2],real_coord[0]);
			vec3f_t normale = Vec3fCross(cot1,cot2);
			normale=Vec3fNormalize(normale);

			float intensity = Vec3fScal(direction,normale);
			
			if (intensity > 0) 
			{
				
				//WindowDrawTriangle(mainwindow,screen[0],screen[1],screen[2],rand()%255,rand()%255,rand()%255);
				//WindowDrawTriangle_prof(mainwindow,z_buffer,screen[0],screen[1],screen[2],intensity*255,intensity*255,intensity*255);
				//WindowDrawTriangle_zbuffer(mainwindow,z_buffer,screen[0],screen[1],screen[2],intensity*255,intensity*255,intensity*255,0xFF);
				WindowDrawTriangle_zbuffer_text(mainwindow,z_buffer, screen, real_coord, text_coord, data_txt, tga_width, tga_height, intensity);
				//WindowDrawTriangle_zbuffer_texture(mainwindow,z_buffer,screen[0],screen[1],screen[2],data_txt,tga_width,tga_height,text_coord,intensity);
				//WindowUpdate( mainwindow );
				//usleep( 100 );
			}

			i=i+1;

		}

		
		//WindowDrawPoint( mainwindow, width / 2, height / 2, 0, 255, 0 );
		//WindowDrawPoint( mainwindow, width / 2 +300 , height / 2 , 0, 255, 0 );

		//WindowDrawLine(mainwindow, 0 , 0 , width / 2, height / 2  , 0, 0, 255);
		//WindowDrawLine(mainwindow, 0 , height / 2 , width / 2, height / 2  , 0, 0, 255);
		//WindowDrawLine(mainwindow, 0 , height / 2 +100, width / 2, height / 2  , 0, 0, 255);
		//WindowDrawLine(mainwindow,width / 2 +300 , height / 2 -100, width / 2, height / 2  , 0, 0, 255);
		//WindowDrawTriangle(mainwindow,t0,t1,t2,rand()%255,rand()%255,rand()%255);
		
		//WindowDrawTriangle(mainwindow,t0,t1,t2,rand()%255,rand()%255,rand()%255);
		
		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );

	}
	
	// Fermeture de la fenêtre
	//WindowDestroy( mainwindow );

	return 1;
}

/*
//Y_BUFFER
{
	const int width		= 1024;
	const int height	= 768;
	vec3i_t t0={0, 200 , 0};
	vec3i_t t1={200, 20 , 10};
	vec3i_t t2={0, 200 , 0};
	vec3i_t t3={width/2, height/2 , 10};
	vec3i_t t4={100, 300 , 0};
	vec3i_t t5={100, 50 , 10};

	int y_buffer[width];

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );

	int done = false;
	WindowDrawClearColor( mainwindow,255,255,255);
	WindowDrawLine(mainwindow, t0.x , t0.y ,t1.x, t1.y  , 255, 0, 0);
	WindowDrawLine(mainwindow, t2.x , t2.y ,t3.x, t3.y  , 0, 255, 0);
	WindowDrawLine(mainwindow, t4.x , t4.y ,t5.x, t5.y  , 0, 0, 255);
	// Tant que l'utilisateur de ferme pas la fenêtre
	while ( !done ) {	
		
		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );	

	
		
	    for (int i=0; i<width; i++) {
	        y_buffer[i] = INT_MIN;
	    }
	    if (t1.x>t0.x) Vec3iSwap(&t0,&t1);
	    for (int x=t1.x; x<=t0.x; x++) {
	    	int y = t0.y+ (x-t0.x) * (t0.y-t1.y)/(float)(t0.x-t1.x); //t0.x-t1.x != 0
	    	if (y_buffer[x]<y) {
	        	y_buffer[x] = y;
	        	//printf("%d\n",y_buffer[x] );
	        	WindowDrawPoint( mainwindow,x,10,255,0,0);
	    	}
	    }
	    
		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );

	}
	return 1;
}

*/

