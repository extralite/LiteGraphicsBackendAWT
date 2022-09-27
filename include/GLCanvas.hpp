#pragma once

#include <jni.h>

extern "C" {

JNIEXPORT jlong JNICALL
Java_GLCanvas_createContext(JNIEnv *env, jobject canvas);

JNIEXPORT void JNICALL
Java_GLCanvas_paint(JNIEnv *env, jobject canvas, jlong context);
}
