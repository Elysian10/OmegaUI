// cxx/src/skia_graphics.cc
#define SK_GL
#define SK_GANESH

#include "graphics.h"
#include "skia/core/SkSurface.h"
#include "skia/core/SkCanvas.h"
#include "skia/core/SkPaint.h"
#include "skia/core/SkColor.h"
#include "skia/gpu/ganesh/GrDirectContext.h"
#include "skia/gpu/ganesh/gl/GrGLInterface.h"
#include "skia/gpu/ganesh/SkSurfaceGanesh.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

class SkiaGraphicsContext : public GraphicsContext {
private:
    sk_sp<GrDirectContext> grContext;
    sk_sp<SkSurface> surface;
    int currentWidth;
    int currentHeight;
    
public:
    SkiaGraphicsContext() : currentWidth(0), currentHeight(0) {
        // Get the default GL interface (uses the current GL context)
        sk_sp<const GrGLInterface> interface = GrGLInterface::MakeNative();
        if (!interface) {
            std::cerr << "Failed to create native GL interface. Falling back to MakeDefault." << std::endl;
            interface = GrGLInterfaces::MakeDefault();
        }
        
        // Create the GPU context using the modern function
        grContext = GrDirectContexts::MakeGL(interface);
        
        if (!grContext) {
            std::cerr << "Failed to create Skia GPU context. Graphics will not work." << std::endl;
        }
    }
    
    ~SkiaGraphicsContext() override {
        surface = nullptr;
        if (grContext) {
            grContext->releaseResourcesAndAbandonContext();
        }
        grContext = nullptr;
    }
    
    void begin_frame(int width, int height) override {
        if (!grContext) return;
        
        if (width != currentWidth || height != currentHeight || !surface) {
            // Configure the backend render target for the default framebuffer (0)
            GrGLFramebufferInfo framebufferInfo;
            framebufferInfo.fFBOID = 0; // Default framebuffer
            framebufferInfo.fFormat = GL_RGBA8; // Standard format
            
            GrBackendRenderTarget backendRenderTarget(
                width, height, 
                0, // sample count (no MSAA)
                8, // stencil bits
                framebufferInfo
            );
            
            SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag, 
                                 SkPixelGeometry::kRGB_H_SkPixelGeometry);
            
            // Create a surface from the default framebuffer
            surface = SkSurfaces::WrapBackendRenderTarget(
                grContext.get(), 
                backendRenderTarget, 
                kBottomLeft_GrSurfaceOrigin,
                kRGBA_8888_SkColorType,
                nullptr, // color space
                &props
            );
            
            if (!surface) {
                std::cerr << "Failed to create Skia surface for framebuffer" << std::endl;
                return;
            }
            
            currentWidth = width;
            currentHeight = height;
        }
        
        // Clear any GL errors from before we started
        while (glGetError() != GL_NO_ERROR);
    }
    
    void end_frame() override {
        if (surface && grContext) {
            surface->flushAndSubmit();
            grContext->submit(true); // Force a submit to the GPU
        }
    }
    
    void clear(uint32_t color) override {
        if (!surface) return;
        
        auto canvas = surface->getCanvas();
        SkPaint paint;
        paint.setColor(color);
        paint.setStyle(SkPaint::kFill_Style);
        
        canvas->drawPaint(paint);
    }
    
    void draw_rect(float x, float y, float width, float height, uint32_t color) override {
        if (!surface) return;
        
        auto canvas = surface->getCanvas();
        SkPaint paint;
        paint.setColor(color);
        paint.setStyle(SkPaint::kFill_Style);
        paint.setAntiAlias(true);
        
        SkRect rect = SkRect::MakeXYWH(x, y, width, height);
        canvas->drawRect(rect, paint);
    }
    
    void draw_circle(float center_x, float center_y, float radius, uint32_t color) override {
        if (!surface) return;
        
        auto canvas = surface->getCanvas();
        SkPaint paint;
        paint.setColor(color);
        paint.setStyle(SkPaint::kFill_Style);
        paint.setAntiAlias(true);
        
        canvas->drawCircle(center_x, center_y, radius, paint);
    }
    
    void draw_line(float x1, float y1, float x2, float y2, float width, uint32_t color) override {
        if (!surface) return;
        
        auto canvas = surface->getCanvas();
        SkPaint paint;
        paint.setColor(color);
        paint.setStyle(SkPaint::kStroke_Style);
        paint.setStrokeWidth(width);
        paint.setAntiAlias(true);
        paint.setStrokeCap(SkPaint::kRound_Cap);
        
        canvas->drawLine(x1, y1, x2, y2, paint);
    }
    
    void set_transform(float matrix[16]) override {
        if (!surface) return;
        
        auto canvas = surface->getCanvas();
        SkMatrix skMatrix;
        skMatrix.setAll(
            matrix[0], matrix[4], matrix[12],
            matrix[1], matrix[5], matrix[13],
            matrix[3], matrix[7], matrix[15]
        );
        canvas->setMatrix(skMatrix);
    }
    
    void reset_transform() override {
        if (!surface) return;
        
        auto canvas = surface->getCanvas();
        canvas->resetMatrix();
    }
};

std::unique_ptr<GraphicsContext> create_skia_graphics_context() {
    return std::make_unique<SkiaGraphicsContext>();
}