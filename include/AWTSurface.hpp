#pragma once

#include <jawt_md.h>

#ifdef __linux__
typedef JAWT_X11DrawingSurfaceInfo *JAWT_PlatformInfo;
#endif

#define AWT_GET_INSTANCE_ERROR -1
#define AWT_SURFACE_LOCK_ERROR -2
#define CANVAS_NULL_DRAWING_SURFACE_ERROR -3
#define CANVAS_NULL_DRAWING_SURFACE_INFO_ERROR -4

class AWTSurface {
public:
    AWTSurface(JNIEnv *env, jobject canvas);

    virtual ~AWTSurface();

    JAWT_PlatformInfo platformInfo() const {
        return static_cast<JAWT_PlatformInfo>(dsi->platformInfo);
    }

private:
    JAWT awt;
    JAWT_DrawingSurface *ds;
    JAWT_DrawingSurfaceInfo *dsi;
};
