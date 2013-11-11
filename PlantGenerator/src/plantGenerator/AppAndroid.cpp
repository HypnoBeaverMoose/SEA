#include "Definitions.h"
#include "AppAndroid.h"

extern "C" 
{	
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr);
    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj);	
};

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height)
{
	AppAndroid::getInstance()->OnResize(width, height);
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj)
{
	AppAndroid::getInstance()->OnRender();
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj)
{	
	LOGI("DestroyCallback");
	AppAndroid::getInstance()->OnDestroy();
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj)
{	
	LOGI("CrateCallback");	
	AppAndroid::getInstance()->OnCreate();

}
JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr)
{
	AppAndroid::getInstance()->m_assetManager = AAssetManager_fromJava(env,mgr);

	if(AppAndroid::getInstance()->m_assetManager ==NULL){
		LOGE("FAILED TO LOAD ASSET MANAGER");	
		return;
	}
	AAsset* asset = AAssetManager_open(AppAndroid::getInstance()->m_assetManager,"test.txt",AASSET_MODE_STREAMING);
	
	if(asset == NULL){
		LOGE("ASSET MANAGER FAIL: FAILED TO LOAD INITAL ASSET");	
		return;
	}

	off_t  start, length;
	int ret = AAsset_openFileDescriptor(asset,&start, &length);
	if(ret < 0){
		LOGE("ASSET MANAGER FAIL: FILE DESCRIPTOR IS NOT OPEN!");	
		AAsset_close(asset);
		return;
	}
	
	char* string  = new char[length + 1];	
	ret = AAsset_read(asset,string,length);
	if(ret !=length){
		if(ret == 0 ) 
			LOGE("ASSET MANAGER FAIL: EOF ENCOUNTERED");	
		else  if(ret < 0)
			LOGE("ASSET MANAGER FAIL: READ ERROR");		
		AAsset_close(asset);
		return;
	}
	string[length] = '\0';
	
	LOGI("%s",string);
	AAsset_close(asset);	
	delete string;
}

AppAndroid* AppAndroid::s_instance = NULL;
void AppAndroid::CrateCallback()
{
	getInstance()->OnCreate();
}
void AppAndroid::RenderCallback()
{
	getInstance()->OnRender();
}

void AppAndroid::ResizeCallback(int width, int height)
{
	getInstance()->OnResize(width, height);
}

void AppAndroid::DestroyCallback()
{
	getInstance()->OnDestroy();
}

AppAndroid* AppAndroid::getInstance()
{
	if(s_instance == NULL)
		s_instance = new AppAndroid();
	return s_instance;
}
AppAndroid::AppAndroid(void)
{
}


AppAndroid::~AppAndroid(void)
{

}
