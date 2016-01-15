/**
 * @file
 * Hier ist die Datenhaltung und Programmlogik
 *
 * @author Maurice Tollmien
 */

#ifdef _WIN32
#include <windows.h>
#endif

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* ---- Eigene Header einbinden ---- */
#include "types.h"


/**
 * Printet einen Vector aus.
 */
void printVector (CGVector3D a)
{
    int i;
    printf("\nprintVector:\n");
    for (i=0;i<3;i++)
        printf("%.1f\n", a[i]);
    printf("\n");
}

/**
 * Konvertierungsfunktion,
 * wandelt drei Koordinaten in den dazugehörigen Vektor um
 * @param x
 * @param y
 * @param z
 */
void toVector3D(CGVector3D vector, GLfloat x, GLfloat y, GLfloat z)
{
  vector[0] = x;
  vector[1] = y;
  vector[2] = z;
}

/**
 * Berechnet die Länge es Vektors
 * @param v
 *@return float
 */
float vectorLength3D(CGVector3D vector)
{
  return sqrt((vector[0]*vector[0])+
              (vector[1]*vector[1])+
              (vector[2]*vector[2]));
}

/**
 * Normiert eine Vektor
 * @param v der zu normierende Vektor
 * @return der normierte Vektor
 */
void normVector3D(CGVector3D vector)
{
  float l = vectorLength3D(vector);
  if (l != .0f)
    toVector3D(vector, vector[0]/l, vector[1]/l, vector[2]/l);
}

/**
 * Berechnet das Kreuzprodukt zweier Vektoren
 * @param
 * @param
 * @return
 */
void crossProduct3D(CGVector3D product, CGVector3D a, CGVector3D b)
{
  toVector3D(product, (a[1]*b[2] - a[2]*b[1]),
                      (a[2]*b[0] - a[0]*b[2]),
                      (a[0]*b[1] - a[1]*b[0]));
}

/**
 * Multipliziert zwei Vektoren miteinander (Skalarprodukt)
 */
double multiplyVectorVector (CGVector3D a, CGVector3D b)
{
    int i;
    double res = 0.0;
    for (i=0;i<3;i++)
        res += a[i]*b[i];
    return res;
}

/**
 * Multipliziert einen Vektor mit einem Skalar.
 */
void multiplyVectorScalar (CGVector3D a, double s, CGVector3D * res)
{
    int i;
    for (i=0;i<3;i++)
        (*res)[i] = a[i]*s;
}

double scalarProduct (CGVector3D a, CGVector3D b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/**
 * Zieht b von a ab, also: a-b
 */
void subtractVectorVector (CGVector3D a, CGVector3D b, CGVector3D * res)
{
    int i;
    for (i=0;i<3;i++)
        (*res)[i] = a[i] - b[i];
}

/**
 * Teilt den Vector a durch s.
 */
void divideVectorScalar (CGVector3D a, double s, CGVector3D * res)
{
    int i;
    for (i=0;i<3;i++)
        (*res)[i] = a[i] / s;
}

/**
 * Addiert a und b und schreibt das Ergebnis in res.
 */
void addVectorVector (CGVector3D a, CGVector3D b, CGVector3D * res)
{
    int i;
    for (i=0;i<3;i++)
        (*res)[i] = a[i] + b[i];
}

/**
 * Wandelt eine Zahl (Grad) in Radiant.
 * deg muss im Wertebereich 0..359 liegen!
 */
double degToRad (double deg)
{
    return deg*PI/180.0;
}

/**
 * Wandelt eine Zahl (Radiant) in Grad um.
 * deg muss im Wertebereich 0..PI liegen!
 */
double radToDeg (double rad)
{
    return rad*180.0/PI;
}

/**
 * Berechnet den Winkel zwischen zwei Vektoren und gibt das Ergebnis in
 * ° zurück (nicht radiant!).
 */
double angleVectorVector (CGVector3D a, CGVector3D b)
{
    return radToDeg (acos (multiplyVectorVector (a, b) / (vectorLength3D(a)*vectorLength3D(b))));
}
















