
#ifndef FUTUREGUI_H
#define FUTUREGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_FutureGUI.h"

class FutureGUI : public QMainWindow
{
	Q_OBJECT

public:
	FutureGUI(QWidget *parent = 0);
	~FutureGUI();

private:
	Ui::FutureGUIClass ui;
};

#endif // FUTUREGUI_H
