#pragma once

#include <glad/gl.h>
#include <GL/glx.h>

struct _ContextRef {
    Display *display;
    Drawable drawable;
    GLXContext _ptr;
};

#include "GLContext.hpp"
