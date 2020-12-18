#include "window.h"


static void WindowUpdateTexture( window_t * w ) {
	Uint32 * dst;
	int row, col;
	void * pixels;
	int pitch;
	if ( SDL_LockTexture( w->texture, NULL, &pixels, &pitch ) < 0 ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError() );
		SDL_Quit();
	}
	Uint8 * ptr = w->framebuffer;
	for ( row = 0; row < w->height; ++row ) {
		dst = (Uint32*)( (Uint8*)pixels + row * pitch );
		for ( col = 0; col < w->width; ++col ) {
			Uint8 b = *ptr++;
			Uint8 g = *ptr++;
			Uint8 r = *ptr++;
			Uint8 a = *ptr++;
			*dst++ = ( ( b << 24 ) | ( g << 16 ) | ( r << 8 ) | a );
		}
	}
	SDL_UnlockTexture( w->texture );
}

static Uint8 * WindowInitFramebuffer( window_t * w ) {
	size_t sz = w->width * w->height * w->bpp * sizeof( Uint8 );
	Uint8 * buffer = (Uint8*)malloc( sz );
	if ( buffer == NULL) return NULL;
	memset( buffer, 0, sz );
	return buffer;
}

window_t * WindowInit( int width, int height, int bpp ) {

	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		printf( "SDL_Init Error: %s", SDL_GetError() );
		return NULL;
	}

	SDL_Window * sdlwindow = SDL_CreateWindow(	"Software OpenGL renderer", 
												SDL_WINDOWPOS_CENTERED, 
												SDL_WINDOWPOS_CENTERED, 
												width, height,
												SDL_WINDOW_ALLOW_HIGHDPI );

	if ( sdlwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL window: %s", SDL_GetError() );
		SDL_DestroyWindow( sdlwindow );
		SDL_Quit();
		return NULL;
	}

	SDL_Renderer * renderer = SDL_CreateRenderer( sdlwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	if ( renderer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError() );
		SDL_DestroyRenderer( renderer );
		SDL_Quit();
		return NULL;
	}

	SDL_Texture * texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, width, height );
	
	if ( texture == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError() );
		SDL_DestroyTexture( texture );
		SDL_Quit();
		return NULL;
	}

	window_t * mainwindow = (window_t*)malloc( sizeof( window_t ) );

	if ( mainwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate window\n" );
		SDL_Quit();
		return NULL;
	}

	mainwindow->width	= width;
	mainwindow->height	= height;
	mainwindow->bpp		= bpp;
	mainwindow->pitch	= width * bpp;

	Uint8 * framebuffer = WindowInitFramebuffer( mainwindow );

	if ( framebuffer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate framebuffer: %s\n", SDL_GetError() );
		SDL_Quit();
		return NULL;
	}

	mainwindow->framebuffer = framebuffer;
	mainwindow->sdlwindow	= sdlwindow;
	mainwindow->renderer	= renderer;
	mainwindow->texture		= texture;

	return mainwindow;
}

void WindowDestroy( window_t * w ) {
	SDL_DestroyRenderer( w->renderer );
	SDL_DestroyTexture( w->texture );
	SDL_DestroyWindow( w->sdlwindow );
	free( w->framebuffer );
	SDL_Quit();
}

void WindowUpdate( window_t * w ) {
	SDL_RenderClear( w->renderer );
	SDL_RenderCopy( w->renderer, w->texture, NULL, NULL );
	SDL_RenderPresent( w->renderer );
	WindowUpdateTexture( w );
}

void WindowSetTitle( window_t * w, const char * title ) {
	SDL_SetWindowTitle( w->sdlwindow, title );
}
						
void WindowDrawPoint( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b ) {
	
	Uint32 * ptr =(Uint32 *) w->framebuffer;

	ptr[(w->width*(w->height-y))+w->width-x]= ( ( 0xFF << 24 ) | ( r << 16 ) | ( g << 8 ) | b );
	//SDL_RenderDrawPoint(w->renderer,x,y);

}

void WindowDrawClearColor( window_t * w, Uint8 r, Uint8 g, Uint8 b ) {
	
	for (int row = 0; row < w->height; ++row)
	{
		for (int col = 0; col < w->width; ++col)
		{
			WindowDrawPoint(w,col,row,r,g,b);
		}
	}
}

void WindowDrawLine( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b ) 

////Program for Line Drawing (Direct method)
/*{ 
	
	int i,j;
	float mX,mY;
	int x,y;

	float m;
	
	mX=x0-x1;
	mY=y0-y1;
	//y=y1-x1*m+m*x

	if (mX != 0) {

		m=mY/mX;
		if (m<=1)
		{
			if (mX<0)
			{
				for (i=x0;i<=x1;++i)
				{
					y= (int)(y1+m*(i-x1));
					WindowDrawPoint(w,i,y,r,g,b);
				}				
			}else{
				for (i=x1;i<=x0;++i)
				{
					y= (int)(y1+m*(i-x1));
					WindowDrawPoint(w,i,y,r,g,b);
				}
			}

		}
		else 
		{
			if (mY<0)
			{
				for (j=y0;j<=y1;++j)
				{
					x= (int) x1+(j-y1)/m;
					WindowDrawPoint(w,x,j,r,g,b);
				}
			}else{

				for (j=y1;j<=y0;++j)
				{
					x=(int) (x1+(j-y1)/m);
					WindowDrawPoint(w,x,j,r,g,b);
				}
			}

		}
	}
	else
	{
		if (mY<0)
		{
			for (j=y0;j<=y1; ++j)
			{
				WindowDrawPoint(w,x0,j,r,g,b);
			}
		}
		else
		{
			for (j=y1;j<=y0; ++j)
			{
				WindowDrawPoint(w,x0,j,r,g,b);
			}
		}
	}
	
}*/
  
//Program for Bresenham’s Line Drawing Algorithm in C
{
  int dx =  abs (x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0);
  int sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy;
  int e2; 
 
  for (;;){  
    WindowDrawPoint(w,x0,y0,r,g,b);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } 
    if (e2 <= dx) { err += dx; y0 += sy; } 
  }
}

void WindowDrawTriangle(window_t * w, vec3i_t t0, vec3i_t t1, vec3i_t t2, Uint8 r, Uint8 g, Uint8 b) 

{ 
	
	

	if (t1.y>t0.y) Vec3iSwap(&t0,&t1);
    if (t2.y>t0.y) Vec3iSwap(&t0,&t2);	
    if (t2.y>t1.y) Vec3iSwap(&t2,&t1);

    
    int triangle_height = t0.y-t2.y;
    int segment_height_t0_t1 = t0.y -t1.y; 
    int segment_height_t1_t2 = t1.y -t2.y;
    vec3i_t A,B;
    float n,m;
    //les coordonnées de la ligne séparative
    //vec3i_t d={0,t1.y,0};
    //d.x=t2.x+ (t1.y-t2.y)* (t0.x-t2.x)/triangle_height;
   
    
    for (int i = t1.y; i <= t0.y; ++i)
    {
    	n = triangle_height !=0 ? (float)(i-t2.y)/triangle_height : 0; 
        m = segment_height_t0_t1 !=0 ? (float)(i-t1.y)/segment_height_t0_t1 : 0;

        A.x= t2.x + (t0.x-t2.x)*n;  
        B.x= t1.x + (t0.x-t1.x)*m; 
        WindowDrawLine(w,A.x,i,B.x,i,r,g,b);
    }
    for (int i = t2.y; i <= t1.y; ++i)
    {
    	n = triangle_height !=0 ? (float)(i-t2.y)/triangle_height : 0; 
        m = segment_height_t1_t2 !=0 ? (float)(i-t2.y)/segment_height_t1_t2 : 0;

        A.x= t2.x + (t0.x-t2.x)*n;  
        B.x= t2.x + (t1.x-t2.x)*m; 
        WindowDrawLine(w,A.x,i,B.x,i,r,g,b);
    }

    
    //Pour l'affichage en fil de fer, on commente les deux boucles for ci-dessus puis on decommente les trois lignes suivantes 

    //WindowDrawLine(w,t0.x,t0.y,t1.x,t1.y,0,255,0);
    //WindowDrawLine(w,t1.x,t1.y,t2.x,t2.y,0,255,0);
    //WindowDrawLine(w,t2.x,t2.y,t0.x,t0.y,255,0,0);
    //WindowDrawLine(w,t1.x,t1.y,d.x,d.y,0,0,0);
}


//Methode de barycentre et box qui contoure le triangle
void WindowDrawTriangle_prof(window_t * w, float *z_buffer, vec3f_t t0, vec3f_t t1, vec3f_t t2, Uint8 r, Uint8 g, Uint8 b)
{
	
	vec3f_t pt0,pt1;
	vec3f_t barcenter; 
	int indx;
	int x,y;

    pt0.x=MIN(MIN(t0.x,t1.x),t2.x); pt1.x=MAX(MAX(t0.x,t1.x),t2.x);
    pt0.y=MIN(MIN(t0.y,t1.y),t2.y); pt1.y=MAX(MAX(t0.y,t1.y),t2.y);

 
    vec3f_t D;
    
	for (x = pt0.x; x <= pt1.x; x++)
	{

		for (y = pt0.y; y <= pt1.y; y++)
		{
			D =Vec3f(x,y,0);
			
			barcenter = Barycentre(t1,t0,t2,D);
			//printf("%f  %f  %f\n", barcenter.x, barcenter.y, barcenter.z);
			if (barcenter.x<0 || barcenter.y<0 || barcenter.z<0) continue; //Le point n'appartient pas au triangle (t0,t1,t2)
			//printf("%f  %f  %f\n", barcenter.x, barcenter.y, barcenter.z);
			barcenter.x = barcenter.x*w->width; barcenter.y = barcenter.y*w->height;
			float z = Vec3fScal(Vec3f(t1.z,t2.z,t0.z),barcenter);

			indx = x+y*w->width;
			
			if (z_buffer[indx] < z) {
	        	z_buffer[indx] = z;
	        	WindowDrawPoint(w,x,y,r,g,b);
	    	}
		}
		
	}
	
}

void WindowDrawTriangle_zbuffer_texture(window_t * w, float *z_buffer, vec3f_t t0, vec3f_t t1, vec3f_t t2, unsigned char * data_txt, int tga_width,int tga_height, vec3f_t * text_coord , float intensity)
{
	
	vec3f_t pt0,pt1;
	vec3i_t tab_text_coord;
	int indx,txt_indx;
	int x,y;
	int r,g,b;
	unsigned char* pixelOffset;
	
    pt0.x=MIN(MIN(t0.x,t1.x),t2.x); pt1.x=MAX(MAX(t0.x,t1.x),t2.x);
    pt0.y=MIN(MIN(t0.y,t1.y),t2.y); pt1.y=MAX(MAX(t0.y,t1.y),t2.y);

    vec3f_t D;
    
	for (x = pt0.x; x <= pt1.x; x++)
	{

		for (y = pt0.y; y <= pt1.y; y++)
		{
			D =Vec3f(x,y,0);
			
			vec3f_t barcenter = Barycentre3(t0,t1,t2,D);
			if (barcenter.x < 0 || barcenter.y < 0 || barcenter.z < 0) continue; //Le point n'appartient pas au triangle (t0,t1,t2)
			

			float z = Vec3fScal(Vec3f(t0.z,t2.z,t1.z),barcenter);
			
			indx = x+y*w->width;
			if (z_buffer[indx] < z) {
	        	z_buffer[indx] = z;
	        	//Interpolation des coordonnées de textures dans le triangle 
	        	tab_text_coord = Vec3i( Vec3fScal(Vec3f(text_coord[0].x,text_coord[1].x,text_coord[2].x),barcenter)*tga_height, Vec3fScal(Vec3f(text_coord[0].y,text_coord[1].y,text_coord[2].y),barcenter)*tga_width, 0);
	        	txt_indx = (tga_width-tab_text_coord.x)+(tga_height-tab_text_coord.y)*tga_width;
	        	pixelOffset = data_txt + txt_indx * 4;
 				r = pixelOffset[0];
 				g = pixelOffset[1];
  				b = pixelOffset[2];
  				//printf("%#.2X  %#.2X  %#.2X \n", r,g,b);
	        	WindowDrawPoint(w,x,y,r*intensity,g*intensity,b*intensity);
	    	}
		}
		
	}
	
}


//Deuxième essai pour le z_buffer: Utilisation de l'interpolation linéaire en calculant z depuis le main.c comme la moyenne des z des sommets

void WindowDrawPoint_zbuffer( window_t * w, int x, int y,float *z_buffer, float z, Uint8 r, Uint8 g, Uint8 b , Uint8 alpha ) {
	
	Uint32 * ptr =(Uint32 *) w->framebuffer;

	if (z_buffer[x+y*w->width] < z) {
    	z_buffer[x+y*w->width] = z;
		ptr[(w->width*(w->height-y))+w->width-x]= ( ( alpha << 24 ) | ( r << 16 ) | ( g << 8 ) | b );
	}

}
void WindowDrawLine_zbuffer( window_t * w, int x0, int y0, int x1, int y1,float *z_buffer, float z, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha )
{
  int dx =  abs (x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0);
  int sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy;
  int e2; 
 
  for (;;){  
    WindowDrawPoint_zbuffer(w,x0,y0,z_buffer,z,r,g,b,alpha);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } 
    if (e2 <= dx) { err += dx; y0 += sy; } 
  }
}

void WindowDrawTriangle_zbuffer(window_t * w, float *z_buffer, vec3i_t t0, vec3i_t t1, vec3i_t t2, Uint8 r, Uint8 g, Uint8 b, Uint8 alpha)
{
	if (t1.y>t0.y) Vec3iSwap(&t0,&t1);
    if (t2.y>t0.y) Vec3iSwap(&t0,&t2);	
    if (t2.y>t1.y) Vec3iSwap(&t2,&t1);

    
    int triangle_height = t0.y-t2.y;
    int segment_height_t0_t1 = t0.y - t1.y; 
    int segment_height_t1_t2 = t1.y - t2.y;
    vec3i_t A,B;
    float n,m;
    float z = (t0.z+t1.z+t2.z)/3/1000; //comme on a multiplie par 1000 lors de récupération des coordonnées 
    
    for (int i = t1.y; i <= t0.y; ++i)
    {
    	n = triangle_height !=0 ? (float)(i-t2.y)/triangle_height : 0; 
        m = segment_height_t0_t1 !=0 ? (float)(i-t1.y)/segment_height_t0_t1 : 0;

        A.x= t2.x + (t0.x-t2.x)*n;  
        B.x= t1.x + (t0.x-t1.x)*m; 
        WindowDrawLine_zbuffer(w,A.x,i,B.x,i,z_buffer,z,r,g,b,alpha);
    }
    for (int i = t2.y; i <= t1.y; ++i)
    {
    	n = triangle_height !=0 ? (float)(i-t2.y)/triangle_height : 0; 
        m = segment_height_t1_t2 !=0 ? (float)(i-t2.y)/segment_height_t1_t2 : 0;

        A.x= t2.x + (t0.x-t2.x)*n;  
        B.x= t2.x + (t1.x-t2.x)*m; 
        WindowDrawLine_zbuffer(w,A.x,i,B.x,i,z_buffer,z,r,g,b,alpha);
    }  
}


//Troisième essai pour le z_buffer et texture: Utilisation de l'interpolation barycentrique

void WindowDrawPoint_zbuffer_text( window_t * w, int x, int y,vec3i_t *screen_coord, vec3f_t *real_coord, float *z_buffer, float z, vec3f_t *text_coord, unsigned char *data_txt,int tga_width, int tga_height, float intensity) {
	
	Uint32 * ptr =(Uint32 *) w->framebuffer;
	int r,g,b,alpha;
	float z1;
	unsigned char* pixelOffset;
	vec3i_t tab_text_coord;
	vec3f_t barcenter;
	//printf("x=%d y=%d\n",x,y );
	//printf("%d   %d   %d\n", screen_coord[0].y,screen_coord[1].y,screen_coord[2].y);
	barcenter = Barycentre3(real_coord[0],real_coord[1],real_coord[2],Vec3f(x,y,0));
	//printf("bar= %f  %f  %f\n",barcenter.x,barcenter.y,barcenter.z );
	//if (barcenter.x>=0 || barcenter.y>=0 || barcenter.z>=0){
		 z1 =  Vec3fScal(Vec3f(real_coord[0].z,real_coord[1].z,real_coord[2].z),barcenter);
		 //}
	//printf("%f\n",z1 );
	if (z_buffer[x+y*w->width] < z1 ) {
    	z_buffer[x+y*w->width] = z1;
    	
    	//barcenter1 = Barycentre(real_coord[0],real_coord[1],real_coord[2],Vec3f(x,y,0));
    	tab_text_coord = Vec3i( Vec3fScal(Vec3f(text_coord[0].x,text_coord[1].x,text_coord[2].x),barcenter)*tga_width, Vec3fScal(Vec3f(text_coord[0].y,text_coord[1].y,text_coord[2].y),barcenter)*tga_height, 0);

    	//printf("%d\n", (tga_width-tab_text_coord.x+(tga_height-tab_text_coord.y)*tga_width)* 4);
    	
    	pixelOffset = data_txt + (tga_width-tab_text_coord.x+(tga_height-tab_text_coord.y)*tga_width)*4;
		r = pixelOffset[0]*intensity;
		g = pixelOffset[1]*intensity;
		b = pixelOffset[2]*intensity;
		alpha= pixelOffset[3];

		ptr[(w->width*(w->height-y))+w->width-x]= ( ( alpha << 24 ) | ( r << 16 ) | ( g << 8 ) | b );
	}

}
void WindowDrawLine_zbuffer_text( window_t * w, int x0, int y0, int x1, int y1,vec3i_t *screen_coord, vec3f_t *real_coord, float *z_buffer, float z, vec3f_t *text_coord, unsigned char *data_txt,int tga_width, int tga_height, float intensity )
{
  int dx =  abs (x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0);
  int sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy;
  int e2; 
 
  for (;;){  
    WindowDrawPoint_zbuffer_text(w,x0,y0,screen_coord,real_coord, z_buffer,z,text_coord,data_txt,tga_width,tga_height,intensity);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } 
    if (e2 <= dx) { err += dx; y0 += sy; } 
  }
}

void WindowDrawTriangle_zbuffer_text(window_t * w, float *z_buffer,vec3i_t *screen_coord, vec3f_t *real_coord, vec3f_t *text_coord, unsigned char *data_txt,int tga_width, int tga_height, float intensity)
{
	if (screen_coord[1].y>screen_coord[0].y) Vec3iSwap(&screen_coord[0],&screen_coord[1]);
    if (screen_coord[2].y>screen_coord[0].y) Vec3iSwap(&screen_coord[0],&screen_coord[2]);	
    if (screen_coord[2].y>screen_coord[1].y) Vec3iSwap(&screen_coord[2],&screen_coord[1]);

    
    int triangle_height = screen_coord[0].y-screen_coord[2].y;
    int segment_height_t0_t1 = screen_coord[0].y - screen_coord[1].y; 
    int segment_height_t1_t2 = screen_coord[1].y - screen_coord[2].y;
    vec3i_t A,B;
    float n,m;
    float z = (screen_coord[0].z+screen_coord[1].z+screen_coord[2].z)/3/1000; //comme on a multiplie par 1000 lors de récupération des coordonnées 
    
    for (int i = screen_coord[1].y; i <= screen_coord[0].y; ++i)
    {
    	n = triangle_height !=0 ? (float)(i-screen_coord[2].y)/triangle_height : 0; 
        m = segment_height_t0_t1 !=0 ? (float)(i-screen_coord[1].y)/segment_height_t0_t1 : 0;

        A.x= screen_coord[2].x + (screen_coord[0].x-screen_coord[2].x)*n;  
        B.x= screen_coord[1].x + (screen_coord[0].x-screen_coord[1].x)*m; 
        WindowDrawLine_zbuffer_text(w,A.x,i,B.x,i,screen_coord,real_coord, z_buffer,z,text_coord,data_txt,tga_width,tga_height,intensity);
    }
    for (int i = screen_coord[2].y; i <= screen_coord[1].y; ++i)
    {
    	n = triangle_height !=0 ? (float)(i-screen_coord[2].y)/triangle_height : 0; 
        m = segment_height_t1_t2 !=0 ? (float)(i-screen_coord[2].y)/segment_height_t1_t2 : 0;

        A.x= screen_coord[2].x + (screen_coord[0].x-screen_coord[2].x)*n;  
        B.x= screen_coord[2].x + (screen_coord[1].x-screen_coord[2].x)*m; 
        WindowDrawLine_zbuffer_text(w,A.x,i,B.x,i, screen_coord,real_coord,  z_buffer,z,text_coord,data_txt,tga_width,tga_height,intensity);
    }
}









