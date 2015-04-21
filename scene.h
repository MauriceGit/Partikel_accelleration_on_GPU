#ifndef __SCENE_H__
#define __SCENE_H__

#define __DEBUG_GL_H__
/**
 * @file
 * hier kann gezeichnet werden (Hoffentlich^^)
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

void drawVertex(CGPoint3f p1, CGPoint3f p2, CGPoint3f p3, CGPoint3f p4, int color);
void drawScene (void);
void drawWaterSurface(void);
void drawFPS (void);
void drawSides(void);
int initScene (void);
void toggleWireframeMode (void);

#endif
