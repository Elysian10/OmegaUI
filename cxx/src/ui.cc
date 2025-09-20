// cxx/src/ui.cc
#include "ui.h"
#include "window.h"
#include <memory>
#include <cstdint>
#include <iostream>
#include "omega/src/main.rs.h"

UI::UI(std::unique_ptr<Window> window) : window(std::move(window)) {}

UI::~UI() = default;

bool UI::should_close() const {
    return window->should_close();
}

void UI::poll_events() {
    window->poll_events();
}

void UI::swap_buffers() {
    window->swap_buffers();
}


std::unique_ptr<UI> new_ui() {
    auto window = create_glfw_window(800, 600, "Omega Window");
    uint32_t result = my_rust_function(10);
       std::cout << "Result from Rust function: " << result << std::endl;
    return std::make_unique<UI>(std::move(window));
}