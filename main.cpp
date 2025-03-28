#include <cassert>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/gl.h>

static void error_callback(int error, const char *description) {
    std::cerr << "error: " << description << std::endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "framebuffer size changed: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto result = glfwInit();
    assert(result == GLFW_TRUE);

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
#error "unsupported platform"
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, World!", nullptr, nullptr);
    assert(window != nullptr);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    auto gl_version = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    std::cout << "GL version " << GLAD_VERSION_MAJOR(gl_version) << "." << GLAD_VERSION_MINOR(gl_version) << std::endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    std::cout << "framebuffer size: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
