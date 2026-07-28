#pragma once

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Pegas {

class Engine {
   public:
    Engine(GLFWwindow* window);
    ~Engine();

    void InitShaders(const std::string& vertexSource, const std::string& fragmentSource);
    void InitProjection(float width, float height);

    void DrawCircle(glm::vec2 center, float radius, int segments);

   private:
    std::string ParseShaderFromFile(const std::string& filePath) const;
    GLuint CompileShader(GLenum shaderType, const std::string& source) const;

    std::vector<glm::vec3> GenerateCircleVertices(glm::vec2 center, float radius,
                                                  int segments) const;

   private:
    GLFWwindow* m_MainWindow = nullptr;

    glm::mat4 m_ProjectionMatrix{1.0f};
    GLuint m_ShaderProgram = 0;
};

}  // namespace Pegas
