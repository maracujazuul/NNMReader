/*
 * GLWindow.cpp
 *
 *  Created on: 30 avr. 2011
 *      Author: !!M
 */

#include "GLWindow.h"

// Used to get elapsedTime
static	double	deltaTime	= 0.0;
static	bool	ready		= false;

GLWindow::GLWindow(const char* aTitle, bool aFullscreen) :
handle			(NULL),
thHandle		(NULL),
dc				(NULL),
glHandle		(NULL),
title			(aTitle),
threadId		(0),
width			(720),
height			(480),
thLoopFlag		(false),
loopFlag		(false),
fullscreen		(aFullscreen),
ratio2d			(1.0f)
{
	/*
	if(aFullscreen)
	{
		this->width		= GetSystemMetrics(SM_CXSCREEN);
		this->height	= GetSystemMetrics(SM_CYSCREEN);
		this->ratio2d	= (float)this->width / (float)this->height;
	}
	*/

	/* register window class */
	WNDCLASSEX			wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= &GLWindow::staticWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(0);
	wcex.hIcon			= 0;//LoadIcon(NULL, (char*)APP_ICON);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH) CreateSolidBrush(RGB(0,0,0));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= this->title;
	wcex.hIconSm		= 0;//LoadIcon(NULL, (char*)APP_ICON);

	if (!RegisterClassEx(&wcex))
		throw SimpleError("RegisterClassEx failed!");
}

GLWindow::~GLWindow()
{
	if(this->thLoopFlag)	this->thStop();
	ReleaseDC				(this->handle, this->dc);
	DestroyWindow			(this->handle);
	UnregisterClass			(this->title, GetModuleHandle(0));
}

float GLWindow::getRatio2d()
{
	return this->ratio2d;
}

void GLWindow::createWindow()
{
	/* create the window */
	RECT	wRect		= {0, 0, this->width, this->height};
	DWORD	exStyle		= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD	style		= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD	pos			= CW_USEDEFAULT;

	if(this->fullscreen)
	{
		style	|= WS_POPUP;
		pos		= 0;
	}
	else style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRectEx (&wRect, style, 0, exStyle);

	this->handle = CreateWindowEx( exStyle,
									this->title,
									this->title,
									style,
									pos,
									pos,
									wRect.right - wRect.left,
									wRect.bottom - wRect.top,
									NULL,
									NULL,
									GetModuleHandle(0),
									NULL);

	if(!this->handle) throw SimpleError("createWindow failed!");
}

HDC GLWindow::getHDC()
{
	return this->dc;
}

void GLWindow::run()
{
	// Create the window
	this->createWindow();

	// Go fullscreen if needed and show window
	if(this->fullscreen)
		if(!this->enterFullscreen())
		{
	    	MessageBox			(this->handle, "Could not enter in fullscreen mode!", "Warning", MB_OK | MB_ICONWARNING);
	    	this->fullscreen	= false;
	    	DestroyWindow		(this->handle);
	    	this->createWindow	();
		}

	this->dc			= GetDC(this->handle);
	SetWindowLongPtr	(this->handle, GWLP_USERDATA, (LONG_PTR) this);
	ShowWindow			(this->handle, SW_SHOW);

	// Create and start the render thread (try to set an higher priority)
	this->thHandle		= (HANDLE) _beginthreadex(NULL, 0, (unsigned (__stdcall *)(void *))&GLWindow::thStart, this, 0, &this->threadId);
	if(this->thHandle)	SetThreadPriority (this->thHandle, THREAD_PRIORITY_HIGHEST);
	else				throw SimpleError("_beginthreadex failed!");

	// Loop until the windows was closed
	MSG				msg;
	this->loopFlag	= true;
	while(this->loopFlag)
	{
		if(PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage	(&msg);
			DispatchMessage		(&msg);
		}
		// Do something verryyy interesting...
		Sleep(5);
	}

	/*
	 * 1. Stop the render thread and free the dc
	 * 2. Destroy the window
	 * 3. Restore resolution if needed
	 */
	this->thStop							();
    if(!ReleaseDC(this->handle, this->dc))	throw SimpleError("ReleaseDC failed!");
	DestroyWindow							(this->handle);
	if(this->fullscreen) 					ChangeDisplaySettings(NULL, 0);

}

bool GLWindow::enterFullscreen()
{
	DEVMODE dm;
    memset			(&dm, 0, sizeof(DEVMODE));
    dm.dmSize       = sizeof(DEVMODE);
    dm.dmPelsWidth  = this->width;
    dm.dmPelsHeight = this->height;
    dm.dmBitsPerPel = 32;
    dm.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Switch to fullscreen mode
    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
    	return false;
    }
    return true;
}

int GLWindow::getHeight()
{
	return this->height;
}

int GLWindow::getWidth()
{
	return this->width;
}

/*
 * This method should be inherited to init some OpenGL things
 * like texture, vbo and all other related graphics resources
 */
void GLWindow::glInit()
{}

/*
 * This method should be inherited to deinit the graphics resource
 * allocated in glInit
 */
void GLWindow::glShutdown()
{}

void GLWindow::keyPressed(unsigned int aKey)
{};

void GLWindow::renderTick(double aDelta)
{}

bool GLWindow::isFullscreen()
{
	return this->fullscreen;
}

void GLWindow::startGL()
{
	// Init whole GL
	PIXELFORMATDESCRIPTOR pd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
		1,                               // version number
		PFD_DRAW_TO_WINDOW |             // support window
		PFD_SUPPORT_OPENGL |             // support OpenGL
		PFD_DOUBLEBUFFER,                // double buffered
		PFD_TYPE_RGBA,                   // RGBA type
		32,                              // 24-bit color depth
		0, 0, 0, 0, 0, 0,                // color bits ignored
		0,                               // no alpha buffer
		0,                               // shift bit ignored
		0,                               // no accumulation
		0, 0, 0, 0,                  	 // accum bits
		32,                              // 24-bit z-buffer
		0,                               // 32-bits stencil buffer
		0,                               // no auxiliary buffer
		PFD_MAIN_PLANE,                  // main layer
		0,                               // reserved
		0, 0, 0                          // layer masks ignored
	};

	if (! SetPixelFormat(this->dc, ChoosePixelFormat(this->dc, &pd), &pd))
		throw SimpleError("SetPixelFormat failed!");

	this->glHandle = wglCreateContext(this->dc);
	if(! this->glHandle)
		throw SimpleError("wglCreateContext failed!");

	if (! wglMakeCurrent(this->dc, this->glHandle))
		throw SimpleError("wglMakeCurrent failed!");

	// use of arb buffer
	if(!GLExtLoader::checkExtension((const char*)glGetString(GL_EXTENSIONS), "GL_ARB_vertex_buffer_object")) throw LoadingError("GL_ARB_vertex_buffer_object", "Necessary extensions unsupported!");
	else
	{
		GLExtLoader::LoadExtension(GLExtLoader::glBindBufferARB,		"glBindBufferARB");
		GLExtLoader::LoadExtension(GLExtLoader::glDeleteBuffersARB,		"glDeleteBuffersARB");
		GLExtLoader::LoadExtension(GLExtLoader::glGenBuffersARB,		"glGenBuffersARB");
		GLExtLoader::LoadExtension(GLExtLoader::glBufferDataARB,		"glBufferDataARB");
		GLExtLoader::LoadExtension(GLExtLoader::glBufferSubDataARB,		"glBufferSubDataARB");
		GLExtLoader::LoadExtension(GLExtLoader::glMapBufferARB,			"glMapBufferARB");
		GLExtLoader::LoadExtension(GLExtLoader::glUnmapBufferARB,		"glUnmapBufferARB");
	}

/*
	if(!GLExtLoader::checkExtension("GL_EXT_texture_filter_anisotropic")) throw LoadingError("GL_EXT_texture_filter_anisotropic", "Necessary extensions unsupported!");
	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	if(maxAnisotropy < 4.0f) throw SimpleError("Anisotropy level 4 required!");

	// use of glGenerateMimmapEXT (can also use framebuffer function)
	if(!GLExtLoader::checkExtension("GL_EXT_framebuffer_object")) throw LoadingError("GL_EXT_framebuffer_object", "Necessary extensions unsupported!");
	else
	{

	}

	// use of glsl shader
	if(!GLExtLoader::checkExtension("GL_ARB_shader_objects GL_ARB_vertex_shader GL_ARB_fragment_shader")) throw LoadingError("GL_ARB_shader_objects GL_ARB_vertex_shader GL_ARB_fragment_shader", "Necessary extensions unsupported!");
	else
	{

	}
	*/
	// used to force vsync
	GLExtLoader::LoadExtension(GLExtLoader::wglGetExtensionsStringARB, "wglGetExtensionsStringARB");
	if(!GLExtLoader::checkExtension((const char*)GLExtLoader::wglGetExtensionsStringARB(wglGetCurrentDC()), "WGL_EXT_swap_control")) MessageBox(NULL, "Could not activate vsync!", "Warning", MB_OK | MB_ICONWARNING);
	else
	{
		GLExtLoader::LoadExtension(GLExtLoader::wglSwapIntervalEXT, "wglSwapIntervalEXT");
		GLExtLoader::wglSwapIntervalEXT(1);
	}

	glEnable		(GL_TEXTURE_2D);
	glPixelStorei	(GL_UNPACK_ALIGNMENT, 4);
	glEnable		(GL_DEPTH_TEST);
	glDepthFunc		(GL_LEQUAL);
	glEnable		(GL_CULL_FACE);
	glFrontFace		(GL_CW);
	glHint			(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable		(GL_BLEND);
	glClearColor	(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth	(1.0f);
	glColor4f		(1.0f, 1.0f, 1.0f, 1.0f);
	// Let the dev doing it's initialisation if needed
	this->glInit();
	ready = true;
}

void GLWindow::stopGL()
{
	// Deinit some things
	this->glShutdown		();

	// Destroy the context
    if (this->glHandle)
    {
    	glFlush();
    	if(!wglMakeCurrent(NULL, NULL))
			throw SimpleError("wglMakeCurrent failed!");
		if(! wglDeleteContext(this->glHandle))
			throw SimpleError("wglDeleteContext failed!");
	}
}

int GLWindow::thStart(void* aArg)
{
	((GLWindow*)aArg)->threadFunc();
	return 0;
}

void GLWindow::threadFunc()
{
	/*
	 * Start render loop
	 * 1. Start OpenGL
	 * 2. Enter into the render thread loop
	 * 3. Stop OpenGL
	 */
	try
	{
		this->startGL();
		this->thLoopFlag = true;
		while(this->thLoopFlag)
		{
			deltaTime = 0.0;
			while(deltaTime <= 0.0166666666666667) // 0.0166666666666667f = (1 / 60) = 60 fps
				deltaTime += this->timer.tick();

			// Do the user render loop
			this->renderTick(deltaTime);
			SwapBuffers(this->dc);
			//Sleep(1);
		}
		this->stopGL();
	}
	catch(const std::exception& e)
	{
		MessageBox(NULL, e.what(), "Fatal Error", MB_ICONERROR | MB_OK);
		if(thLoopFlag)		this->stopGL();
		this->thLoopFlag	= false;
		this->loopFlag		= false;
	}
}

void GLWindow::thStop()
{
	// Set the render thread loop to false and wait for the thread to terminate
	this->thLoopFlag = false;
	if(WaitForSingleObject(this->thHandle, INFINITE) == WAIT_FAILED)
		throw SimpleError("WaitForSingleObject failed!");
	if(!CloseHandle(this->thHandle))
		throw SimpleError("CloseHandle failed!");
}

void GLWindow::quit()
{
	this->loopFlag = false;
}

LRESULT CALLBACK GLWindow::staticWindowProc(HWND aHWND, UINT aUMSG, WPARAM aWPARAM, LPARAM aLPARAM)
{
	GLWindow *w = (GLWindow*) GetWindowLongPtr(aHWND, GWLP_USERDATA);
	if(w) return w->windowProc(aHWND, aUMSG, aWPARAM, aLPARAM);
	return DefWindowProc(aHWND, aUMSG, aWPARAM, aLPARAM);
}

LRESULT CALLBACK GLWindow::windowProc(HWND aHWND, UINT aUMSG, WPARAM aWPARAM, LPARAM aLPARAM)
{
	if(ready)
	{
		switch (aUMSG)
		{
			case WM_CLOSE:
				this->loopFlag = false;
				return 0;
			break;

			case WM_DESTROY:
				PostQuitMessage(0);
			break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				switch (aWPARAM)
				{
					// Handle keys pressed
					case VK_ESCAPE:
						// [ESCAPE] = We want to quit
						this->loopFlag = false;
					break;

					default:
						this->keyPressed(aWPARAM);
					break;
				}
			break;

			case WM_MOUSEWHEEL:
				if(GET_WHEEL_DELTA_WPARAM(aWPARAM) > 0) this->keyPressed(VK_UP);
				else this->keyPressed(VK_DOWN);
			break;

			case WM_SYSCOMMAND:
				switch (aWPARAM)
				{
					// Disable screensaver and monitor power saving
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
					return 0;
				}
			break;
		}
	}
	return DefWindowProc(aHWND, aUMSG, aWPARAM, aLPARAM);
}
