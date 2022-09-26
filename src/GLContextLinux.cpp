#include "GLContextLinux.hpp"

#include <iostream>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

void GLContext::Create(GLContextRef ctx) {
    if (ctx == nullptr)
        return;

    ctx->_ptr = GL_NULL_CONTEXT;

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
            glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");

    if (glXCreateContextAttribsARB == NULL) {
        std::cout << "glXCreateContextAttribsARB() not found" << std::endl;
        return;
    }

    static int visual_attribs[] = {
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_DOUBLEBUFFER, true,
            GLX_DEPTH_SIZE, 24,
            None
    };

    int fbcount;
    int screen = DefaultScreen(ctx->display);
    GLXFBConfig *fbc = glXChooseFBConfig(ctx->display, screen, visual_attribs, &fbcount);

    GLint context_attributes[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
    };

    GLXContext context =
            glXCreateContextAttribsARB(ctx->display, fbc[0], NULL, 1, context_attributes);
    if (!context) {
        std::cerr << "Unable to create OpenGL context" << std::endl;
        return;
    }

    ctx->_ptr = context;
    if (!Bind(ctx)) {
        ctx->_ptr = GL_NULL_CONTEXT;
        std::cerr << "Unable to make GLX context current" << std::endl;
        return;
    }

    // TODO: Replace with LiteGraphics abstraction
    int gl_version = gladLoadGL((GLADloadfunc) glXGetProcAddressARB);
    if (!gl_version) {
        std::cerr << "Unable to load OpenGL" << std::endl;
        return;
    }

    Unbind(ctx);
}

int GLContext::Bind(GLContextRef ctx) {
    return glXMakeCurrent(ctx->display, ctx->drawable, ctx->_ptr);
}

int GLContext::Unbind(GLContextRef ctx) {
    return glXMakeCurrent(ctx->display, GL_NULL_CONTEXT, GL_NULL_CONTEXT);
}
