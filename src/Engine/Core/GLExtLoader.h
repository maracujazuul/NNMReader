/*
 * GLExtLoader.h
 *
 *  Created on: 11 sept. 2011
 *      Author: !!M
 */

#ifndef GLEXTLOADER_H_
#define GLEXTLOADER_H_

#include "../../Utils/Exceptions.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <string.h>

class GLExtLoader
{
	public:
		static bool checkExtension		(const char* aExtensions, const char* aExtension);

		template <class T>
		static inline void LoadExtension(T& aProc, const char* aName);

		static PFNGLBINDBUFFERARBPROC		glBindBufferARB;
		static PFNGLDELETEBUFFERSARBPROC	glDeleteBuffersARB;
		static PFNGLGENBUFFERSARBPROC		glGenBuffersARB;
		static PFNGLBUFFERDATAARBPROC		glBufferDataARB;
		static PFNGLBUFFERSUBDATAARBPROC	glBufferSubDataARB;
		static PFNGLMAPBUFFERARBPROC		glMapBufferARB;
		static PFNGLUNMAPBUFFERARBPROC		glUnmapBufferARB;

		static PFNWGLSWAPINTERVALEXTPROC		wglSwapIntervalEXT;
        static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

        static PFNGLBINDRENDERBUFFEREXTPROC						glBindRenderbufferEXT;
        static PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT;
        static PFNGLGENRENDERBUFFERSEXTPROC						glGenRenderbuffersEXT;
        static PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT;
        static PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT;
        static PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT;
        static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT;
        static PFNGLFRAMEBUFFERTEXTURE1DEXTPROC					glFramebufferTexture1DEXT;
        static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC					glFramebufferTexture2DEXT;
        static PFNGLFRAMEBUFFERTEXTURE3DEXTPROC					glFramebufferTexture3DEXT;
        static PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameterivEXT;

        static PFNGLATTACHOBJECTARBPROC				glAttachObjectARB;
        static PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
        static PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
        static PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
        static PFNGLDELETEOBJECTARBPROC				glDeleteObjectARB;
        static PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
        static PFNGLGETOBJECTPARAMETERIVARBPROC		glGetObjectParameterivARB;
        static PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;
        static PFNGLSHADERSOURCEARBPROC				glShaderSourceARB;
};

template <class T>
inline void GLExtLoader::LoadExtension(T& aProc, const char* aName)
{
    aProc = (T)(wglGetProcAddress(aName));
}

#endif /* GLEXTLOADER_H_ */
