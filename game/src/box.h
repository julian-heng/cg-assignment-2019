#ifndef BOX_H
#define BOX_H

#include <cglm/vec3.h>

#include "list.h"
#include "texture.h"

#include "shader.h"

#define ERR_BOX_MALLOC "Error: unable to allocate memory for box\n"

typedef struct Box
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int vertexAttribPointerIndex;

    Shader* shader;
    List* textures;
    vec3 position;
    vec3 scale;
    vec3 rotation;

    void (*setShader)(struct Box*, Shader*);
    void (*addTexture)(struct Box*, Texture*);
    void (*setPosition)(struct Box*, vec3);
    void (*setScale)(struct Box*, vec3);
    void (*setRotation)(struct Box*, vec3);

    void (*draw)(struct Box*);
} Box;

Box* newBox(vec3);

#endif
