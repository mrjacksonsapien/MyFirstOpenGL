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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Tell glfw I'm using OpenGL core profile

    GLFWwindow* window = glfwCreateWindow(800, 800, "MyFirstOpenGL", nullptr, nullptr);
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
        // Position         // Color
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // Create vertex array and buffers
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // VAO for vertex attributes
    glGenBuffers(1, &VBO); // VBO for vertices data
    glGenBuffers(1, &EBO); // EBO for elements (triangles)

    // Bind buffers so subsequent calls affect them (state machine)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // VAO remembers this EBO
    
    // VBO data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Static draw for now since data is not updated

    // EBO data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // VAO data. Attributes reference the currently binded VBO at creation.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0); // Pass id used as first argument during. Enables the data stream of that attribute.

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);

    // Create shader program. Vertex runs for each vertex and fragment runs for each pixel/fragment
    unsigned int shaderProgram = createShaderProgram("../shaders/vertex.glsl", "../shaders/fragment.glsl");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Tell that you want to clear the color buffer (the screen)

        glUseProgram(shaderProgram); // Activate the shader program
        glBindVertexArray(VAO); // Automatically binds it's EBO and the attributes for rendering
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw EBO

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}