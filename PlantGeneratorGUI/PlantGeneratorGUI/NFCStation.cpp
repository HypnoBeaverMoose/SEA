
#include <iostream>

#include "NFCStation.h"
#include "PlantGenGUI.h"

static JavaVM *javaVM = 0;
static jclass nfcClassID = 0;
static jmethodID nfcContstructorMethodID = 0;
static jmethodID nfcOpenReaderMethodID = 0;

NFCStation::NFCStation()
{
    JNIEnv* env;
    // Qt is running in a different thread than Java UI, so you always Java VM *MUST* be attached to current thread
    if ( javaVM->AttachCurrentThread(&env, NULL) < 0 )
    {
        std::cout  << "AttachCurrentThread failed" << std::endl;
        return;
    }

    nfcObject = env->NewGlobalRef( env->NewObject(nfcClassID, nfcContstructorMethodID) );
    if ( !nfcObject )
    {
        std::cout << "Can't create HelloJavaWorld object!" << std::endl;
        return;
    }

    // Don't forget to detach from current thread
    javaVM->DetachCurrentThread();
}


/*NFCStation::~NFCStation()
{
}*/


void NFCStation::openReader()
{
    if ( !nfcObject )
        return;

    JNIEnv* env;
    if (javaVM->AttachCurrentThread(&env, NULL) < 0)
    {
        std::cout << "AttachCurrentThread failed" << std::endl;
        return;
    }

    env->CallVoidMethod( nfcObject, nfcOpenReaderMethodID );
    javaVM->DetachCurrentThread();
}


/*static JNINativeMethod jniMethods[] = {
    {"processPlants", "(III)V", (void *)processPlants}
};*/


// this method is called immediately after the module is load
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        std::cout << "Can't get the environment" << std::endl;
        return -1;
    }

    javaVM = vm;
    // search for our class
    jclass clazz=env->FindClass("org/qtproject/qt5/android/bindings/NFCStation");
    if (!clazz)
    {
        std::cout << "Can't find NFCStation class" << std::endl;
        return -1;
    }
    // keep a global reference to it
    nfcClassID = (jclass)env->NewGlobalRef(clazz);

    // search for its contructor
    nfcContstructorMethodID = env->GetMethodID(nfcClassID, "<init>", "()V");
    if (!nfcContstructorMethodID)
    {
        std::cout << "Can't find NFCStation class contructor" << std::endl;
        return -1;
    }

    // search for deviceCallback method
    nfcOpenReaderMethodID = env->GetMethodID(nfcClassID, "OpenReader", "()V");
    if (!nfcOpenReaderMethodID)
    {
        std::cout << "Can't find OpenReader method" << std::endl;
        return -1;
    }


    // register our native methods
    /*if ( env->RegisterNatives(nfcClassID, jniMethods, 1) < 0 )
    {
        std::cout << "RegisterNatives failed" << std::endl;
        return -1;
    }*/


    std::cout << "JNI successfully loaded!" << std::endl;
    return JNI_VERSION_1_6;
}
