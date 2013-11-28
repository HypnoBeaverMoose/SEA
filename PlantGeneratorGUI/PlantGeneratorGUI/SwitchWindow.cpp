
#include <iostream>
#include <jni.h>

#include "PlantGenGUI.h"
#include "SwitchWindow.h"
#include "ui_SwitchWindow.h"


SwitchWindow::SwitchWindow(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SwitchWindow), genGUI(this), portGUI(this)
{
    ui->setupUi(this);

    plantGenIdx = this->addWidget(&genGUI);
    portraitIdx = this->addWidget(&portGUI);
    setCurrentIndex(plantGenIdx);

    QObject::connect( genGUI.getGUISwitchBtn(), SIGNAL(clicked()), this, SLOT(toggleGUI()) );
    QObject::connect( portGUI.getGUISwitchBtn(), SIGNAL(clicked()), this, SLOT(toggleGUI()) );
}

SwitchWindow::~SwitchWindow()
{
    delete ui;
}


void SwitchWindow::toggleGUI()
{
    if ( currentIndex() == plantGenIdx )
        setCurrentIndex(portraitIdx);
    else setCurrentIndex(plantGenIdx);
}


extern "C"
{
    JNIEXPORT void JNICALL Java_org_qtproject_qt5_android_bindings_NFCStation_processPlants( JNIEnv * env, jobject obj, int p1, int p2, int p3 )
    {
        PlantGenGUI::pGUI->setTestLabelText( "M'kay!!" );

        std::cout << "Received plants " << p1 << ", " << p2 << " and " << p3 << std::endl;
    }

    JNIEXPORT void JNICALL Java_org_qtproject_qt5_android_bindings_NFCStation_setLabelText( JNIEnv * env, jobject obj, jstring text )
    {
        const char *cText = env->GetStringUTFChars(text, false);
        PlantGenGUI::pGUI->setTestLabelText( cText );
    }
}
