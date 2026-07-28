#include <glad/glad.h>

#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#include "DrawEngine.h"

Pegas::Engine::Engine(GLFWwindow* window) : m_MainWindow(window) {}
Pegas::Engine::~Engine() { glDeleteProgram(m_ShaderProgram); }

void Pegas::Engine::InitShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    auto vertexSource = ParseShaderFromFile(vertexPath);
    auto fragmentSource = ParseShaderFromFile(fragmentPath);
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) return;

    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return;
    }

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);

    glLinkProgram(m_ShaderProgram);

    GLint success;
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";

        glDeleteProgram(m_ShaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Pegas::Engine::InitProjection(float width, float height) {
    m_ProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
}

std::string Pegas::Engine::ParseShaderFromFile(const std::string& filePath) const {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << "\n";
        return "";
    }
}

GLuint Pegas::Engine::CompileShader(GLenum shaderType, const std::string& source) const {
    GLuint shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);

    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        std::string typeStr = (shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cerr << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << "\n";

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
void Pegas::Engine::DrawCircle(glm::vec2 center, float radius, int segments) {
    GLuint VAO = 0, VBO = 0;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    std::vector<glm::vec3> vertices = GenerateCircleVertices(center, radius, segments);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(float), vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glUseProgram(m_ShaderProgram);
    GLint projLoc = glGetUniformLocation(m_ShaderProgram, "uProjection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
}

std::vector<glm::vec3> Pegas::Engine::GenerateCircleVertices(glm::vec2 center, float radius,
                                                             int segments) const {
    std::vector<glm::vec3> vertices;

    vertices.reserve(segments + 2);

    vertices.emplace_back(center.x, center.y, 0.0f);

    for (int i = 0; i <= segments; ++i) {
        float theta =
            2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(segments);
        float x = center.x + radius * glm::cos(theta);
        float y = center.y + radius * glm::sin(theta);

        vertices.emplace_back(x, y, 0.0f);
    }

    return vertices;
}
