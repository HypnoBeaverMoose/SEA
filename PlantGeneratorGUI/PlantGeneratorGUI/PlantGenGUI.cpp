
#include <iostream>
#include <sstream>
#include <jni.h>
#include <QFile>
#include <QFontDatabase>

#include "PlantDatabase.h"
#include "PlantGenerator.h"
#include "PlantGenGUI.h"
#include "ui_PlantGenGUI.h"


PlantGenGUI *PlantGenGUI::pGUI = 0;
extern "C" {
    JNIEXPORT void JNICALL Java_org_qtproject_qt5_android_bindings_QtActivity_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr);
}

JNIEXPORT void JNICALL Java_org_qtproject_qt5_android_bindings_QtActivity_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr)
{
    PlantGenerator::setAssetManager(env, mgr);
}

PlantGenGUI::PlantGenGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlantGenGUI), pdb(), plants(), mPlayer(),
    opFxSun(), opFxThorns(), opFxSkull(), opFxNose(), labelLines(0), m_img(0)
{
    PlantGenerator::InitGenerator();
    ui->setupUi(this);

    // load plant label font
    QFile fontFile(":/PlantGen/PRISTINA.TTF");
    if (!fontFile.open(QIODevice::ReadOnly))
        std::cout << "failed to open font file" << std::endl;
    QByteArray fontData = fontFile.readAll();

    if (QFontDatabase::addApplicationFontFromData(fontData) == -1)
        std::cout << "failed to add a font" << std::endl;
    QFont font("Pristina", 18, QFont::Normal);

    ui->plant1Label->setFont(font);
    ui->plant2Label->setFont(font);
    ui->plant3Label->setFont(font);


    ui->guiSwitchBtn->setSize( QSize(122, 122) );
    QImage btnImg;
    if ( !btnImg.load(":/PlantGen/Images/toPortraitBtn.png") )
        std::cout << "Error loading image" << std::endl;

    QImage btnImg_down;
    if ( !btnImg_down.load(":/PlantGen/Images/toPortraitBtn_down.png") )
        std::cout << "Error loading image" << std::endl;
    ui->guiSwitchBtn->setImages( &btnImg, &btnImg_down );


    // connect graphicsEffects to the icon glows
    ui->glowSun->setGraphicsEffect(&opFxSun);
    ui->glowThorns->setGraphicsEffect(&opFxThorns);
    ui->glowSkull->setGraphicsEffect(&opFxSkull);
    ui->glowNose->setGraphicsEffect(&opFxNose);
    ui->glowApple->setGraphicsEffect(&opFxFruit);
    ui->glowToy->setGraphicsEffect(&opFxToy);
    ui->glowTree->setGraphicsEffect(&opFxTree);
    ui->glowRain->setGraphicsEffect(&opFxRain);


    ui->dialFlower->setProperty("plantPart",0);
    ui->dialStalk->setProperty("plantPart",1);
    ui->dialLeaf->setProperty("plantPart",2);

    QObject::connect( ui->dialFlower, SIGNAL(valueChanged(int)), this, SLOT(updateIcons(int)) );
    QObject::connect( ui->dialStalk, SIGNAL(valueChanged(int)), this, SLOT(updateIcons(int)) );
    QObject::connect( ui->dialLeaf, SIGNAL(valueChanged(int)), this, SLOT(updateIcons(int)) );

    QObject::connect( ui->dialFlower, SIGNAL(sliderReleased()), this, SLOT(updatePlantImage()) );
    QObject::connect( ui->dialStalk, SIGNAL(sliderReleased()), this, SLOT(updatePlantImage()) );
    QObject::connect( ui->dialLeaf, SIGNAL(sliderReleased()), this, SLOT(updatePlantImage()) );

   PlantGenerator::loadPlants(PlantDatabase::PlantData(),PlantDatabase::PlantData(),PlantDatabase::PlantData());

    getPlants(66, 67, 68);

    updateIcons(0);

    //setup background music
    mPlayer.setMedia( QUrl("assets:/plantGenMusic.mp3") );
    mPlayer.setVolume(100);

    // set global pointer to plant generator GUI
    pGUI = this;
}

PlantGenGUI::~PlantGenGUI()
{
    delete ui;
}


QPushButton * PlantGenGUI::getGUISwitchBtn()
{
    return ui->guiSwitchBtn;
}


void PlantGenGUI::playMusic()
{
    mPlayer.play();
}


void PlantGenGUI::stopMusic()
{
    mPlayer.stop();
}


void PlantGenGUI::updatePlantImage()
{
    int ability = QObject::sender()->property("plantPart").toInt();
    uint width = ui->imgLabel->width(), height = ui->imgLabel->height();


    ArrowDial *dials[] = { ui->dialFlower, ui->dialStalk, ui->dialLeaf };
    ArrowDial::Area a = dials[ability]->getCurArea();

    int l_index, r_index;
    switch ( a.id )
    {
        case 1:                     // plant 1 and 2
            l_index = 0;
            r_index = 1;
            break;
        case 2:                     // plant 2 and 3
            l_index = 1;
            r_index = 2;
            break;
        case 3:                     // plant 3 and 1
            l_index = 2;
            r_index = 0;
            break;
        default:
            break;
    }


    float bias = std::min(a.left,1.0f);
    PlantGenerator::setCombination(ability, l_index, r_index, 1.0f - bias);

    PlantGenerator::RenderPlant(width, height);

    unsigned char* img = PlantGenerator::getRenderedImage(width, height);

    QImage image(img, width, height, QImage::Format_ARGB32);
    for( int x = 0; x < image.width(); x++){
        for( int y = 0; y < image.width(); y++){
          QRgb  pix = image.pixel(x,y);
          unsigned char* el = (unsigned char*)&pix;
          std::swap(el[0],el[2]);
          image.setPixel(x,y,pix);
        }
     }
    ui->imgLabel->setPixmap(QPixmap::fromImage(image.mirrored()));
}

void PlantGenGUI::setTestLabelText( std::string text )
{
    if ( labelLines >= 6 )
    {
        labelLines = 0;
        ui->testLabel->setText( QString(text.c_str()) );
    } else
    {
        labelLines++;
        ui->testLabel->setText( ui->testLabel->text() + "\n" + QString(text.c_str()) );
    }
}

void PlantGenGUI::getIndexesAndBias(int& l_index, int& r_index, float bias, int ability)
{
    ArrowDial *dials[] = { ui->dialFlower, ui->dialStalk, ui->dialLeaf };

    switch ( dials[ability]->getCurArea().id )
    {
        case 1:                     // plant 1 and 2
            l_index = 0;
            r_index = 1;
            break;
        case 2:                     // plant 2 and 3
            l_index = 1;
            r_index = 2;
            break;
        case 3:                     // plant 3 and 1
            l_index = 2;
            r_index = 0;
        break;
        default:
            break;
    }
    bias  =  dials[ability]->getCurArea().left;

}
void PlantGenGUI::updateIcons( int )
{
    float antiDrought = 0.0f;
    float thorns      = 0.0f;
    float poison      = 0.0f;
    float smell       = 0.0f;
    float fruit       = 0.0f;
    float soft        = 0.0f;
    float growth      = 0.0f;
    float antiwater   = 0.0f;

    ArrowDial *dials[] = { ui->dialFlower, ui->dialStalk, ui->dialLeaf };

    for ( int i = 0; i < PlantDatabase::PlantData::NUM_ABS; ++i )
    {
        ArrowDial::Area a = dials[i]->getCurArea();

        PlantDatabase::PlantData *pLeft;
        PlantDatabase::PlantData *pRight;
        switch ( a.id )
        {
            case 1:                     // plant 1 and 2
                pLeft  = &plants[0];
                pRight = &plants[1];
                break;
            case 2:                     // plant 2 and 3
                pLeft  = &plants[1];
                pRight = &plants[2];
                break;
            case 3:                     // plant 3 and 1
                pLeft  = &plants[2];
                pRight = &plants[0];
                break;
            default:
                break;
        }

        // calculate icon glows
        antiDrought += (pLeft->abs[i].antidrought * a.left) + (pRight->abs[i].antidrought * a.right);
        thorns      += (pLeft->abs[i].thorns * a.left) + (pRight->abs[i].thorns * a.right);
        poison      += (pLeft->abs[i].poison * a.left) + (pRight->abs[i].poison * a.right);
        smell       += (pLeft->abs[i].smell * a.left) + (pRight->abs[i].smell * a.right);
        fruit       += (pLeft->abs[i].fruit * a.left) + (pRight->abs[i].fruit * a.right);
        soft        += (pLeft->abs[i].soft * a.left) + (pRight->abs[i].soft * a.right);
        growth      += (pLeft->abs[i].growth * a.left) + (pRight->abs[i].growth * a.right);
        antiwater   += (pLeft->abs[i].antiwater * a.left) + (pRight->abs[i].antiwater * a.right);
    }

    opFxSun.setOpacity(antiDrought);
    opFxThorns.setOpacity(thorns);
    opFxSkull.setOpacity(poison);
    opFxNose.setOpacity(smell);
    opFxFruit.setOpacity(fruit);
    opFxToy.setOpacity(soft);
    opFxTree.setOpacity(growth);
    opFxRain.setOpacity(antiwater);
}


void PlantGenGUI::getPlants( int p1, int p2, int p3 )
{
    // remove any previously stored plants
    plants.clear();

    bool res = false;
    PlantDatabase::PlantData plant = pdb.getPlant(p1, res);
    if ( res )
        plants.push_back(plant);

    plant = pdb.getPlant(p2, res);
    if ( res )
        plants.push_back(plant);

    plant = pdb.getPlant(p3, res);
    if ( res )
        plants.push_back(plant);

    QLabel *plantNames[]  = { ui->plant1Label, ui->plant2Label, ui->plant3Label };
    QLabel *flowerIcons[] = { ui->flowerPlant1, ui->flowerPlant2, ui->flowerPlant3 };
    QLabel *stalkIcons[] = { ui->stalkPlant1, ui->stalkPlant2, ui->stalkPlant3 };
    QLabel *leafIcons[] = { ui->leafPlant1, ui->leafPlant2, ui->leafPlant3 };

    const std::string ICON_PATH = ":/PlantGen/Images/";
    unsigned int i;
    for ( i = 0; i < plants.size(); ++i )
    {
        plantNames[i]->setText( QString(plants[i].name.c_str()) );

        // flower dial icon
        std::stringstream ssF;
        ssF << ICON_PATH << plants[i].abs[PlantDatabase::PlantData::ABS_FLOWER].img;
        flowerIcons[i]->setPixmap( QPixmap(ssF.str().c_str()) );

        // stalk dial icon
        std::stringstream ssS;
        ssS << ICON_PATH << plants[i].abs[PlantDatabase::PlantData::ABS_STALK].img;
        stalkIcons[i]->setPixmap( QPixmap(ssS.str().c_str()) );

        // leaf dial icon
        std::stringstream ssL;
        ssL << ICON_PATH << plants[i].abs[PlantDatabase::PlantData::ABS_LEAF].img;
        leafIcons[i]->setPixmap( QPixmap(ssL.str().c_str()) );
    }

    if ( plants.size() < 3 )
        for ( i; i < 3; ++i )
            plantNames[i]->setText( "" );
}
