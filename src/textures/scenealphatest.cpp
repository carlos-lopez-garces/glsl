#include "scenealphatest.h"
#include "texture.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

SceneAlphaTest::SceneAlphaTest() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f),
                                   teapot(50, glm::mat4(1.0f))
{ }

void SceneAlphaTest::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

  prog.setUniform("ChinaMaterial.Kd", 0.5f, 0.5f, 0.5f);
  prog.setUniform("ChinaMaterial.Ka", 0.5f, 0.5f, 0.5f);
  prog.setUniform("ChinaMaterial.Ks", 0.9f, 0.9f, 0.9f);
  prog.setUniform("ChinaMaterial.Shininess", 130.0f);
  prog.setUniform("ChinaMaterial.Ld", 1.0f, 1.0f, 1.0f);
  prog.setUniform("ChinaMaterial.La", 0.4f, 0.4f, 0.4f);
  prog.setUniform("ChinaMaterial.Ls", 1.0f, 1.0f, 1.0f);

  prog.setUniform("CementMaterial.Kd", 0.7f, 0.7f, 0.7f);
  prog.setUniform("CementMaterial.Ka", 0.5f, 0.5f, 0.5f);
  prog.setUniform("CementMaterial.Ks", 0.0f, 0.0f, 0.0f);
  prog.setUniform("CementMaterial.Shininess", 0.0f);
  prog.setUniform("CementMaterial.Ld", 0.5f, 0.5f, 0.5f);
  prog.setUniform("CementMaterial.La", 0.4f, 0.4f, 0.4f);
  prog.setUniform("CementMaterial.Ls", 0.0f, 0.0f, 0.0f);

  GLuint texIDs[2];
  glGenTextures(2, texIDs);

  // Brick texture.
  int width = 0, height = 0;
  unsigned char* cementImg = Texture::loadPixels("../media/texture/cement.jpg", width, height);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texIDs[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cementImg);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  int uniloc = glGetUniformLocation(prog.getHandle(), "CementTex");
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

void SceneAlphaTest::update( float t )
{
    float deltaT = t - tPrev;
    if(tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;

    if( this->m_animate ) {
	angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
    }
}

void SceneAlphaTest::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(6.0f * cos(angle), 0.25f, 6.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

    glm::vec4 worldLight = glm::vec4(0.0f, -1.5f, 1.5f, 0.5f);
    prog.setUniform("Light.Position", view * worldLight);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f,-1.5f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot.render();
}

void SceneAlphaTest::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneAlphaTest::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneAlphaTest::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/alphatest_41.vs");
    prog.compileShader("shader/alphatest_41.fs");
#else
    prog.compileShader("shader/alphatest.my.vert.glsl");
    prog.compileShader("shader/alphatest.my.frag.glsl");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
