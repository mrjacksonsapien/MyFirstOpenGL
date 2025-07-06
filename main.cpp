#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::string loadShaderSource(const char *path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int compileShader(unsigned int type, const char *src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << "Shader compilation error:\n" << log << std::endl;
    }

    return id;
}

unsigned int createShaderProgram(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath.c_str());
    std::string fragmentCode = loadShaderSource(fragmentPath.c_str());

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Check for linking errors
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Shader program linking error:\n" << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Tell which version of OpenGL I'm using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Tell glfw I'm using OpenGL

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Make window the current context for rendering
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Resize viewport when window resizes

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) { // Load OpenGL functions (at runtime)
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
    };

    unsigned int VERTEX_SIZE = 3;

    unsigned int VAO, VBO; // Vertex array object (how to interpret VBO data) and vertex buffer object (data in gpu memory)
    glGenVertexArrays(1, &VAO); // Generate ID
    glGenBuffers(1, &VBO); // Generate ID

    glBindVertexArray(VAO); // Bind VAO/VBO so subsequent calls affect them.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER is the target for VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Upload vertex data to VBO

    glVertexAttribPointer(0, VERTEX_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)0); // Describe how to interpret vertex data layout (VAO)
    glEnableVertexAttribArray(0); // Enable VAO

    // Create shader program. Vertex runs for each vertex and fragment runs for each pixel/fragment
    unsigned int shaderProgram = createShaderProgram("../shaders/vertex.glsl", "../shaders/fragment.glsl");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Tell that you want to clear the color buffer (the screen)

        glUseProgram(shaderProgram); // Activates the shader program
        glBindVertexArray(VAO); // Bind the VAO (which also binds the VBO because it was binded)
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / VERTEX_SIZE); // Draw the triangles

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}