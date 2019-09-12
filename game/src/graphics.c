#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"

#include "graphics.h"


Backend* init()
{
    Backend* engine;

    if (! (engine = (Backend*)malloc(sizeof(Backend))))
    {
        fprintf(stderr, ERR_ENGINE_MALLOC);
        return NULL;
    }

    memset(engine, 0, sizeof(Backend));

    if (! (engine->cam = makeCamera()))
    {
        fprintf(stderr, ERR_CAMERA_MALLOC);
        return NULL;
    }

    initWindow(engine);
    initGlad(engine);

    if (engine->window)
    {
        glEnable(GL_DEPTH_TEST);
        initShader(engine);
        initShapes(engine);
        initTextures(engine);
    }

    glfwSetWindowUserPointer(engine->window, engine);

    return engine;
}


void initWindow(Backend* engine)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__) && defined(__MACH__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if ((engine->window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL)))
    {
        glfwMakeContextCurrent(engine->window);
        glfwSetKeyCallback(engine->window, input);
        glfwSetFramebufferSizeCallback(engine->window, framebuffer_size_callback);
    }
    else
    {
        fprintf(stderr, ERR_WINDOW);
    }
}


void initGlad(Backend* engine)
{
    if (engine->window &&
        ! gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, ERR_GLAD);
        engine->window = NULL;
    }
}


void initShader(Backend* engine)
{
    Shader* shader = makeShader("shaders/shader.vs", "shaders/shader.fs");
    engine->shaderPrograms[engine->programCount++] = shader;
}


void initShapes(Backend* engine)
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cubePositions[] = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

    engine->vertices = (float*)malloc(sizeof(vertices) * sizeof(float));
    engine->positions = (vec3*)malloc(sizeof(cubePositions) * sizeof(vec3));

    memcpy(engine->vertices, vertices, sizeof(vertices));
    memcpy(engine->positions, cubePositions, sizeof(cubePositions));

    glGenVertexArrays(1, &(engine->VAO));
    glGenBuffers(1, &(engine->VBO));

    glBindVertexArray(engine->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, engine->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void initTextures(Backend* engine)
{
    TextureSpec spec;

    spec.filename = "resources/container.jpg";
    spec.rgbMode = GL_RGB;
    spec.flip = false;
    generateTexture(&(engine->textures[engine->textureCount++]), spec);

    spec.filename = "resources/awesomeface.png";
    spec.rgbMode = GL_RGBA;
    spec.flip = true;
    generateTexture(&(engine->textures[engine->textureCount++]), spec);
}


void loop(Backend* engine)
{
    int i;

    float lastTime = glfwGetTime();
    float currentTime;

    mat4 model;
    mat4 projection;
    mat4 view;

    USE_SHADER(engine->shaderPrograms[0]);
    SET_SHADER_INT(engine->shaderPrograms[0], "texture1", 0);
    SET_SHADER_INT(engine->shaderPrograms[0], "texture2", 1);

    while (! glfwWindowShouldClose(engine->window))
    {
        currentTime = glfwGetTime();
        engine->timeDelta = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, engine->textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, engine->textures[1]);

        USE_SHADER(engine->shaderPrograms[0]);

        glm_perspective(glm_rad(engine->cam->zoom), ASPECT_RATIO, 0.1f, 100.0f, projection);
        SET_SHADER_MAT4(engine->shaderPrograms[0], "projection", projection);

        getCameraViewMatrix(engine->cam, view);
        SET_SHADER_MAT4(engine->shaderPrograms[0], "view", view);

        glBindVertexArray(engine->VAO);
        for (i = 0; i < 10; i++)
        {
            glm_mat4_copy((mat4)IDENTITY_MAT4, model);

            glm_translate(model, engine->positions[i]);
            glm_rotate(model, glm_rad(20.0f * i), (vec3){1.0f, 0.3f, 0.5f});
            glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
            glm_perspective(glm_rad(45.0f), ASPECT_RATIO, 0.1f, 100.0f, projection);

            SET_SHADER_MAT4(engine->shaderPrograms[0], "model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(engine->window);
        glfwPollEvents();
    }
}


void terminate(Backend** engine)
{
    int i;

    glDeleteVertexArrays(1, &((*engine)->VAO));
    glDeleteBuffers(1, &((*engine)->VBO));

    if (*engine)
    {
        for (i = 0; i < (*engine)->programCount; i++)
        {
            free((*engine)->shaderPrograms[i]);
            (*engine)->shaderPrograms[i] = NULL;
        }

        SAFE_FREE((*engine)->vertices);
        SAFE_FREE((*engine)->positions);

        free(*engine);
        *engine = NULL;
    }

    glfwTerminate();
}


void input(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    static int wireframeToggle = 0;
    Backend* engine = (Backend*)glfwGetWindowUserPointer(win);

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                glfwSetWindowShouldClose(win, true);
                break;

            case GLFW_KEY_TAB:
                wireframeToggle = (wireframeToggle + 1) % 3;
                glPolygonMode(GL_FRONT_AND_BACK,
                              wireframeToggle == 0 ? GL_FILL :
                              wireframeToggle == 1 ? GL_LINE :
                              GL_POINT);
                break;
        }
    }
    else if (action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_W:
                doCameraForwardMovement(engine->cam, engine->timeDelta);
                break;

            case GLFW_KEY_A:
                doCameraLeftMovement(engine->cam, engine->timeDelta);
                break;

            case GLFW_KEY_S:
                doCameraBackwardMovement(engine->cam, engine->timeDelta);
                break;

            case GLFW_KEY_D:
                doCameraRightMovement(engine->cam, engine->timeDelta);
                break;

            default:
                break;
        }
    }
}


void framebuffer_size_callback(GLFWwindow* win, int width, int height)
{
    glViewport(0, 0, width, height);
}
