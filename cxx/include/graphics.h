// Minimal version
#pragma once

#include <memory>
#include <cstdint>

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    
    // Just the drawing commands
    virtual void clear(uint32_t color) = 0;
    virtual void draw_rect(float x, float y, float width, float height, uint32_t color) = 0;
    virtual void draw_circle(float center_x, float center_y, float radius, uint32_t color) = 0;
    virtual void draw_line(float x1, float y1, float x2, float y2, float width, uint32_t color) = 0;
};

std::unique_ptr<GraphicsContext> create_skia_graphics_context();