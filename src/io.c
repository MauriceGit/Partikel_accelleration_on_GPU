

/**
 * @file
 * Hier sind alle CallBack-Funktionen
 *
 * @author Maurice Tollmien, Tilman Nedele
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>




/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


/* ---- Eigene Header einbinden ---- */
#include "io.h"
#include "scene.h"
#include "logic.h"
#include "vector.h"
#include "imageLoader.h"


/* Shader-ID's */
GLuint G_ShaderColor, G_ComputeShader;

/* Shader-Variablen */
GLuint G_Velocity_buffer_loc, G_Position_buffer_loc;
Attractor G_Attractor;
// AttractorMass G_Attractor_Mass;
GLuint G_Position_buffer_tex, G_Velocity_buffer_tex;

/* Geometrie-Buffer */
GLuint G_ObjectsBuffer, G_Compute_Buffers, G_Position_buffer, G_Velocity_buffer, G_Attractor_buffer, G_AttractorMass_buffer, G_Life_buffer;

GLuint G_Dispatch_buffer;

static const struct
{
    GLuint num_groups_x;
    GLuint num_groups_y;
    GLuint num_groups_z;
} G_Dispatch_params = {16, 16, 1};

Vec3 * G_ComputePositions;
Vec4 * G_ComputeVelocities;
Vec4 * G_ComputeAttractors;
float * G_ComputeAttractorMass;
float * G_ComputeLife;

int G_Width = 1920;
int G_Height = 1080;
int G_FullScreen = 1;
char* G_WindowTitle = "";
GLFWwindow * G_Window = NULL;
float G_Interval = 0;
float G_ThisCallTime = 0;

int G_FPS_Count = 0;
double G_FPS_All = 0;


GLfloat G_Objects[] = {
    -15.0, -15.0, -15.0,
    15.0, -15.0, -15.0,
    15.0, 15.0, -15.0,
    -15.0, -15.0, -15.0,
    15.0, 15.0, -15.0,
    -15.0, 15.0, -15.0
};



/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und Farben und anschliessendes
 * Neuzeichnen, setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
double cbTimer (int lastCallTime)
{
    /* Seit dem Programmstart vergangene Zeit in Sekunden */
    G_Interval = glfwGetTime();
    glfwSetTime(0.0);

    G_FPS_Count++;
    G_FPS_All += G_Interval;

    if (G_FPS_Count >= 1000) {
        printf ("fps: %i\n", (int) (1.0 / ((double)G_FPS_All / (double)G_FPS_Count)));
        G_FPS_All = 0.0;
        G_FPS_Count = 0;
    }

    /*printf ("dt: %f\n", G_Interval);*/

    calcTimeRelatedStuff(G_Interval);
    return G_Interval;
}

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Berücksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
static void
setProjection (GLdouble aspect)
{
  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  {
      /* perspektivische Projektion */
      gluPerspective (90.0,     /* Oeffnungswinkel */
                      aspect,   /* Seitenverhaeltnis */
                      0.5,      /* nahe ClipPIEng-Ebene */
                      10000.0 /* ferne ClipPIEng-Ebene */ );
  }
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void cbDisplay (GLFWwindow * window)
{
    int i;
    int modValue = 3000;
    double difValue = 10.0;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glDisable(GL_CULL_FACE);

    glViewport (0, 0, G_Width, G_Height);
    setProjection ((double)G_Width/G_Height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (getCameraPosition(0), getCameraPosition(1), getCameraPosition(2),
         0.0, 0.0, 0.0,
         0.0, 1.0, 0.0);

    G_ComputeAttractors = (Vec4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, ATTRACTOR_COUNT*sizeof(Vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (i = 0; i < ATTRACTOR_COUNT; i++) {
        G_ComputeAttractors[i].x = sinf(G_FPS_All) * (rand()%500)/10.0;
        G_ComputeAttractors[i].y = cosf(G_FPS_All) * (rand()%500)/10.0;
        G_ComputeAttractors[i].z = tanf(G_FPS_All);
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glGenBuffers(1, &G_Attractor_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, G_Attractor_buffer);
    glBufferData(GL_ARRAY_BUFFER, ATTRACTOR_COUNT * sizeof(Vec4), G_ComputeAttractors, GL_DYNAMIC_COPY);

    if (1) {
        glUseProgram(G_ComputeShader);

        /* Zeit-Interval-Geschichte */
        glUniform1fv(glGetUniformLocation(G_ComputeShader, "dt"), 1, &G_Interval);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, G_Position_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, G_Velocity_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, G_Attractor_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, G_Life_buffer);

        glDispatchCompute(PARTICLE_COUNT / 128, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glUseProgram(0);
    }

    glDeleteBuffers(1, &G_Attractor_buffer);

    glUseProgram(G_ShaderColor);

    GLfloat mp[16], mv[16];
    glGetFloatv(GL_PROJECTION_MATRIX, mp);
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glUniformMatrix4fv(glGetUniformLocation(G_ShaderColor, "projMatrix"),  1, GL_FALSE, &mp[0]);
    glUniformMatrix4fv(glGetUniformLocation(G_ShaderColor, "viewMatrix"),  1, GL_FALSE, &mv[0]);

    /* Vertex-Buffer zum Rendern der Positionen */
    glBindBuffer (GL_ARRAY_BUFFER, G_Position_buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glBindBuffer (GL_ARRAY_BUFFER, G_Life_buffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);

    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

    glEnable(GL_CULL_FACE);
    glUseProgram(0);



    /* fuer DoubleBuffering */
    glfwSwapBuffers(window);

    glfwSwapInterval(0);
}

/**
 * Geht in den Windowed/Fullscreen-Mode.
 */
int createWindow(void)
{
    if (G_Window)
        glfwDestroyWindow(G_Window);

    glfwDefaultWindowHints();

    if (G_FullScreen)
        G_Window = glfwCreateWindow(1920, 1080, G_WindowTitle, glfwGetPrimaryMonitor(), NULL);
    else
        G_Window = glfwCreateWindow(G_Width, G_Height, G_WindowTitle, NULL, NULL);

    if (G_Window) {
        glfwMakeContextCurrent(G_Window);
        glfwGetFramebufferSize(G_Window, &G_Width, &G_Height);
    } else {
        return 0;
    }

    return 1;
}

/**
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
void cbKeyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key)
        {
            case 'q':
            case 'Q':
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_R:
                break;
            case GLFW_KEY_H:
                break;
            case 'n':
            case 'N':
                break;
            case 'w':
            case 'W':
                break;
            case 'f':
            case 'F':
                /*G_FullScreen = !G_FullScreen;
                createWindow();
                registerCallBacks (G_Window);
                mainLoop (G_Window);*/
                break;
            case 's':
            case 'S':
                break;
            case 'k':
            case 'K':
                break;
            case 'v':
            case 'V':
                break;
            case GLFW_KEY_UP:
                setKey (1, 1);
                break;
            case GLFW_KEY_DOWN:
                setKey(0,1);
                break;
            case GLFW_KEY_F1:
                toggleWireframeMode();
                break;
        }
    }

    if (action == GLFW_RELEASE) {

        switch (key)
        {
            case GLFW_KEY_LEFT:

                break;
            case GLFW_KEY_RIGHT:

                break;
            case GLFW_KEY_UP:
                setKey (1,0);
                break;
            case GLFW_KEY_DOWN:
                setKey (0,0);
                break;
        }
    }

}

/**
 * Mouse-Button-Callback.
 * @param button Taste, die den Callback ausgeloest hat.
 * @param state Status der Taste, die den Callback ausgeloest hat.
 * @param x X-Position des Mauszeigers beim Ausloesen des Callbacks.
 * @param y Y-Position des Mauszeigers beim Ausloesen des Callbacks.
 */
void cbMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {

        if (action == GLFW_RELEASE)
            setMouseState(NONE);
        else
            setMouseState(MOVE);

    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {

        if (action == GLFW_RELEASE)
            setMouseState(NONE);
        else
            setMouseState(ZOOM);
    }
}

static void cbMouseMotion (GLFWwindow* window, double x, double y)
{
    if (getMouseEvent() == MOVE)
        setCameraMovement(x,y);

    if (getMouseEvent() == ZOOM)
        setCameraZoom(x,y);

    setMouseCoord(x,y);
}




/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veraenderte Fenstergroesse.
 * @param w Fensterbreite (In).
 * @param h Fensterhoehe (In).
 */
void cbReshape (GLFWwindow* window, int w, int h)
{
  /* Das ganze Fenster ist GL-Anzeigebereich */
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);

  /* Anpassen der Projektionsmatrix an das Seitenverhaeltnis des Fensters */
  setProjection ((GLdouble) w / (GLdouble) h);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    printf ("Key Callback with: key: [%d], scancode: [%d], action: [%d], mods: [%d]\n", key, scancode, action, mods);
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
void registerCallBacks (GLFWwindow * window)
{

    /* Reshape-Callback - wird ausgefuehrt, wenn neu gezeichnet wird (z.B. nach
    * Erzeugen oder Groessenaenderungen des Fensters) */
    glfwSetFramebufferSizeCallback (window, cbReshape);

    glfwSetKeyCallback (window, cbKeyboard);

    glfwSetCursorPosCallback (window, cbMouseMotion);

    glfwSetMouseButtonCallback (window, cbMouseButton);
}

int readFile (char * name, GLchar ** buffer) {
    FILE *f = fopen(name, "rb");
    fseek(f, 0, SEEK_END);
    int pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    (*buffer) = malloc(pos+1);
    fread(*buffer, pos-1, 1, f);
    (*buffer)[pos-1] = '\0';
    fclose(f);
}

GLuint loadComputeShader(char * computeShader)
{
      /* Create the shaders */
    GLuint computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    GLint result = GL_FALSE;
    int infoLogLength;

    /* Compile Vertex Shader */
    printf("Compiling Compute shader\n");
    char * computeSourcePointer = NULL;
    readFile(computeShader, &computeSourcePointer);

    glShaderSource(computeShaderID, 1, (const GLchar **)&computeSourcePointer , NULL);
    glCompileShader(computeShaderID);

    /* Check Vertex Shader */
    glGetShaderiv(computeShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(computeShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    char * computeShaderErrorMessage = calloc(infoLogLength, sizeof(char));
    glGetShaderInfoLog(computeShaderID, infoLogLength, NULL, &(computeShaderErrorMessage[0]));
    fprintf(stdout, "computeShaderErrorMessage: %s\n", computeShaderErrorMessage);

    /*  Link the program */
    GLuint programID = glCreateProgram();

    glAttachShader(programID, computeShaderID);

    printf ("Compute Shader ID: %d\n", computeShaderID);

    glLinkProgram(programID);

    /* Check the program */
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    char * programErrorMessage = calloc(infoLogLength, sizeof(char));
    glGetProgramInfoLog(programID, infoLogLength, NULL, &(programErrorMessage[0]));
    fprintf(stdout, "programErrorMessage: %s\n", programErrorMessage);

    glDeleteShader(computeShaderID);

    printf ("Compute Shader Program ID: %d\n", programID);

    return programID;
}

GLuint loadShaders(char * vertexShader, char * fragmentShader){

    /* Create the shaders */
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    /* Compile Vertex Shader */
    printf("Compiling Vertex shader\n");
    char * VertexSourcePointer = NULL;
    readFile(vertexShader, &VertexSourcePointer);

    glShaderSource(VertexShaderID, 1, (const GLchar **)&VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    /* Check Vertex Shader */
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    char * vertexShaderErrorMessage = calloc(InfoLogLength, sizeof(char));
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &(vertexShaderErrorMessage[0]));
    fprintf(stdout, "vertexShaderErrorMessage: %s\n", vertexShaderErrorMessage);

    /* Compile Fragment Shader */
    printf("Compiling Fragment shader\n");
    char * FragmentSourcePointer = NULL;
    readFile(fragmentShader, &FragmentSourcePointer);

    glShaderSource(FragmentShaderID, 1, (const GLchar **)&FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    /* Check Fragment Shader */
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    char * fragmentShaderErrorMessage = calloc(InfoLogLength, sizeof(char));
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &(fragmentShaderErrorMessage[0]));
    fprintf(stdout, "fragmentShaderErrorMessage: %s\n", fragmentShaderErrorMessage);

    /*  Link the program */
    GLuint ProgramID = glCreateProgram();

    glAttachShader(ProgramID, VertexShaderID);

    glAttachShader(ProgramID, FragmentShaderID);

    glLinkProgram(ProgramID);

    /* Check the program */
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    char * programErrorMessage = calloc(InfoLogLength, sizeof(char));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &(programErrorMessage[0]));
    fprintf(stdout, "programErrorMessage: %s\n", programErrorMessage);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

/**
 * Selbstgemachter Main-Loop mit Callback, ob das Fenster geschlossen werden soll.
 */
void mainLoop (GLFWwindow * window)
{
    double lastCallTime = cbTimer(0.0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    while (!glfwWindowShouldClose(window))
    {
        cbDisplay (window);
        lastCallTime = cbTimer (lastCallTime);
        glfwPollEvents();
    }

}

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int initAndStartIO (char *title, int width, int height)
{

    G_Width = width;
    G_Height = height;
    /*G_WindowTitle = malloc(sizeof(title)*sizeof(char));*/
    G_WindowTitle = title;
    G_FullScreen = 0;

    if (!glfwInit())
        return 0;

    if (createWindow())
    {
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
          /* Problem: glewInit failed, something is seriously wrong. */
          printf("Error: %s\n", glewGetErrorString(err));
          glfwDestroyWindow(G_Window);
          exit(1);
        }
        printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
        /* Hintergrund und so werden initialisiert (Farben) */
        if (initScene ())
        {
            int i;
            printf ("--> Shader laden...\n"); fflush(stdout);

            G_ShaderColor = loadShaders("colorVertexShader.vert", "colorFragmentShader.frag");
            G_ComputeShader = loadComputeShader("particlesComputeShader.comp");

            printf ("--> Shader sind geladen.\n"); fflush(stdout);

            registerCallBacks (G_Window);

            glGenBuffers(1, &G_ObjectsBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, G_ObjectsBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(G_Objects), G_Objects, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            /* Physik-Simulation! ==================================================*/

            /* Position */
            G_ComputePositions = calloc(PARTICLE_COUNT, sizeof(Vec3));
            for (i = 0; i < PARTICLE_COUNT; i++) {
                Vec3 vec;
                vec.x = (rand() % 2000) / (500.0);
                vec.y = (rand() % 2000) / (500.0);
                vec.z = (rand() % 2000) / (500.0);
                G_ComputePositions[i] = vec;
            }

            glGenBuffers    (1, &G_Position_buffer);
            glBindBuffer    (GL_ARRAY_BUFFER, G_Position_buffer);
            glBufferData    (GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(Vec3), G_ComputePositions, GL_DYNAMIC_COPY);

            /* Lebensdauer */
            G_ComputeLife = calloc(PARTICLE_COUNT, sizeof(float));
            for (i = 0; i < PARTICLE_COUNT; i++) {
                G_ComputeLife[i] = rand() / (double)RAND_MAX;
            }

            glGenBuffers    (1, &G_Life_buffer);
            glBindBuffer    (GL_ARRAY_BUFFER, G_Life_buffer);
            glBufferData    (GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(float), G_ComputeLife, GL_DYNAMIC_COPY);

            /* Geschwindigkeit */
            G_ComputeVelocities = calloc(PARTICLE_COUNT, sizeof(Vec4));
            for (i = 0; i < PARTICLE_COUNT; i++) {
                Vec4 vec;
                vec.x = (rand() % 100) / 500.0 - (rand() % 100) / 500.0;
                vec.y = (rand() % 100) / 500.0 - (rand() % 100) / 500.0;
                vec.z = (rand() % 100) / 500.0 - (rand() % 100) / 500.0;
                vec.w =  0.0;
                G_ComputeVelocities[i] = vec;
            }

            glGenBuffers    (1, &G_Velocity_buffer);
            glBindBuffer    (GL_ARRAY_BUFFER, G_Velocity_buffer);
            glBufferData    (GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(Vec4), G_ComputeVelocities, GL_DYNAMIC_COPY);

            /* Anziehungspunkte */
            G_ComputeAttractors = calloc(ATTRACTOR_COUNT, sizeof(Vec4));
            for (i = 0; i < ATTRACTOR_COUNT; i++) {
                Vec4 vec;
                vec.x = (rand() % 500) / 30.0 - (rand() % 500) / 30.0;
                vec.y = (rand() % 500) / 30.0 - (rand() % 500) / 30.0;
                vec.z = (rand() % 500) / 30.0 - (rand() % 500) / 30.0;
                vec.w = 0;
                G_ComputeAttractors[i] = vec;
            }
            glGenBuffers(1, &G_Attractor_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, G_Attractor_buffer);
            glBufferData(GL_ARRAY_BUFFER, ATTRACTOR_COUNT * sizeof(Vec4), G_ComputeAttractors, GL_DYNAMIC_COPY);

            printf ("--> Initialisierung angeschlossen.\n"); fflush(stdout);

            /* Die Endlosschleife wird angestoßen */
            mainLoop (G_Window);


        } else {
            glfwDestroyWindow(G_Window);
            return 0;
        }
    } else {
        return 0;
    }

    glDeleteBuffers(1, &G_Position_buffer);
    glDeleteBuffers(1, &G_Velocity_buffer);

    glfwDestroyWindow(G_Window);

    return 1;
}
