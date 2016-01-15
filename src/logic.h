#ifndef __LOGIC_H__
#define __LOGIC_H__
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

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include "types.h"

/* eig. Funktionen */

/* ------- GETTER / SETTER ------- */
void setMouseEvent(MouseInterpretType state,int x, int y);
void setMouseState(MouseInterpretType state);
void setMouseCoord(int x, int y);
MouseInterpretType getMouseEvent(void);
double getCameraPosition (int axis);
void setCameraMovement(int x,int y);
void setCameraZoom(int x,int y);
void setKey (int key, int value);
int getFPS(void);

/* ------- BERECHNUNGEN ------- */
void moveCamera(int x, int y);
void zoomCamera(int x, int y);
void calcTimeRelatedStuff (double interval);
void drawRecursiveQuad(CGPoint3f p1, CGPoint3f p2, CGPoint3f p3, CGPoint3f p4, int rek, int color);
/* ------- INIT ------- */

void initGame ();



#endif
