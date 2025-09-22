// cxx/src/ui.cc
#include "ui.h"
#include "window.h"
#include <memory>
#include <iostream>
#include "omega/src/main.rs.h"

UI::UI(std::unique_ptr<Window> window) : window(std::move(window)) {}

UI::~UI() = default;

bool UI::should_close() const
{
    return window->should_close();
}

void UI::poll_events()
{
    window->poll_events();
}

void UI::swap_buffers()
{
    window->swap_buffers();
}

void UI::set_callbacks()
{
    window->set_key_callback([this](int key, int scancode, int action, int mods, const std::string& key_name) {
        handle_key_event(key, scancode, action, mods, key_name);
    });
    
    window->set_cursor_pos_callback([this](double x, double y) {
        handle_cursor_event(x, y);
    });

    window->set_mouse_button_callback([this](int button, int action, int mods) {
        handle_mouse_button_event(button, action, mods);
    });
}

void UI::run_event_loop()
{
    set_callbacks();
    
    while (!window->should_close())
    {
        window->poll_events();
        window->swap_buffers();
    }
}

std::unique_ptr<UI> new_ui()
{
    auto window = create_glfw_window(800, 600, "Omega Window");
    return std::make_unique<UI>(std::move(window));
}

