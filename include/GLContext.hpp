#pragma once

struct _ContextRef;
using GLContextRef = struct _ContextRef *;

#define GL_NULL_CONTEXT 0L

class GLContext {
public:
    static GLContextRef Alloc();

    static void Free(GLContextRef ctx);

    static void Create(GLContextRef ctx);

    static int Bind(GLContextRef ctx);

    static int Unbind(GLContextRef ctx);
};
