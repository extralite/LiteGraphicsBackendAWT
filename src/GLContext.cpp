#include "GLContext.hpp"

#include <cstdlib>

GLContextRef GLContext::Alloc() {
    return static_cast<GLContextRef>(malloc(sizeof(GLContextRef)));
}

void GLContext::Free(GLContextRef ctx) {
    free(ctx);
}
