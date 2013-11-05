
#include <iostream>

#include "FutureGUI.h"

FutureGUI::FutureGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	std::cout << "UI setup" << std::endl;

	QSize arrowBtnSize(55, 38);
	ui.specialBtnL->setSize( arrowBtnSize );
	ui.specialBtnR->setSize( arrowBtnSize );
	ui.leafBtnL->setSize( arrowBtnSize );
	ui.leafBtnR->setSize( arrowBtnSize );
	ui.stalkBtnL->setSize( arrowBtnSize );
	ui.stalkBtnR->setSize( arrowBtnSize );
	ui.rootBtnL->setSize( arrowBtnSize );
	ui.rootBtnR->setSize( arrowBtnSize );

	ui.saveBtn->setSize( QSize(149, 90) );
	QImage saveImg("jugenstilSaveBtn.png");
	QImage saveImgDown("jugenstilSaveBtnDown.png");
	ui.saveBtn->setImages( &saveImg, &saveImgDown );

	QImage btnImageL("jugenstilArrowLeft.png");
	QImage btnImageLDown("jugenstilArrowLeftDown.png");
	QImage btnImageR("jugenstilArrowRight.png");
	QImage btnImageRDown("jugenstilArrowRightDown.png");
	ui.specialBtnL->setImages( &btnImageL, &btnImageLDown );
	ui.specialBtnR->setImages( &btnImageR, &btnImageRDown );
	ui.leafBtnL->setImages( &btnImageL, &btnImageLDown );
	ui.leafBtnR->setImages( &btnImageR, &btnImageRDown );
	ui.stalkBtnL->setImages( &btnImageL, &btnImageLDown );
	ui.stalkBtnR->setImages( &btnImageR, &btnImageRDown );
	ui.rootBtnL->setImages( &btnImageL, &btnImageLDown );
	ui.rootBtnR->setImages( &btnImageR, &btnImageRDown );
}

FutureGUI::~FutureGUI()
{
}
