/*
 * GLWindow.h
 *
 *  Created on: 30 avr. 2011
 *      Author: !!M
 */
#ifndef GLWINDOW_H_
#define GLWINDOW_H_


#include "Core/GLExtLoader.h"
#include "../Utils/Exceptions.h"
#include "../Utils/Timer.h"
#include <windows.h>
#include <process.h>

class GLWindow
{
	public:
					GLWindow		(const char* aTitle, bool aFullscreen);
		virtual		~GLWindow		();
		int			getHeight		();
		int			getWidth		();
		float		getRatio2d		();
		HINSTANCE	getHInstance	();
		HDC			getHDC			();
		void		run				();
		bool		isFullscreen	();

	protected:
		Timer			timer;
		HWND			handle;
		HANDLE			thHandle;
		HDC				dc;
		HGLRC			glHandle;
		LPCSTR			title;
		unsigned int	threadId;
		volatile int	width;
		volatile int	height;
		volatile bool	thLoopFlag;
		volatile bool	loopFlag;
		bool			fullscreen;
		void			quit			();
		virtual	void	glInit			();
		virtual	void	glShutdown		();
		virtual	void	renderTick		(double aDelta);
		virtual	void	keyPressed		(unsigned int aKey);
				bool	enterFullscreen	();

	private:
				float				ratio2d;
		static	int					thStart				(void* aArg);
				void				thStop				();
				void				threadFunc			();
				void				startGL				();
				void				stopGL				();
				void				createWindow		();
		static	LRESULT	CALLBACK	staticWindowProc	(HWND aHWND, UINT aUMSG, WPARAM aWPARAM, LPARAM aLPARAM);
				LRESULT	CALLBACK	windowProc			(HWND aHWND, UINT aUMSG, WPARAM aWPARAM, LPARAM aLPARAM);
};

#endif /* GLWINDOW_H_ */
