#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>

#include "list.h"
#include "camera.h"
#include "shader.h"

#define WIDTH 1440
#define HEIGHT 900
#define TITLE "CG Assignment"

#define ERR_ENGINE_MALLOC "Error: Unable to allocate memory for engine\n"
#define ERR_WINDOW "Error: failed to initialise window\n"
#define ERR_GLAD "Error: failed to initialise GLAD\n"


typedef enum {
    CAM_MOVE_FORWARD,
    CAM_MOVE_LEFT,
    CAM_MOVE_BACKWARD,
    CAM_MOVE_RIGHT,
    CAM_JUMP,

    CAM_RESET
} KeyAction;


typedef struct Backend
{
    GLFWwindow* window;
    unsigned int VAO;
    unsigned int VBO;

    int width;
    int height;

    List* shaders;
    List* textures;

    List* boxes;

    Camera* cam;
    float timeDelta;
} Backend;


Backend* init(void);
void initWindow(Backend*);
void initGlad(Backend*);
void initShader(Backend*);
void initShapes(Backend*);
void initTextures(Backend*);

void loop(Backend*);

void draw(Backend* engine);

void toggleWireframe(void);

void normalInputCallback(GLFWwindow*, int, int, int, int);
void instantKeyInputCallback(GLFWwindow*);
void mouseCallback(GLFWwindow*, double, double);
void scrollCallback(GLFWwindow*, double, double);
void framebufferSizeCallback(GLFWwindow*, int, int);

void terminate(Backend**);


#endif
