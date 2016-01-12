#ifndef __VECTOR_H__
#define __VECTOR_H__
#define __DEBUG_GL_H__
/**
 * @file
 * Programmlogik und Datenhaltung
 *
 * @author Maurice Tollmien
 */

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdarg.h>

#include "types.h"

/* eig. Funktionen */

/**
 * Konvertierungsfunktion,
 * wandelt drei Koordinaten in den dazugehörigen Vektor um
 * @param x
 * @param y
 * @param z
 */
void toVector3D(CGVector3D vector, GLfloat x, GLfloat y, GLfloat z);
/**
 * Berechnet die Länge es Vektors
 * @param v
 *@return float
 */
float vectorLength3D(CGVector3D vector);

/**
 * Normiert eine Vektor
 * @param v der zu normierende Vektor
 * @return der normierte Vektor
 */
void normVector3D(CGVector3D vector);

/**
 * Berechnet das Kreuzprodukt zweier Vektoren
 * @param
 * @param
 * @return
 */
void crossProduct3D(CGVector3D product, CGVector3D a, CGVector3D b);

double multiplyVectorVector (CGVector3D a, CGVector3D b);
void multiplyVectorScalar (CGVector3D a, double s, CGVector3D * res);
void subtractVectorVector (CGVector3D a, CGVector3D b, CGVector3D * res);
void divideVectorScalar (CGVector3D a, double s, CGVector3D * res);
void addVectorVector (CGVector3D a, CGVector3D b, CGVector3D * res);
void printVector (CGVector3D a);
double angleVectorVector (CGVector3D a, CGVector3D b);
double radToDeg (double rad);
double degToRad (double deg);
double scalarProduct (CGVector3D a, CGVector3D b);

#endif
