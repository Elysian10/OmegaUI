#include <memory>

// Forward declaration
struct GLFWwindow;

class UI {
private:
    GLFWwindow* window;
    
public:
    UI();
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