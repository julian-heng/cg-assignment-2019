#include <cglm/mat4.h>
#include <cglm/cam.h>
#include <cglm/vec3.h>
#include <cglm/affine.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "box.h"
#include "list.h"
#include "macros.h"

#include "camera.h"


static void linkMethods(Camera*);

static void getViewMatrix(Camera*, mat4);

static void moveForward(Camera*, float);
static void moveLeft(Camera*, float);
static void moveBackward(Camera*, float);
static void moveRight(Camera*, float);
static void moveMouse(Camera*, double, double, bool);
static void scrollMouse(Camera*, float);

static void attach(Camera*, Box*);

static void setPosition(Camera*, vec3);
static void setFront(Camera*, vec3);
static void setJump(Camera*, bool);

static void resetPosition(Camera*);
static void resetFront(Camera*);

static void poll(Camera*);
static void destroy(Camera*);

static void updateCameraVectors(Camera*);
static void jump(Camera*);

static float calcJump(float t);
static float _calcJump(float t);


Camera* newCamera()
{
    Camera* cam;

    if (! (cam = (Camera*)malloc(sizeof(Camera))))
    {
        fprintf(stderr, ERR_CAMERA_MALLOC);
        return NULL;
    }

    memset(cam, 0, sizeof(Camera));
    linkMethods(cam);

    glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, cam->position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, cam->front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, cam->up);
    glm_vec3_zero(cam->right);
    glm_vec3_copy(cam->up, cam->worldUp);

    cam->yaw = -90.0f;
    cam->pitch = 0.0f;

    cam->speed = 3.0f;
    cam->mouseSensitivity = 0.05f;
    cam->zoom = 45.0f;

    cam->attached = newList();

    updateCameraVectors(cam);

    return cam;
}


static void linkMethods(Camera* this)
{
    this->getViewMatrix = getViewMatrix;

    this->moveForward = moveForward;
    this->moveLeft = moveLeft;
    this->moveBackward = moveBackward;
    this->moveRight = moveRight;
    this->moveMouse = moveMouse;
    this->scrollMouse = scrollMouse;

    this->attach = attach;

    this->setPosition = setPosition;
    this->setFront = setFront;
    this->setJump = setJump;

    this->resetPosition = resetPosition;
    this->resetFront = resetFront;

    this->poll = poll;
    this->destroy = destroy;
}


static void getViewMatrix(Camera* this, mat4 result)
{
    vec3 temp;
    glm_vec3_add(this->position, this->front, temp);
    glm_lookat(this->position, temp, this->up, result);
}


static void moveForward(Camera* this, float timeDelta)
{
    ListNode* node;
    Box* attach;

    vec3 temp;
    glm_vec3_copy((vec3){this->front[0], 0.0f, this->front[2]}, temp);
    glm_vec3_normalize(temp);
    glm_vec3_scale(temp, this->speed * timeDelta, temp);
    glm_vec3_add(temp, this->position, this->position);

    FOR_EACH(this->attached, node)
    {
        attach = (Box*)(node->value);
        attach->move(attach, temp);
    }
}


static void moveLeft(Camera* this, float timeDelta)
{
    ListNode* node;
    Box* attach;

    vec3 temp;
    glm_vec3_scale(this->right, this->speed * timeDelta, temp);
    glm_vec3_sub(this->position, temp, this->position);

    glm_vec3_flipsign(temp);

    FOR_EACH(this->attached, node)
    {
        attach = (Box*)(node->value);
        attach->move(attach, temp);
    }
}


static void moveBackward(Camera* this, float timeDelta)
{
    ListNode* node;
    Box* attach;

    vec3 temp;
    glm_vec3_copy((vec3){this->front[0], 0.0f, this->front[2]}, temp);
    glm_vec3_normalize(temp);
    glm_vec3_scale(temp, this->speed * timeDelta, temp);
    glm_vec3_sub(this->position, temp, this->position);

    glm_vec3_flipsign(temp);

    FOR_EACH(this->attached, node)
    {
        attach = (Box*)(node->value);
        attach->move(attach, temp);
    }
}


static void moveRight(Camera* this, float timeDelta)
{
    ListNode* node;
    Box* attach;

    vec3 temp;
    glm_vec3_scale(this->right, this->speed * timeDelta, temp);
    glm_vec3_add(this->position, temp, this->position);

    FOR_EACH(this->attached, node)
    {
        attach = (Box*)(node->value);
        attach->move(attach, temp);
    }
}


static void moveMouse(Camera* this, double xoffset,
                      double yoffset, bool constraint)
{
    static bool first = true;
    static float lastYaw = 0.0f;

    ListNode* node;
    Box* attach;

    mat4 temp;

    lastYaw = this->yaw;

    this->yaw += xoffset * this->mouseSensitivity;
    this->pitch += yoffset * this->mouseSensitivity;
    this->pitch = constraint && this->pitch > 89.0f ? 89.0f : this->pitch;
    this->pitch = constraint && this->pitch < -89.0f ? -89.0f : this->pitch;

    if (first)
    {
        first = false;
        return;
    }

    glm_rotate_atm(temp, this->position,
                   glm_rad(this->yaw - lastYaw),
                   (vec3){0.0f, -1.0f, 0.0f});

    FOR_EACH(this->attached, node)
    {
        attach = (Box*)(node->value);
        attach->transformPosition(attach, temp);
        attach->setRotation(attach, (vec3){0.0f, -(this->yaw + 90.0f), 0.0f});
    }

    updateCameraVectors(this);
}


static void scrollMouse(Camera* this, float yoffset)
{
    this->zoom -= RANGE_INC(this->zoom, 1.0f, 45.0f) ? yoffset : 0;
    this->zoom = MAX(this->zoom, 1.0f);
    this->zoom = MIN(this->zoom, 45.0f);
}


static void attach(Camera* this, Box* box)
{
    this->attached->insertLast(this->attached, box, true);
}


static void setPosition(Camera* this, vec3 newPos)
{
    glm_vec3_copy(newPos, this->position);
}


static void setFront(Camera* this, vec3 newPos)
{
    glm_vec3_copy(newPos, this->front);
}


static void setJump(Camera* this, bool value)
{
    this->jumping = value;
}


static void resetPosition(Camera* this)
{
    ListNode* node;
    Box* attach;

    this->setPosition(this, (vec3){0.0f, 0.0f, 3.0f});

    FOR_EACH(this->attached, node)
    {
        attach = (Box*)(node->value);
        attach->resetPosition(attach);
        attach->resetRotation(attach);
    }
}


static void resetFront(Camera* this)
{
    this->setFront(this, (vec3){0.0f, 0.0f, 3.0f});
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    updateCameraVectors(this);
}


static void poll(Camera* this)
{
    jump(this);
}


static void destroy(Camera* this)
{
    this->attached->deleteListShallow(&this->attached);
}


static void updateCameraVectors(Camera* this)
{
    glm_vec3_normalize_to((vec3){
        cos(glm_rad(this->yaw)) * cos(glm_rad(this->pitch)),
        sin(glm_rad(this->pitch)),
        sin(glm_rad(this->yaw)) * cos(glm_rad(this->pitch))
    }, this->front);
    glm_vec3_crossn(this->front, this->worldUp, this->right);
    glm_vec3_crossn(this->right, this->front, this->up);
}


static void jump(Camera* this)
{
    static bool start = true;
    static float startTime = 0.0f;
    static float initialPosition = 0.0f;

    if (! this->jumping)
    {
        start = true;
        startTime = 0.0f;
        initialPosition = 0.0f;
        this->position[1] = initialPosition;
        this->setJump(this, false);

        return;
    }

    if (start)
    {
        start = false;
        startTime = glfwGetTime();
        initialPosition = this->position[1];
        return;
    }

    this->position[1] = initialPosition + calcJump(glfwGetTime() - startTime);

    if ((this->position[1] - initialPosition) < 0.0f)
    {
        start = true;
        startTime = 0.0f;
        initialPosition = 0.0f;
        this->position[1] = initialPosition;
        this->setJump(this, false);
    }
}


static float calcJump(float t)
{
    return (JUMP_HEIGHT / _calcJump(JUMP_DURATION / 2.0f)) * _calcJump(t);
}


static float _calcJump(float t)
{
    return -((t - JUMP_DURATION) * t);
}
