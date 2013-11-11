#pragma once
#include "App.h"
class WinApp : public App
{
public:
	bool			CreateGLWindow(char* title, int width, int height);
	virtual void	OnDestroy();
					~WinApp();

public:	
	static  int		InitInstance(int width, int heigth, HINSTANCE instance, char* name);
	static	int		RunMainLoop(int cmdShow);

protected:
	int				MainLoop(int cmdShow);
					WinApp(int width,int heigth,HINSTANCE instance,char* name);	

protected:
	static	WinApp* const getInstance();
	static	LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	MSG				m_msg;		// Windows Message Structure
	HDC				m_hDC;		// Private GDI Device Context
	HGLRC			m_hRC;		// Permanent Rendering Context
	HWND			m_hWnd;		// Holds Our Window Handle
	HINSTANCE		m_hInstance;	// Holds The Instance Of The Application

};