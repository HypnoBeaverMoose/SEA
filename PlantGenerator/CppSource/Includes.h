#pragma once

#ifdef ANDROID

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#endif

#ifdef _WIN32
#include "glew.h"
#include<windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>

#endif

#include<cmath>
#include<vector>

typedef unsigned int uint;