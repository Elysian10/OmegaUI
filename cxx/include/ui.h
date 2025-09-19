#include <memory>

class UI {
public:
    UI();
    ~UI();
};

std::unique_ptr<UI> new_ui();
