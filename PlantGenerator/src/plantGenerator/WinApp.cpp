#include "Definitions.h"
#include "WinApp.h"

//WinApp* WinApp::Instance = NULL;

 int WinApp::InitInstance(int width, int heigth, HINSTANCE instance, char* name)
{

	if(s_instance == NULL)
	{
		s_instance = new WinApp(width, heigth, instance, name);
		//Instance = new WinApp(width, heigth, instance, name);
		return 0;
	}

	return -1;
}

 WinApp::WinApp(int width,int heigth,HINSTANCE instance,char* name) : m_hInstance(instance)
{
	CreateGLWindow(name, width,  heigth);
}

WinApp::~WinApp(void)
{
	OnDestroy();
}

void WinApp::OnDestroy()
{

	if (m_hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(m_hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_hRC=NULL;										// Set RC To NULL
	}

	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hDC=NULL;										// Set DC To NULL
	}

	if (m_hWnd && !DestroyWindow(m_hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release m_hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hWnd=NULL;										// Set m_hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",m_hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hInstance=NULL;									// Set m_hInstance To NULL
	}
}

bool WinApp::CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left =  (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top  = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	ZeroMemory(&wc,sizeof(WNDCLASS));
	wc.style			= CS_HREDRAW | CS_VREDRAW;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= WndProc;								// WndProc Handles Messages
	wc.hInstance		= m_hInstance;							// Set The Instance
	//wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	//wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.lpszClassName	= "PlantGenerator";						// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											
	}	

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(m_hWnd=CreateWindowEx(dwExStyle,							// Extended Style For The Window
								wc.lpszClassName,					// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								m_hInstance,						// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL,// |						// Format Must Support OpenGL
		//PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		32,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))							// Did We Get A Device Context?
	{
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(m_hRC=wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(m_hDC,m_hRC))					// Try To Activate The Rendering Context
	{
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	GLenum glewErr =  glewInit();
	if(glewErr != GLEW_OK)
	{
		LOGI((char*)glewGetErrorString(glewErr));
	}
	OnCreate();
	OnResize(width, height);					// Set Up Our Perspective GL Screen

	ShowWindow(m_hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(m_hWnd);						// Slightly Higher Priority
	SetFocus(m_hWnd);									// Sets Keyboard Focus To The Window

	return TRUE;		
}

int WinApp::RunMainLoop(int cmdShow)
{	
	return ((WinApp*)s_instance)->MainLoop(cmdShow);
}

int WinApp::MainLoop(int cmdShow)
{	
	WinApp* l_instance = (WinApp*)s_instance;
	ShowWindow(l_instance->m_hWnd,cmdShow);
	UpdateWindow(m_hWnd);
	
	while(true)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			TranslateMessage(&m_msg);				// Translate The Message
			DispatchMessage(&m_msg);				// Dispatch The Message

			if (m_msg.message == WM_QUIT)				// Have We Received A Quit Message?
				break;
		}
		else
		{
			OnRender();
			glFlush();
			//SwapBuffers(m_hDC);				// Swap Buffers (Double Buffering)
		}
	}
	return (m_msg.wParam);							// Exit The Program
}
LRESULT CALLBACK WinApp::WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	//WinApp* l_instance = (WinApp*)s_instance;
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			s_instance->OnTouch(0,0); break;
		case WM_SIZE:								// Resize The OpenGL Window
			s_instance->OnResize(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			break;
		case WM_SYSCOMMAND:							// Intercept System Commands
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				break;
			}
			break;									// Exit		
		case WM_CLOSE:								// Did We Receive A Close Message?
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
}