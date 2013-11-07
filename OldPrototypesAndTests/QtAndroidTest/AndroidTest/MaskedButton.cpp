
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

	QObject::connect( this, SIGNAL(clicked()), this, SLOT(clickedSlot()) );
}


void MaskedButton::clickedSlot()
{
	std::cout << "clickedSlot()" << std::endl;
}


QSize MaskedButton::sizeHint() const
{
    return btnSize;
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

    std::cout << "up->isNull(): " << !up->isNull() << std::endl;
    std::cout << "upImg loaded: " << !upImg.isNull() << std::endl;

	repaint();
}


void MaskedButton::paintEvent(QPaintEvent *)
{
	if ( upImg.isNull() || downImg.isNull() )
    {
        //std::cout << "upImg == NULL" << std::endl;
		return;
    }

	QPainter p(this);
	if ( !this->isDown() )
		p.drawPixmap( 0, 0, upImg.scaled(btnSize) );
	else p.drawPixmap( 0, 0, downImg.scaled(btnSize) );
}


bool MaskedButton::hitButton( const QPoint & ) const
{
	//TODO: add masking code here...

	std::cout << "button hit" << std::endl;

	return true;
}
