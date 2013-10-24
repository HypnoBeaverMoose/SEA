#pragma once
//#include"Includes.h"

//#ifdef ANDROID

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//#else

//#include "glew.h"
//#include<windows.h>
//#include<gl/GL.h>
//#include<gl/GLU.h>
//
//#endif

#include<cmath>
#include<vector>
#include<map>
#include<iostream>
#include<string>
#include<vector>
#include<memory>

typedef unsigned int uint;

#define  LOG_TAG    "plantgen"

//#ifdef ANDROID
	#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
	#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
//#endif

//#ifdef WIN32
//
//inline void log(const char* format, ...)
//{		
//	char buf[1024];
//	wvsprintfA(buf, format, ((char*)&format) + sizeof(void*));
//	OutputDebugStringA(buf);
//}
//
//#define  LOGI(format, ...)	log(format, __VA_ARGS__)
//#define  LOGE(format, ...)  log(format, __VA_ARGS__)
//#endif

inline void printGLString(const char *name, GLenum s) 
{
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

inline  void checkGlError(const char* op) 
{
    for (GLint error = glGetError(); error; error = glGetError()) 
	{
        LOGE("after %s() glError (0x%x)\n", op, error);
    }
}
 

template<class T>
class Base
{
public:
	virtual T* getValuePtr() = 0;
	//const T* getValue() const = 0;
};

#define DEG2RAD 0.0174532925