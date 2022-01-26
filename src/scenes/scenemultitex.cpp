#include "scenemultitex.h"
#include "texture.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneMultiTex::SceneMultiTex() : cube() { }

void SceneMultiTex::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(1.0f,1.25f,1.25f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);
    glm::vec4 worldLight = glm::vec4(1.0f, 0.0f, 1.0f, 0.5f);

    prog.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);

    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);

    prog.setUniform("Light.Position", view * worldLight);
    prog.setUniform("Material.Shininess", 10.0f);

    GLuint texIDs[2];
    glGenTextures(2, texIDs);

    // Brick texture.
    int width = 0, height = 0;
    unsigned char* brickImg = Texture::loadPixels("../media/texture/brick1.jpg", width, height);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIDs[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, brickImg);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    int uniloc = glGetUniformLocation(prog.getHandle(), "BrickTex");
    std::cout << "uniloc " << uniloc << "\n";
    if (uniloc >= 0) glUniform1i(uniloc, 0);

    unsigned char* mossImg = Texture::loadPixels("../media/texture/moss.png", width, height);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texIDs[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mossImg);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    uniloc = glGetUniformLocation(prog.getHandle(), "MossTex");
    if (uniloc >= 0) glUniform1i(uniloc, 1);
    std::cout << "uniloc " << uniloc << "\n";
}

void SceneMultiTex::update( float t ) { }

void SceneMultiTex::render()
{
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = mat4(1.0f);
    setMatrices();
    cube.render();
}

void SceneMultiTex::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneMultiTex::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneMultiTex::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/multitex_41.vs");
    prog.compileShader("shader/multitex_41.fs");
#else
    prog.compileShader("shader/multitex.my.vert.glsl");
    prog.compileShader("shader/multitex.my.frag.glsl");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
