
#define _CRT_SECURE_NO_DEPRECATE		// Disable deprecation warnings about freopen()
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define NOMINMAX
#include <windows.h>

#include "FutureGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
					// open console when compiling for debuggin
#ifdef _DEBUG
	AllocConsole();
	AttachConsole( GetCurrentProcessId() );

#pragma warning(suppress: 6031)
	freopen( "CON", "w", stdout );					//redirect stdout to the created console
#endif


	QApplication a(argc, argv);
	FutureGUI w;
	w.show();
	return a.exec();
}
