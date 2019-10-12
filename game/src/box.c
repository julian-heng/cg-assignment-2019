#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "macros.h"
#include "material.h"
#include "shader.h"
#include "texture.h"

#include "box.h"


static float VERTICES[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


static void linkMethods(Box*);
static void setGlBuffers(Box*);

static void attach(Box*, Box*);

static void setShader(Box*, Shader*);
static void addTexture(Box*, Texture*);
static void setPosition(Box*, vec3);
static void setPositionDelta(Box*, vec3);
static void setScale(Box*, vec3);
static void setRotation(Box*, vec3);

static void recordInitialPosition(Box*);
static void recordInitialRotation(Box*);
static void resetPosition(Box*);
static void resetRotation(Box*);

static void move(Box*, vec3);
static void transformPosition(Box*, mat4);

static void draw(Box*);
static void destroy(Box*);


Box* newBox(vec3 position)
{
    Box* box;

    if (! (box = (Box*)malloc(sizeof(Box))))
    {
        fprintf(stderr, ERR_BOX_MALLOC);
        return NULL;
    }

    memset(box, 0, sizeof(Box));
    linkMethods(box);

    box->textures = newList();
    box->attached = newList();

    setGlBuffers(box);
    box->setPosition(box, position);
    box->setScale(box, NULL);
    box->setRotation(box, NULL);
    box->recordInitialPosition(box);
    box->material = newMaterial();

    if (! box->material)
    {
        box->textures->deleteListShallow(&(box->textures));
        free(box);
        return NULL;
    }

    return box;
}


static void linkMethods(Box* this)
{
    this->attach = attach;
    this->setShader = setShader;
    this->addTexture = addTexture;
    this->setPosition = setPosition;
    this->setPositionDelta = setPositionDelta;
    this->setScale = setScale;
    this->setRotation = setRotation;

    this->recordInitialPosition = recordInitialPosition;
    this->recordInitialRotation = recordInitialRotation;
    this->resetPosition = resetPosition;
    this->resetRotation = resetRotation;

    this->move = move;
    this->transformPosition = transformPosition;

    this->draw = draw;
    this->destroy = destroy;
}


static void attach(Box* this, Box* attach)
{
    this->attached->insertLast(this->attached, attach, true);
}


static void setGlBuffers(Box* this)
{
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(VERTICES),
                 VERTICES,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


static void setShader(Box* this, Shader* shader)
{
    this->shader = shader;
}


static void addTexture(Box* this, Texture* texture)
{
    this->textures->insertLast(this->textures, texture, true);
}


static void setPosition(Box* this, vec3 position)
{
    ListNode* iter;
    vec3 delta;

    glm_vec3_sub(position, this->position, delta);

    glm_vec3_copy(position ? position
                           : (vec3){0.0f, 0.0f, 0.0f}, this->position);

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->setPositionDelta((Box*)(iter->value), delta);
}


static void setPositionDelta(Box* this, vec3 position)
{
    ListNode* iter;
    glm_vec3_add(this->position, position, this->position);

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->setPositionDelta((Box*)(iter->value), position);
}


static void setScale(Box* this, vec3 scale)
{
    glm_vec3_copy(scale ? scale
                        : (vec3){1.0f, 1.0f, 1.0f}, this->scale);
}


static void setRotation(Box* this, vec3 rotation)
{
    glm_vec3_copy(rotation ? rotation
                           : (vec3){0.0f, 0.0f, 0.0f}, this->rotation);
}


static void recordInitialPosition(Box* this)
{
    ListNode* iter;
    glm_vec3_copy(this->position, this->initialPosition);

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->recordInitialPosition((Box*)(iter->value));
}


static void recordInitialRotation(Box* this)
{
    ListNode* iter;
    glm_vec3_copy(this->rotation, this->initialRotation);

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->recordInitialRotation((Box*)(iter->value));
}


static void resetPosition(Box* this)
{
    ListNode* iter;
    this->setPosition(this, this->initialPosition);

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->resetPosition((Box*)(iter->value));
}


static void resetRotation(Box* this)
{
    ListNode* iter;
    this->setRotation(this, this->initialRotation);

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->resetRotation((Box*)(iter->value));
}


static void move(Box* this, vec3 delta)
{
    glm_vec3_add(delta, this->position, this->position);
}


static void transformPosition(Box* this, mat4 transform)
{
    glm_mat4_mulv3(transform, this->position, 1.0f, this->position);
}


static void draw(Box* this)
{
    ListNode* iter;
    Box* box;
    int i = 0;

    mat4 model;

    this->shader->use(this->shader);

    this->shader->setVec3(this->shader,
                          "material.ambient",
                          this->material->ambient);

    this->shader->setInt(this->shader,
                         "material.diffuse",
                         this->material->diffuse);

    this->shader->setInt(this->shader,
                         "material.specular",
                         this->material->specular);

    this->shader->setFloat(this->shader,
                           "material.shininess",
                           this->material->shininess);

    LIST_FOR_EACH(this->textures, iter)
    {
        glActiveTexture(GL_TEXTURE0 + i++);
        glBindTexture(GL_TEXTURE_2D, ((Texture*)iter->value)->ID);
    }

    glBindVertexArray(this->VAO);
    glm_mat4_identity(model);

    glm_translate(model, this->position);
    glm_scale(model, this->scale);

    glm_rotate_x(model, glm_rad(this->rotation[0]), model);
    glm_rotate_y(model, glm_rad(this->rotation[1]), model);
    glm_rotate_z(model, glm_rad(this->rotation[2]), model);

    this->shader->setMat4(this->shader, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    LIST_FOR_EACH(this->attached, iter)
    {
        box = (Box*)(iter->value);
        box->setShader(box, this->shader);
        box->draw(box);
    }

    i = 0;
    LIST_FOR_EACH(this->textures, iter)
    {
        glActiveTexture(GL_TEXTURE0 + i++);
        //glBindTexture(GL_TEXTURE_2D, ((Texture*)iter->value)->ID);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}


static void destroy(Box* this)
{
    ListNode* iter;

    LIST_FOR_EACH(this->attached, iter)
        ((Box*)(iter->value))->destroy((Box*)(iter->value));
    this->attached->deleteList(&this->attached);
    this->textures->deleteListShallow(&this->textures);
}
