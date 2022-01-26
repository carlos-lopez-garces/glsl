#ifndef SCENETEXTURE_H
#define SCENETEXTURE_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"
#include "sphere.h"

#include <glm/glm.hpp>

class SceneTexture : public Scene
{
private:
    GLSLProgram prog;

    Cube cube;
    Sphere sphere;

    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTexture();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPERFRAGMENT_H
