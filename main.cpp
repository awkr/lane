#include <cassert>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#elif defined(WIN32)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#else
#error "unsupported platform"
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, World!", nullptr, nullptr);
    assert(window != nullptr);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    auto gl_version = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    std::cout << "GL version " << GLAD_VERSION_MAJOR(gl_version) << "." << GLAD_VERSION_MINOR(gl_version) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
//    auto &style = ImGui::GetStyle();
//    style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
//    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    bool succeed = ImGui_ImplGlfw_InitForOpenGL(window, true);
    assert(succeed);
    succeed = ImGui_ImplOpenGL3_Init("#version 400");
    assert(succeed);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    std::cout << "framebuffer size: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);  // 设置窗口位置为屏幕坐标 (100, 100)
//        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver); // 设置窗口大小为 400x300 像素
        ImGui::Begin("Hello, World!");
        ImGui::TextWrapped("all human wisdom is summed up in two words - wait and hope.");
        ImGui::End();
//        ImGui::ShowDemoWindow();
        ImGui::Render();

        GLfloat clear_color[4] = {0.5f, 0.4f, 0.5f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, clear_color);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
