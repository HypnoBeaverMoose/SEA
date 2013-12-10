#include <iostream>
#include <jni.h>
#include <sstream>

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
    setCurrentIndex(portraitIdx);
    portGUI.playMusic();

    QObject::connect( genGUI.getGUISwitchBtn(), SIGNAL(clicked()), this, SLOT(toggleGUI()) );
    QObject::connect( portGUI.getGUISwitchBtn(), SIGNAL(clicked()), this, SLOT(toggleGUI()) );

    buttonPlayer.setMedia( QUrl("assets:/SE-button.wav") );
}

SwitchWindow::~SwitchWindow()
{
    delete ui;
}

void SwitchWindow::AfterShownSetVariables()
{
    portGUI.AfterShownSetVariables();
}

void SwitchWindow::toggleGUI()
{
    buttonPlayer.stop();
    buttonPlayer.setVolume(100);
    buttonPlayer.play();
    if ( currentIndex() == plantGenIdx )
    {
        setCurrentIndex(portraitIdx);
        //portGUI.PlayMovies();
        genGUI.stopMusic();
        portGUI.playMusic();
    }
    else
    {
        setCurrentIndex(plantGenIdx);
        portGUI.stopMusic();
        genGUI.playMusic();
    }

}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_qtproject_qt5_android_bindings_NFCStation_processPlants( JNIEnv * env, jobject obj, int p1, int p2, int p3 )
    {
        std::stringstream ss;
        ss << "Received plants " << p1 << ", " << p2 << " and " << p3;
        if(p1 > 0 && p2 > 0 && p3 > 0)
            PlantGenGUI::pGUI->getPlants(p1, p2, p3);

        PlantGenGUI::pGUI->setTestLabelText( ss.str() );
    }

    JNIEXPORT void JNICALL Java_org_qtproject_qt5_android_bindings_NFCStation_setLabelText( JNIEnv * env, jobject obj, jstring text )
    {
        const char *cText = env->GetStringUTFChars(text, false);
        PlantGenGUI::pGUI->setTestLabelText( cText );
    }
}
