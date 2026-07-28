#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include "DrawEngine.h"

const char* VERTEX_SHADER_SRC = "C:\\dev\\opengl\\GravitySimulator\\res\\shaders\\def.vert";
const char* FRAGMENT_SHADER_SRC = "C:\\dev\\opengl\\GravitySimulator\\res\\shaders\\def.frag";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

bool init(GLFWwindow** window);

int main() {
    GLFWwindow* pWindow = nullptr;
    if (!init(&pWindow)) return -1;

    Pegas::Engine engine(pWindow);
    engine.InitShaders(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    engine.InitProjection(800, 800);

    while (!glfwWindowShouldClose(pWindow)) {
        processInput(pWindow);

        glClearColor(0.36f, 0.29f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        engine.DrawCircle(glm::vec2(100, 100), 20.0f, 10);

        glfwPollEvents();
        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();
    return 0;
}

bool init(GLFWwindow** window) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(800, 800, "Graviry Simulator", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    return true;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwWindowShouldClose(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
