#include "scenetexture.h"
#include "texture.h"
#include "stb/stb_image.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneTexture::SceneTexture() : sphere(0.5, 100, 100) { }

void SceneTexture::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(1.0f,1.25f,1.25f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);
    glm::vec4 worldLight = glm::vec4(3.0f, 1.0f, 1.2f, 0.5f);


    angle = 0.0;

    prog.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);

    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);

    prog.setUniform("Light.Position", view * worldLight);
    prog.setUniform("Material.Shininess", 10.0f);

    // Load texture file
	  GLuint texID = Texture::loadTexture("../media/texture/brick1.jpg");
    printf("texID: %d\n", texID);
    glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, texID);

#ifdef __APPLE__
    // Set the sampler uniform
    prog.setUniform("Tex1", texID);
#endif
}

void SceneTexture::update( float t ) { }

void SceneTexture::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = mat4(1.0f);
    setMatrices();
    //cube.render();
    sphere.render();

    //prog.printActiveUniforms();
}

void SceneTexture::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneTexture::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneTexture::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
      prog.compileShader("shader/texture_41.vs");
    	prog.compileShader("shader/texture_41.fs");
#else
    	prog.compileShader("shader/texture.my.vert.glsl");
    	prog.compileShader("shader/texture.my.frag.glsl");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
