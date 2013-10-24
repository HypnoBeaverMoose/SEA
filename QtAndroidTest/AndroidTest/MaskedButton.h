#ifndef MASKEDBUTTON_H
#define MASKEDBUTTON_H

#include <QPushButton>
#include <qlabel.h>

class MaskedButton : public QPushButton
{
	Q_OBJECT

	public slots:
		void clickedSlot();

	public:
		MaskedButton(QWidget *parent = 0);

		QSize sizeHint() const;
		QSize getSize() const;
		void setSize( QSize newSize );
		void setImages( QImage *up, QImage *down );

	protected:
        void paintEvent(QPaintEvent *);
		bool hitButton( const QPoint &pos ) const;

	private:
		QPixmap upImg;
		QPixmap downImg;
		QSize btnSize;
};

#endif
