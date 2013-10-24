#pragma once
#include "App.h"
class AppAndroid : public App
{
public:
	AppAndroid(void);
	~AppAndroid(void);

public:
	static void CrateCallback();
	static void RenderCallback();
	static void ResizeCallback(int width, int height);
	static void DestroyCallback();
	static AppAndroid* s_instance;
};
//
//extern "C" 
//{	
//    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height);
//    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj);
//	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj);
//	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj);
//};