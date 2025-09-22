//cxx/include/window.h
#pragma once

#include <memory>
#include <functional>

class Window {
public:
    virtual ~Window() = default;
    
    virtual bool should_close() const = 0;
    virtual void poll_events() = 0;
    virtual void swap_buffers() = 0;
    
    virtual void make_context_current() = 0;
    virtual int get_width() const = 0;
    virtual int get_height() const = 0;

    // Callback types
    using KeyCallback = std::function<void(int key, int scancode, int action, int mods, const std::string& key_name)>;
    using CursorPosCallback = std::function<void(double x, double y)>;
    using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
    
    virtual void set_key_callback(KeyCallback callback) = 0;
    virtual void set_cursor_pos_callback(CursorPosCallback callback) = 0;
    virtual void set_mouse_button_callback(MouseButtonCallback callback) = 0;
};

std::unique_ptr<Window> create_glfw_window(int width, int height, const char* title);