#include <iostream>
#include <qpixmap.h>
#include <qpainter.h>

#include "MaskedButton.h"

MaskedButton::MaskedButton(QWidget *parent) :
    QPushButton(parent), upImg(), downImg(), btnSize(0, 0)
{
	this->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	this->setMinimumSize( btnSize );
	this->setMaximumSize( btnSize );
}


QSize MaskedButton::getSize() const
{
	return btnSize;
}


void MaskedButton::setSize( QSize newSize )
{
	btnSize = newSize;
	this->setMinimumSize( btnSize );
	this->setMaximumSize( btnSize );

	repaint();
}


void MaskedButton::setImages( QImage *up, QImage *down )
{
	upImg   = QPixmap::fromImage(*up);
	downImg = QPixmap::fromImage(*down);
	btnSize = upImg.size();

	repaint();
}


void MaskedButton::paintEvent(QPaintEvent *)
{
	if ( upImg.isNull() || downImg.isNull() )
		return;

	QPainter p(this);
	if ( !this->isDown() )
		p.drawPixmap( 0, 0, upImg.scaled(btnSize) );
	else p.drawPixmap( 0, 0, downImg.scaled(btnSize) );
}


bool MaskedButton::hitButton( const QPoint & ) const
{
	//TODO: add masking code here...

    //std::cout << "button hit" << std::endl;

	return true;
}
