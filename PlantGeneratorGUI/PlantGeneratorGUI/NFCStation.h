#ifndef NFCSTATION_H
#define NFCSTATION_H

#include <jni.h>

class NFCStation
{
    public:
        NFCStation();
        //~NFCStation();
        void openReader();

    private:
        jobject nfcObject;
};

#endif // NFCSTATION_H
