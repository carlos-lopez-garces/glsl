#include "scenerefractcube.h"
#include "texture.h"
#include "stb/stb_image.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>
using glm::vec3;
using glm::mat4;

SceneRefractCube::SceneRefractCube() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f),
teapot(14, glm::mat4(1.0f)), sky(30.0f), sphere(2.0, 30, 30)
{ }

void SceneRefractCube::initScene()
{
  compileAndLinkShader();

  glEnable(GL_DEPTH_TEST);

  projection = mat4(1.0f);

  angle = glm::radians(90.0f);

  // --------------------------------------------------------------------------- //
  // 1. Generate desriptor for cubemap texture.
  // 2. Allocate GPU memory for each of the 6 HDR images, one per face.
  // 3. Load each HDR image from the filesystem and upload them to the GPU.

  // Generate texture descriptor.
  GLuint texID;
  glGenTextures(1, &texID);

  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

  const char* suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };

  GLuint targets[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
  };

  // Load the 1st cube face to memory to obtain width and height.
  GLint w, h;
  std::string baseName = "../media/texture/cube/pisa-hdr/pisa";
  std::string texName0 = baseName + "_"; texName0.append(suffixes[0]); texName0.append(".hdr");
  float* data = stbi_loadf(texName0.c_str(), &w, &h, NULL, 3);

  // Allocate storage for the entire cubemap. Since the target is a cubemap,
  // it knows that it has to allocate 6 timex WxH.
  glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGB32F, w, h);

  // Pass the 1st cube face to the GPU.
  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, w, h, GL_RGB, GL_FLOAT, data);
  stbi_image_free(data);

  // Load the rest of the cube faces.
  for (int i = 1; i < 6; i++) {
    std::string texName = baseName + "_"; texName.append(suffixes[i]); texName.append(".hdr");
    data = stbi_loadf(texName.c_str(), &w, &h, NULL, 3);
    glTexSubImage2D(targets[i], 0, 0, 0, w, h, GL_RGB, GL_FLOAT, data);
    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // --------------------------------------------------------------------------- //
  // 4. ...

  // Select texture unit.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
}

void SceneRefractCube::update(float t)
{
  float deltaT = t - tPrev;
  if (tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += rotSpeed * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneRefractCube::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
  view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

  // Draw sky
  skyProg.use();
  // GLM constructs a diagonal matrix when only given one
  // argument. In this case, an identity matrix (1s on
  // diagonal, 0s elsewhere).
  model = mat4(1.0f);
  setMatrices(skyProg);
  sky.render();

  // Draw scene
  prog.use();
  prog.setUniform("WorldCameraPosition", cameraPos);
  prog.setUniform("Material.Eta", 1.0f);
  prog.setUniform("Material.ReflectionFactor", 0.1f);

  model = mat4(1.0f);
  model = glm::translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
  setMatrices(prog);
  sphere.render();
}

void SceneRefractCube::setMatrices(GLSLProgram& p)
{
  mat4 mv = view * model;
  p.setUniform("ModelMatrix", model);
  p.setUniform("ModelViewMatrix", mv);
  p.setUniform("MVP", projection * mv);
}

void SceneRefractCube::resize(int w, int h)
{
  glViewport(0, 0, w, h);
  width = w;
  height = h;
  projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneRefractCube::compileAndLinkShader()
{
  try {
    prog.compileShader("shader/cubemap_refract.my.vs");
    prog.compileShader("shader/cubemap_refract.my.fs");

    skyProg.compileShader("shader/skybox.my.vs");
    skyProg.compileShader("shader/skybox.my.fs");
    skyProg.link();
    prog.link();
    prog.use();
  }
  catch (GLSLProgramException& e) {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }
}

//void SceneRefractCube::debug() {
//  skyProg.printActiveUniforms();
//}