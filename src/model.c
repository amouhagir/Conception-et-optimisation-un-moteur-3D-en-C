#include "model.h"

vector_t * g_vertex;
vector_t * g_norm;
vector_t * g_texcoord;
vector_t * g_face;

vector_t * ModelVertices() {
	return g_vertex;
}

vector_t * ModelNormals() {
	return g_norm;
}

vector_t * ModelTexcoords() {
	return g_texcoord;
}

vector_t * ModelFaces() {
	return g_face;
}

vec3f_t ModelGetVertex( int index ) {
	vec3f_t v = *(vec3f_t*)VectorGetFromIdx( ModelVertices(), index );
	return v;
}

vec3f_t ModelGetNormal( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelNormals(), index );
	return t;
}

vec3f_t ModelGetTexcoord( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelTexcoords(), index );
	return t;
}

face_t ModelGetFace( int index ) {
	face_t f = *(face_t*)VectorGetFromIdx( ModelFaces(), index );
	return f;
}

bool ModelLoad( const char * objfilename ) {
	
	FILE* f =fopen(objfilename, "r");
	
	if (f == NULL)
		{
			printf("Impossible d'ouvrir le fichier\n");
			return false;
		}

	char c[128];
	char type[3];
	
	
	g_vertex=Vector();
	g_norm=Vector();
	g_texcoord=Vector();
	g_face=Vector();

	while (fgets(c,128,f) != NULL) {

		float * dt = (float*)malloc( sizeof( float ) *3 );
		
		sscanf(c,"%s",type);
		//printf("type: %s\n", type );

    	if ( strcmp( type, "v" ) == 0 ){

    		sscanf(c+2,"%f %f %f\n",dt,dt+1,dt+2);
    		VectorAdd(g_vertex, dt );
    		

    	}else if ( strcmp( type, "vn" ) == 0 ){
    		sscanf(c+3,"%f %f %f\n", dt,dt+1,dt+2);
    		
    		VectorAdd(g_norm, dt );
    	}else if ( strcmp( type, "vt" ) == 0 ){
    		sscanf(c+3,"%f %f\n", dt,dt+1);
    		VectorAdd(g_texcoord,  dt );
    	}else if ( strcmp( type, "f" ) == 0 ){
    		int * face = (int*)malloc( sizeof(int) * 9 );
    		sscanf(c+2, "%d/%d/%d %d/%d/%d %d/%d/%d\n", face,face+1,face+2,face+3,face+4,face+5,face+6,face+7,face+8);
    		VectorAdd(g_face, face);
    	}

	}

	fclose(f);
	return true;
}

