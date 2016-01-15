
/**
 * @file
 * Hier ist die Datenhaltung und Programmlogik
 *
 * @author Tilman Nedele, Maurice Tollmien
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


/* ---- Eigene Header einbinden ---- */
#include "logic.h"
#include "vector.h"
#include "scene.h"
#include "types.h"

/** Mausbewegung zoom/move/none */
MouseInterpretType G_Mouse;
/** Kameraposition */
CGVector3D G_CameraPosition = {CAMERA_X,CAMERA_Y,CAMERA_Z};
/** Position der Maus */
Movement G_MouseMove = {0.0,0.0,0.0};
/** Startpunkt der Mausbewegung */
CGVector3D G_LastMouseCenter = {0.0,0.0,0.0};

/** Partikel hinzufügen? */
int G_UpDownKeys[2] = {0,0};
/** FPS */
int G_FPS = 0;
/** FPS counter */
double G_Counter = 0.0-EPS;

/* ------- GETTER / SETTER ------- */

int getFPS(void)
{
	return G_FPS;
}

/**
 * Setzt, ob ein Up/Down-Key gedrückt ist.
 * key == 0 = down
 * key == 1 = up
 * value = gesetzt/nicht gesetzt.
 */
void setKey (int key, int value)
{
    if (key < 2)
        G_UpDownKeys[key] = value;
}

/**
 * Set-Function für den Status der Maus
 * @param Status der Maus
 */
void setMouseEvent(MouseInterpretType state,int x, int y) {
    G_MouseMove[0] = 0.0;
    G_MouseMove[2] = 0.0;
    G_LastMouseCenter[0] = x;
    G_LastMouseCenter[2] = y;
        
    G_Mouse = state;
}

/**
 * Mouse Status.
 */
void setMouseState(MouseInterpretType state) {
	G_Mouse = state;
}

void setMouseCoord(int x, int y) {
	G_LastMouseCenter[0] = x;
	G_LastMouseCenter[2] = y;
}

/**
 * Get-Function für den Status der Maus
 * @return Status der Maus
 */
MouseInterpretType getMouseEvent() {
    return G_Mouse;
}

/**
 * Gibt die Kamerakoordinate zurück.
 */
double getCameraPosition (int axis)
{
    if (axis >= 0 && axis < 3)
        return G_CameraPosition[axis];
        
    return 0.0;
}

/**
 * Bewegt die Kamera auf einem Kugelradius um den Szenenmittelpunkt.
 */
void setCameraMovement(int x,int y)
{
	CGVector3D tmp;
	double factor, radius = vectorLength3D(G_CameraPosition);
	
	tmp[0] = G_CameraPosition[0];
	tmp[1] = G_CameraPosition[1];
	tmp[2] = G_CameraPosition[2];
	
	G_MouseMove[0] = x-G_LastMouseCenter[0];
	G_MouseMove[2] = y-G_LastMouseCenter[2];
	
	G_LastMouseCenter[0] = x;
	G_LastMouseCenter[2] = y;
	
	/* Bewegung um Y-Achse: */
	G_CameraPosition[0] = cos(-G_MouseMove[0]*PI/180.0/CAMERA_MOVEMENT_SPEED)*tmp[0] + sin(-G_MouseMove[0]*PI/180.0/CAMERA_MOVEMENT_SPEED)*tmp[2];
	G_CameraPosition[2] = -sin(-G_MouseMove[0]*PI/180.0/CAMERA_MOVEMENT_SPEED)*tmp[0] + cos(-G_MouseMove[0]*PI/180.0/CAMERA_MOVEMENT_SPEED)*tmp[2];
	
	/* Bewegung oben/unten */
	G_CameraPosition[1] += G_MouseMove[2]/(CAMERA_MOVEMENT_SPEED/2)*(vectorLength3D(G_CameraPosition)/100.0);
	factor = 1.0 / (vectorLength3D(G_CameraPosition) / radius);
	multiplyVectorScalar (G_CameraPosition, factor, &G_CameraPosition);
		
}

/**
 * Verlängert/verkürzt den Vektor zur Kamera.
 */
void setCameraZoom(int x,int y)
{
	double factor = 1.0 + (CAMERA_ZOOM_SPEED / 1000.0) * ((G_MouseMove[2] < 0.0) ? -1.0 : 1.0);
	
	G_MouseMove[0] = x-G_LastMouseCenter[0];
	G_MouseMove[2] = y-G_LastMouseCenter[2];
	
	G_LastMouseCenter[0] = x;
	G_LastMouseCenter[2] = y;
	
	G_CameraPosition[0] *= factor;
	G_CameraPosition[1] *= factor;
	G_CameraPosition[2] *= factor;
}

/* ------- BERECHNUNGEN ------- */

/**
 * Berechnet alle Funktionen, die vom interval abhängig sind
 * @param interval - Zeit
 */
void calcTimeRelatedStuff (double interval)
{
    G_Counter += interval;
    
    if (G_Counter >= 1.0)
		G_Counter = 0.0 -EPS;
    
    if (G_Counter <= 0.0)
		G_FPS = (int) 1.0/interval;
}

/* ------- INIT ------- */

/**
 * Initialisiert die Kamera.
 */
void initCameraPosition ()
{
    G_CameraPosition[0] = CAMERA_X;
    G_CameraPosition[1] = CAMERA_Y;
    G_CameraPosition[2] = CAMERA_Z;
}

/**
 * Hier findet die komplette Initialisierung des kompletten SPIEeles statt.
 * Inklusive der Datenhaltung und des SPIEelfeldes.
 */
void initGame ()
{   
    initCameraPosition();
    /*setRandomize();*/
}

