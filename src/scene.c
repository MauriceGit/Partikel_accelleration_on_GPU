
/**
 * @file
 * Hier wird vll gezeichnet...
 *
 * @author Maurice Tollmien, Tilman Nedele
 */

#ifdef _WIN32
#include <windows.h>
#endif

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "scene.h"
#include "logic.h"
#include "types.h"
#include "vector.h"

/**
 * Setzt global eine Farbe!
 * @param color1,color2,color3 - RGB
 * @param alpha
 * @param shininess
 * @param emission
 */
void setColor (float color1, float color2, float color3, float alpha, float shininess, int emission)
{
    float tmp[] = {0.0, 0.0, 0.0};
    tmp[0] = color1;
    tmp[1] = color2;
    tmp[2] = color3;

    {
        /* Material */
        float matAmbient[] = {0.0, 0.0, 0.0, 0.0};
        float matBlack[]= {0.0, 0.0, 0.0, 0.0};
        float matDiffuse[] = {0.0, 0.0, 0.0, 0.0};
        float matSpecular[] = {0.0, 0.0, 0.0, 0.0};
        float matShininess[] = { 0.0 };

        matShininess[0] = shininess;

        matAmbient[0] = tmp[0];
        matAmbient[1] = tmp[1];
        matAmbient[2] = tmp[2];
        matAmbient[3] = alpha;

        matSpecular[0] = tmp[0];
        matSpecular[1] = tmp[1];
        matSpecular[2] = tmp[2];
        matSpecular[3] = alpha;

        matDiffuse[0] = tmp[0] - 0.7;
        matDiffuse[1] = tmp[1] - 0.7;
        matDiffuse[2] = tmp[2] - 0.7;
        matDiffuse[3] = alpha;

        /* Setzen der Material-Parameter */
        glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
        glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
        glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
        glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
        if (emission)
            glMaterialfv (GL_FRONT, GL_EMISSION, matSpecular);
        else
            glMaterialfv (GL_FRONT, GL_EMISSION, matBlack);
    }
}

void drawFPS (void)
{
    /*GLfloat textColor[3] = { 1.0f, 1.0f, 1.0f };
    char * string = calloc ((strlen ("FPS = ") + 4), sizeof(char));
    sprintf(string, "FPS = %d", getFPS());

    drawString (0.6, 0.1, textColor, string);

    free (string);*/
}


/**
 * Initialisierung der Lichtquellen.
 * Setzt Eigenschaften der Lichtquellen (Farbe, Oeffnungswinkel, ...)
 */
static void initLight (void)
{

    /* Farbe der zweiten Lichtquelle */
    CGPoint4f lightColor1[3] =
    { {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f,
                                                           1.0f}
    };

    /* Farbe der ersten Lichtquelle */
    CGPoint4f lightColor2[3] =
    { {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f,
                                                           1.0f}
    };

    /* Oeffnungswinkel der zweiten Lichtquelle */
    GLdouble lightCutoff1 = 90.0f;
    /* Lichtverteilung im Lichtkegel der zweiten Lichtquelle */
    GLdouble lightExponent1 = 20.0f;

    float globalAmbientLight[] = {0.3, 0.3, 0.3, 1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

    /* Farbe der zweiten Lichtquelle setzen */
    glLightfv (GL_LIGHT1, GL_AMBIENT, lightColor1[0]);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightColor1[1]);
    glLightfv (GL_LIGHT1, GL_SPECULAR, lightColor1[2]);

    /* Spotlight-Eigenschaften der zweiten Lichtquelle setzen */
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, lightCutoff1);
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, lightExponent1);

    /* Farbe der zweiten Lichtquelle setzen */
    glLightfv (GL_LIGHT2, GL_AMBIENT, lightColor2[0]);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, lightColor2[1]);
    glLightfv (GL_LIGHT2, GL_SPECULAR, lightColor2[2]);

    /* Spotlight-Eigenschaften der zweiten Lichtquelle setzen */
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF, lightCutoff1);
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, lightExponent1);
}

/**
 * Bei SPIEelbegin wird das SPIEelfeld komplett initialisiert
 * mit einem Hintergrund, einer Zeichenfarbe, Linienbreite.
 * Außerdem wird die Datenhaltung initialisiert (siehe initField (), initStones ()).
 * @return Ob ein Fehler aufgetreten ist.
 */
int initScene (void)
{
    glEnable (GL_DEPTH_TEST);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glEnable (GL_NORMALIZE);
    glEnable (GL_LIGHTING);
    initLight ();

    return 1;
}



/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void
toggleWireframeMode (void)
{
    /* Flag: Wireframe: ja/nein */
    static GLboolean wireframe = GL_FALSE;

    /* Modus wechseln */
    wireframe = !wireframe;

    if (wireframe)
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}

