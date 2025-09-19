#include "ui.h"
#include <memory>


UI::UI() {

}


UI::~UI() {
}

std::unique_ptr<UI> new_ui() {
    return std::make_unique<UI>();
}