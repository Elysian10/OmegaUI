// cxx/src/glfw_window.cc
#include "window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
// #include "include/core/SkCanvas.h"

class GLFWWindow : public Window {
private:
    GLFWwindow* window;
    KeyCallback key_callback;
    CursorPosCallback cursor_pos_callback;
    MouseButtonCallback mouse_button_callback;
    
    // Static methods for GLFW callbacks
    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        if (self && self->key_callback) {
            // Get the key name (may be nullptr for some keys)
            const char* key_name_ptr = glfwGetKeyName(key, scancode);
            std::string key_name = key_name_ptr ? key_name_ptr : "unknown";
            
            self->key_callback(key, scancode, action, mods, key_name);
        }
    }
    
    static void glfw_cursor_pos_callback(GLFWwindow* window, double x, double y) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        if (self && self->cursor_pos_callback) {
            self->cursor_pos_callback(x, y);
        }
    }

    static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        GLFWWindow* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        if (self && self->mouse_button_callback) {
            self->mouse_button_callback(button, action, mods);
        }
    }
    
public:
    GLFWWindow(int width, int height, const char* title) : window(nullptr) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        
        
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        
        // Set user pointer to this instance for static callbacks
        glfwSetWindowUserPointer(window, this);
        
        // Set up GLFW callbacks
        glfwSetKeyCallback(window, glfw_key_callback);
        glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
        glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
        
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
    
    void set_key_callback(KeyCallback callback) override {
        key_callback = std::move(callback);
    }
    
    void set_cursor_pos_callback(CursorPosCallback callback) override {
        cursor_pos_callback = std::move(callback);
    }

    void set_mouse_button_callback(MouseButtonCallback callback) override {
        mouse_button_callback = std::move(callback);
    }
};

std::unique_ptr<Window> create_glfw_window(int width, int height, const char* title) {
    return std::make_unique<GLFWWindow>(width, height, title);
}