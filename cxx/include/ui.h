// cxx/include/ui.h
#pragma once

#include <memory>

class Window;

class UI {
private:
    std::unique_ptr<Window> window;
    void set_callbacks();
    
public:
    explicit UI(std::unique_ptr<Window> window);
    ~UI();
    
    bool should_close() const;
    void poll_events();
    void swap_buffers();
    void run_event_loop();
    
    // Delete copy constructor and assignment operator
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;
};

std::unique_ptr<UI> new_ui();