#include "ui.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

UI::UI() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Configure window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    window = glfwCreateWindow(800, 600, "Omega Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    std::cout << "Window created successfully" << std::endl;
}

UI::~UI() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    std::cout << "Window destroyed" << std::endl;
}

bool UI::should_close() const {
    return glfwWindowShouldClose(window);
}

void UI::poll_events() {
    glfwPollEvents();
}

void UI::swap_buffers() {
    glfwSwapBuffers(window);
}

std::unique_ptr<UI> new_ui() {
    try {
        return std::make_unique<UI>();
    } catch (const std::exception& e) {
        std::cerr << "Error creating UI: " << e.what() << std::endl;
        return nullptr;
    }
}