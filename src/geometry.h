#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <math.h>
#include <limits.h> 
#include <stdlib.h>
#include <stdio.h>

/**
 * Définition des types
 */
typedef float ** matrixf_t;

typedef struct vec2f		{ float x; float y;			} vec2f_t;
typedef struct vec3f		{ float x; float y; float z;		} vec3f_t;
typedef struct vec2i		{ int x; int y;				} vec2i_t;
typedef struct vec3i		{ int x; int y; int z;			} vec3i_t;
typedef struct face		{ int v[ 3 ]; int vt[ 3 ]; int vn[ 3 ];	} face_t;

/**
 * Définition des macros
 */

/**
 * Macro pour le calcul du min et du max
 */

#define MIN( X, Y) ( X < Y ) ?  X : Y 
#define MAX( X, Y) ( X >  Y ) ?  X : Y 

/**
 * Macro d'affichage d'un matrice de dimension m x n
 */

#define DBG_PRINT_MATRIX( mtx, m, n ) \
    for ( int i = 0; i < m; i++) { \
        for ( int j = 0; j < n ; j++) { \
            printf( "%f ", mtx[ i ][ j ] ); \
        } \
		printf( "\n" ); \
    } \

/**
 * Définition des prototypes de fonctions et implémentation des fonctions inline
 */

/**
 * Construit une matricee flottante de dimension n x m
 */
matrixf_t	Matrixf		( int n, int m );

/**
 * Supprime une matrice flottante de dimension n
 */
void		MatrixfDelete	( matrixf_t m, int n );

/**
 * Construit une matrice identité flottante de dimension n x n
 */
matrixf_t	MatrixfIdentity	( int n );

/**
 * Multiply deux matrices flottante de dimension n x m
 */
matrixf_t	MatrixfMult	( matrixf_t a, matrixf_t b, int n, int m );

/**
 * Construit une matrice viewport flottante
 */
matrixf_t	MatrixfViewport	( int x, int y, int w, int h );

/**
 * Construit une matrice lookat flottante
 */
matrixf_t	MatrixfLookAt	( vec3f_t eye, vec3f_t center, vec3f_t up );

/**
 * Echange deux entiers entre eux
 */
inline void swap( int * a, int * b ) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Construit un vecteur flottant de dimension 3 et positonne ses composantes
 */
inline vec3f_t Vec3f( float x, float y, float z ) {
	vec3f_t v; v.x = x; v.y = y; v.z = z;
	return v;
}
inline vec3i_t Vec3i( int x, int y, int z ) {
	vec3i_t v; v.x = x; v.y = y; v.z = z;
	return v;
}

/**
 * Positonne les composante d'un vecteur flottant de dimension 3
 */
inline void Vec3fSet( vec3f_t * v, float x, float y, float z ) {
	v->x = x; v->y = y; v->z = z;
}


/**
 * Soustrait deux vecteurs flottant de dimension 3
 */
inline vec3f_t Vec3fSub( vec3f_t a, vec3f_t b ) {
	vec3f_t r;
	r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z;
	return r;
}
inline vec3i_t Vec3iSub( vec3i_t a, vec3i_t b ) {
	vec3i_t r;
	r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z;
	return r;
}

/**
 * Calcul la longueur d'un vecteur flottant de dimension 3
 */
inline float Vec3fLength( vec3f_t v ) {
	return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
}

/**
 * Normalise un vecteur flottant de dimension 3
 */
inline vec3f_t Vec3fNormalize( vec3f_t v ) {
	vec3f_t r;
	float l = Vec3fLength( v );
	r.x = v.x / l; r.y = v.y / l; r.z = v.z / l;
	return r;
}

/**
 * Effectue le produit vectoriel entre deux vecteurs flottants de dimension 3
 */
inline vec3f_t Vec3fCross( vec3f_t v1, vec3f_t v2 ) {
	vec3f_t r;
	r.x = v1.y * v2.z - v1.z * v2.y; r.y = v1.z * v2.x - v1.x * v2.z; r.z = v1.x * v2.y - v1.y * v2.x;
	return r;
}

inline vec3i_t Vec3iCross( vec3i_t v1, vec3i_t v2 ) {
	vec3i_t r;
	r.x = v1.y * v2.z - v1.z * v2.y; r.y = v1.z * v2.x - v1.x * v2.z; r.z = v1.x * v2.y - v1.y * v2.x;
	return r;
}
inline int Vec3iScal( vec3i_t v1, vec3i_t v2 ) {
	int r;
	r = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
	return r;
}

inline float Vec3fScal( vec3f_t v1, vec3f_t v2 ) {
	float r;
	r = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
	return r;
}

/**
 * Effectue le barycentre de 3 points
 */
inline vec3f_t Barycentre( vec3f_t A, vec3f_t B, vec3f_t C, vec3f_t D) {


	vec3f_t AB = Vec3fSub(B,A);
	vec3f_t AC = Vec3fSub(C,A);
	vec3f_t DA = Vec3fSub(A,D);

	vec3f_t v1=Vec3f(AB.x,AC.x,DA.x);
	vec3f_t v2=Vec3f(AB.y,AC.y,DA.y);

	vec3f_t cr= Vec3fCross(v1,v2);
	//cr=Vec3fNormalize(cr);
	//printf("%f   %f\n", cr.z, cr.y );
	if (cr.z == 0) return Vec3f(-1.0,1.0,1.0);

	return Vec3f(1.0f-(cr.x+cr.y)/cr.z , cr.y/cr.z , cr.x/cr.z);
}
inline vec3f_t Barycentre2( vec3i_t A, vec3i_t B, vec3i_t C, vec3i_t D) {


	vec3i_t AB = Vec3iSub(B,A);
	vec3i_t AC = Vec3iSub(C,A);
	vec3i_t DA = Vec3iSub(A,D);

	vec3f_t v1=Vec3f(AB.x,AC.x,DA.x);
	vec3f_t v2=Vec3f(AB.y,AC.y,DA.y);

	vec3f_t cr= Vec3fCross(v2,v1);
	//cr=Vec3fNormalize(cr);
	//printf("%f   %f\n", cr.z, cr.y );
	if (cr.z == 0) return Vec3f(-1.0,1.0,1.0);

	return Vec3f(1.0f-(cr.x+cr.y)/cr.z , cr.y/cr.z , cr.x/cr.z);
}
inline vec3f_t Barycentre3( vec3f_t A, vec3f_t B, vec3f_t C, vec3f_t D) {

/*
	vec3f_t AB = Vec3f(Vec3iSub(B,A).x,Vec3iSub(B,A).y,Vec3iSub(B,A).z);
	vec3f_t AC = Vec3f(Vec3iSub(C,A).x,Vec3iSub(C,A).y,Vec3iSub(C,A).z);
	vec3f_t DA = Vec3f(Vec3iSub(A,D).x,Vec3iSub(A,D).y,Vec3iSub(A,D).z);
	vec3f_t DB = Vec3f(Vec3iSub(B,D).x,Vec3iSub(B,D).y,Vec3iSub(B,D).z);
	vec3f_t DC = Vec3f(Vec3iSub(C,D).x,Vec3iSub(C,D).y,Vec3iSub(C,D).z);

     

	vec3f_t n= Vec3fCross(AB,AC);
	n = Vec3fNormalize(n);

	float ABC=Vec3fScal(n,Vec3fCross(AB,AC));
	float DBC=Vec3fScal(n,Vec3fCross(DB,DC));
	float DCA=Vec3fScal(n,Vec3fCross(DC,DA));
	//printf("%f   %f\n", cr.z, cr.y );
	

	return Vec3f(DBC/ABC , DCA/ABC , 1 - (DCA+DBC)/ABC );
	*/
	vec3f_t AB1=Vec3fSub(B,A);
	vec3f_t AC1=Vec3fSub(C,A);
	vec3f_t AD1=Vec3fSub(D,A);
	//vec3f_t AB = Vec3f(AB1.x,AB1.y,AB1.z);
	//vec3f_t AC = Vec3f(AC1.x,AC1.y,AC1.z);
	//fvec3f_t AD = Vec3f(AD1.x,AD1.y,AD1.z);

	float d00 = Vec3fScal(AB1, AB1);
    float d01 = Vec3fScal(AB1, AC1);
    float d11 = Vec3fScal(AC1, AC1);
    float d20 = Vec3fScal(AD1, AB1);
    float d21 = Vec3fScal(AD1, AC1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;

    return Vec3f(1-v-w,v,w);
}
/**
 * Echange le contenu de deux vecteurs flottants de dimension 3
 */
inline void Vec3fSwap( vec3f_t * v1, vec3f_t * v2 ) {
	vec3f_t v = *v1;
	*v1 = *v2;
	*v2 = v;
}

/**
 * Echange le contenu de deux vecteurs flottants de dimension 2
 */
inline void Vec2fSwap( vec2f_t * v1, vec2f_t * v2 ) {
	vec2f_t v = *v1;
	*v1 = *v2;
	*v2 = v;

}

/**
 * Echange le contenu de deux vecteurs entiers de dimension 2
 */
inline void Vec3iSwap( vec3i_t * v1, vec3i_t * v2 ) {
	vec3i_t v = *v1;
	*v1 = *v2;
	*v2 = v;
}

/**
 * Convertit une matrice flottante 4x1 en vecteur flottant de dimension 3
 */
inline vec3f_t Matrixf2Vec3f( matrixf_t m ) {
	vec3f_t v;
	v.x = m[ 0 ][ 0 ] / m[ 3 ][ 0 ]; v.y = m[ 1 ][ 0 ] / m[ 3 ][ 0 ]; v.z = m[ 2 ][ 0 ] / m[ 3 ][ 0 ];
	return v;
}

/**
 * Convertit un vecteur flottant de dimension 3 en une matrice flottante de dimension 4x1
 */
inline matrixf_t Vec3f2Matrixf( vec3f_t v ) {
	matrixf_t m = Matrixf( 4, 1 );
	m[ 0 ][ 0 ] = v.x; m[ 1 ][ 0 ] = v.y; m[ 2 ][ 0 ] = v.z; m[ 3 ][ 0 ] = 1.0f;
	return m;
}

#endif //__GEOMETRY_H__
