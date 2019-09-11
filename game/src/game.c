#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "game.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main()
{
    GLFWwindow* win;

    if ((win = init()))
    {
        loop(win);
    }

    terminate();
    return win == NULL;
}

GLFWwindow* init()
{
    GLFWwindow* win;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__) && defined(__MACH__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if ((win = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game", NULL, NULL)) == NULL)
    {
        fprintf(stderr, "Failed to initialise window\n");
    }
    else
    {
        glfwMakeContextCurrent(win);
        glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

        if (! gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            fprintf(stderr, "Failed to initialise GLAD\n");
            win = NULL;
        }
    }

    return win;
}

void terminate()
{
    glfwTerminate();
}

void loop(GLFWwindow* win)
{
    while (! glfwWindowShouldClose(win))
    {
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(win, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* win, int width, int height)
{
    glViewport(0, 0, width, height);
}
