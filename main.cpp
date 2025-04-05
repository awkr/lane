#include <cassert>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <fstream>
#include <sstream>

std::string load_file_as_string(const char *filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compile_shader(GLenum type, const std::string &source) {
    GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "shader compile error: " << log << std::endl;
        assert(false);
    }

    return shader;
}

GLuint create_shader_program(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode = load_file_as_string(vertexPath);
    std::string fragmentCode = load_file_as_string(fragmentPath);

    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentCode);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "program link error: " << log << std::endl;
        assert(false);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

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

    {
        int num_extensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

        for (int i = 0; i < num_extensions; i++) {
            const char *extension = (const char *) glGetStringi(GL_EXTENSIONS, i);
            std::cout << "extension: " << extension << std::endl;
        }
    }

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

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    unsigned int indices[] = {0, 1, 2};

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint program = create_shader_program("hello.vert", "hello.frag");

    bool is_line_polygon_mode = false;

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);  // 设置窗口位置为屏幕坐标 (100, 100)
//        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver); // 设置窗口大小为 400x300 像素
        ImGui::Begin("Hello, World!");
        ImGui::TextWrapped("all human wisdom is summed up in two words - wait and hope.");
        ImGui::Checkbox("Wireframe", &is_line_polygon_mode);
        ImGui::End();
//        ImGui::ShowDemoWindow();
        ImGui::Render();

        GLfloat clear_color[4] = {0.5f, 0.4f, 0.5f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, clear_color);

        glBindVertexArray(vao);
        double time = glfwGetTime();
        float greenValue = (float) (sin(time) / 2.0f) + 0.5f;
        int vertex_color_location = glGetUniformLocation(program, "color");
        glUseProgram(program);
        glUniform4f(vertex_color_location, 0.0f, greenValue, 0.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, is_line_polygon_mode ? GL_LINE : GL_FILL);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
