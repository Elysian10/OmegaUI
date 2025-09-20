#pragma once

#include <memory>

class Window {
public:
    virtual ~Window() = default;
    
    virtual bool should_close() const = 0;
    virtual void poll_events() = 0;
    virtual void swap_buffers() = 0;
    
    // Add other window methods you need
    virtual void make_context_current() = 0;
    virtual int get_width() const = 0;
    virtual int get_height() const = 0;
};

std::unique_ptr<Window> create_glfw_window(int width, int height, const char* title);