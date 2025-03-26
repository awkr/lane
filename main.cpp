#include <cassert>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/gl.h>

void error_callback(int error, const char *description) {
    std::cerr << "error: " << description << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto result = glfwInit();
    assert(result == GLFW_TRUE);

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, World!", nullptr, nullptr);
    assert(window != nullptr);

    glfwMakeContextCurrent(window);

    auto gl_version = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    std::cout << "GL version " << GLAD_VERSION_MAJOR(gl_version) << "." << GLAD_VERSION_MINOR(gl_version) << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
