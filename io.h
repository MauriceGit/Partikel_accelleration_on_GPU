#ifndef __IO_H__
#define __IO_H__
#define __DEBUG_GL_H__
/**
 * @file
 * CallBack-Funktionsköpfe
 *
 * @author Maurice Tollmien
 */

#include <stdio.h>
#include <stdarg.h>

int initAndStartIO (char *title, int width, int height);
void registerCallBacks (GLFWwindow * window);
void mainLoop (GLFWwindow * window);

#endif
