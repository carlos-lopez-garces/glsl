#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::string vertexShaderSrc = "#version 460\n" \
"layout (location=0) in vec3 VertexPosition;\n" \
"layout (location=1) in vec3 VertexColor;\n" \
"\n" \
"out vec3 Color;\n" \
"\n" \
"uniform mat4 RotationMatrix;"
"\n" \
"void main() {\n" \
"  Color = VertexColor;\n" \
"  gl_Position = RotationMatrix * vec4(VertexPosition, 1.0);\n" \
"}";

std::string fragmentShaderSrc = "#version 460\n" \
"\n" \
"in vec3 Color;\n" \
"layout (location = 0) out vec4 FragColor;\n" \
"\n" \
"void main() {\n" \
"  FragColor = vec4(Color, 1.0);\n" \
"}";

GLuint vaoHandle;

int main() {

  glfwInit();

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if (0 == vertexShader) {
    std::cerr << "Error creating vertex shader." << std::endl;
    exit(EXIT_FAILURE);
  }

  const GLchar *vertexShaderSrcArr[] = { vertexShaderSrc.c_str() };
  glShaderSource(vertexShader, 1, vertexShaderSrcArr, NULL);

  glCompileShader(vertexShader);
  GLint result;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
  if (GL_FALSE == result) {
    std::cerr << "Vertex shader compilation failed!" << std::endl;

    GLint logLen;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      std::string log(logLen, ' ');
      GLsizei written;
      glGetShaderInfoLog(vertexShader, logLen, &written, &log[0]);
      std::cerr << "Shader log: " << std::endl << log;
    }

    exit(EXIT_FAILURE);
  }
  
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if (0 == fragmentShader) {
    std::cerr << "Error creating vertex shader." << std::endl;
    exit(EXIT_FAILURE);
  }

  const GLchar* fragmentShaderSrcArr[] = { fragmentShaderSrc.c_str() };
  glShaderSource(fragmentShader, 1, fragmentShaderSrcArr, NULL);

  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
  if (GL_FALSE == result) {
    std::cerr << "Fragment shader compilation failed!" << std::endl;

    GLint logLen;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      std::string log(logLen, ' ');
      GLsizei written;
      glGetShaderInfoLog(fragmentShader, logLen, &written, &log[0]);
      std::cerr << "Shader log: " << std::endl << log;
    }

    exit(EXIT_FAILURE);
  }

  GLuint program = glCreateProgram();
  if (0 == program) {
    std::cerr << "Error creating program object." << std::endl;
    exit(EXIT_FAILURE);
  }

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    std::cerr << "Failed to link shader program!" << std::endl;
    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      std::string log(logLen, ' ');
      GLsizei written;
      glGetProgramInfoLog(program, logLen, &written, &log[0]);
      std::cerr << "Program log: " << std::endl << log;
    }

    exit(EXIT_FAILURE);
  }

  glUseProgram(program);

  // Once the program is linked, the shaders are not needed anymore.
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float positionData[] = {
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.0f,  0.8f, 0.0f
  };

  float colorData[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };

  GLuint vboHandles[2];
  glGenBuffers(2, vboHandles);
  GLuint positionBufferHandle = vboHandles[0];
  GLuint colorBufferHandle = vboHandles[1];

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    // You don't usually query the location of a uniform or an attribute in the render loop; it's inneficient.
    GLuint location = glGetUniformLocation(program, "RotationMatrix");
    if (location >= 0) {
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    }

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    // Poll IO events (keys pressed/released, mouse moved, etc).
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

// processInput queries GLFW whether relevant keys are pressed/released this frame and 
// react accordingly.
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// framebufferSizeCallback execures whenever the window size changed (by OS or user 
// resize).
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  // Make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}