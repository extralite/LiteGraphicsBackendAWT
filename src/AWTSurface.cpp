#include "AWTSurface.hpp"

#include <iostream>

AWTSurface::AWTSurface(JNIEnv *env, jobject canvas) {
    awt.version = JAWT_VERSION_1_7;
    if (JAWT_GetAWT(env, &awt) == JNI_FALSE) {
        std::cerr << "Error getting AWT instance" << std::endl;
        throw int(AWT_GET_INSTANCE_ERROR);
    }

    ds = awt.GetDrawingSurface(env, canvas);
    if (ds == NULL) {
        std::cerr << "Canvas returned a NULL drawing surface" << std::endl;
        throw int(CANVAS_NULL_DRAWING_SURFACE_ERROR);
    }

    if ((ds->Lock(ds) & JAWT_LOCK_ERROR) != 0) {
        std::cerr << "Error locking Canvas surface" << std::endl;
        awt.FreeDrawingSurface(ds);
        throw int(AWT_SURFACE_LOCK_ERROR);
    }

    dsi = ds->GetDrawingSurfaceInfo(ds);
    if (dsi == NULL) {
        std::cerr << "Canvas returned a NULL drawing surface info" << std::endl;
        ds->Unlock(ds);
        awt.FreeDrawingSurface(ds);
        throw int(CANVAS_NULL_DRAWING_SURFACE_INFO_ERROR);
    }
}

AWTSurface::~AWTSurface() {
    if (ds != NULL) {
        if (dsi != NULL)
            ds->FreeDrawingSurfaceInfo(dsi);

        ds->Unlock(ds);
        awt.FreeDrawingSurface(ds);
    }
}
