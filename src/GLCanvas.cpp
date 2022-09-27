#include "GLCanvas.hpp"
#include "GLContextLinux.hpp"

#include "AWTSurface.hpp"

#include "linmath.h"
#include <iostream>

typedef struct Vertex {
    vec2 pos;
    vec3 col;
} Vertex;

static const Vertex vertices[3] =
        {
                {{-0.6f, -0.4f}, {1.f, 0.f, 0.f}},
                {{0.6f, -0.4f}, {0.f, 1.f, 0.f}},
                {{0.f, 0.6f}, {0.f, 0.f, 1.f}}};

static const char *vertex_shader_text =
        "#version 330\n"
        "uniform mat4 MVP;\n"
        "in vec3 vCol;\n"
        "in vec2 vPos;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char *fragment_shader_text =
        "#version 330\n"
        "in vec3 color;\n"
        "out vec4 fragment;\n"
        "void main()\n"
        "{\n"
        "    fragment = vec4(color, 1.0);\n"
        "}\n";

static GLuint program;
static GLint mvp_location;
static GLuint vertex_array;

JNIEXPORT jlong JNICALL Java_GLCanvas_createContext(JNIEnv *env, jobject canvas) {
    AWTSurface surface(env, canvas);
    JAWT_PlatformInfo dsi_x11 = surface.platformInfo();

    GLContextRef context = GLContext::Alloc();
    context->display = dsi_x11->display;
    context->drawable = dsi_x11->drawable;

    GLContext::Create(context);
    GLContext::Bind(context);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3,
            GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, col));

    GLContext::Unbind(context);
    return reinterpret_cast<jlong>(context);
}

JNIEXPORT void JNICALL Java_GLCanvas_paint(JNIEnv *env, jobject canvas, jlong _context) {
    static float theta = 0.0f;

    AWTSurface surface(env, canvas);
    JAWT_PlatformInfo dsi_x11 = surface.platformInfo();

    Display *display = dsi_x11->display;
    Drawable drawable = dsi_x11->drawable;

    auto context = reinterpret_cast<GLContextRef>(_context);
    GLContext::Bind(context);

    XWindowAttributes gwa;
    XGetWindowAttributes(display, drawable, &gwa);
    int width = gwa.width;
    int height = gwa.height;

    const float ratio = width / (float) height;

    glViewport(0, 0, width, height);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 m, p, mvp;
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, theta);
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *) &mvp);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glXSwapBuffers(display, drawable);
    GLContext::Unbind(context);

    theta += .01f;
}
