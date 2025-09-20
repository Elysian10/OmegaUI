// cxx/src/glfw_window.cc
#include "window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

class GLFWWindow : public Window {
private:
    GLFWwindow* window;
    
public:
    GLFWWindow(int width, int height, const char* title) : window(nullptr) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        
        make_context_current();
        std::cout << "GLFWWindow created successfully" << std::endl;
    }
    
    ~GLFWWindow() override {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
        std::cout << "GLFWWindow destroyed" << std::endl;
    }
    
    bool should_close() const override {
        return glfwWindowShouldClose(window);
    }
    
    void poll_events() override {
        glfwPollEvents();
    }
    
    void swap_buffers() override {
        glfwSwapBuffers(window);
    }
    
    void make_context_current() override {
        glfwMakeContextCurrent(window);
    }
    
    int get_width() const override {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return width;
    }
    
    int get_height() const override {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return height;
    }
};

std::unique_ptr<Window> create_glfw_window(int width, int height, const char* title) {
    return std::make_unique<GLFWWindow>(width, height, title);
}