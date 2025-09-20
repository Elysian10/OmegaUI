// cxx/include/ui.h
#pragma once

#include <memory>

class Window;  // Forward declaration

class UI {
private:
    std::unique_ptr<Window> window;
    
public:
    UI(std::unique_ptr<Window> window);
    ~UI();
    
    // Prevent copying
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;
    
    // Check if window should close
    bool should_close() const;
    
    // Process events and swap buffers
    void poll_events();
    void swap_buffers();
};

std::unique_ptr<UI> new_ui();