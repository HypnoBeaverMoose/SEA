#include "Definitions.h"
#include "AppAndroid.h"

extern "C" 
{	
    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height)
{
	LOGI("ResizeCallback");
	AppAndroid::ResizeCallback(width, height);
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj)
{
	LOGI("RenderCallback");
	AppAndroid::RenderCallback();
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj)
{
	LOGI("DestroyCallback");
	AppAndroid::DestroyCallback();
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj)
{
	LOGI("CrateCallback");
	AppAndroid::CrateCallback();
}

AppAndroid* AppAndroid::s_instance = NULL;
void AppAndroid::CrateCallback()
{
	if(s_instance == NULL)
	{
		s_instance = new AppAndroid();
		s_instance->OnCreate();
	}
}
void AppAndroid::RenderCallback()
{
	s_instance->OnRender();
}

void AppAndroid::ResizeCallback(int width, int height)
{
	s_instance->OnResize(width, height);
}

void AppAndroid::DestroyCallback()
{
	s_instance->OnDestroy();
}


AppAndroid::AppAndroid(void)
{
}


AppAndroid::~AppAndroid(void)
{

}
