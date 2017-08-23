/*
 * GLExtLoader.cpp
 *
 *  Created on: 11 sept. 2011
 *      Author: !!M
 */

#include "GLExtLoader.h"

// Function borrowed from http://loulou.developpez.com/ in the section 3d engine.
// And http://alexandre-laurent.developpez.com/tutoriels/OpenGL/OpenGL-Extensions/
// AND http://steinsoft.net/index.php?site=Programming/Code%20Snippets/OpenGL/no10
// I have taken what i need from these sources to fit my attempt

PFNGLBINDBUFFERARBPROC				GLExtLoader::glBindBufferARB;
PFNGLDELETEBUFFERSARBPROC			GLExtLoader::glDeleteBuffersARB;
PFNGLGENBUFFERSARBPROC				GLExtLoader::glGenBuffersARB;
PFNGLBUFFERDATAARBPROC				GLExtLoader::glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC			GLExtLoader::glBufferSubDataARB;
PFNGLMAPBUFFERARBPROC				GLExtLoader::glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC				GLExtLoader::glUnmapBufferARB;
PFNWGLSWAPINTERVALEXTPROC			GLExtLoader::wglSwapIntervalEXT;
PFNWGLGETEXTENSIONSSTRINGARBPROC	GLExtLoader::wglGetExtensionsStringARB;

PFNGLBINDRENDERBUFFEREXTPROC					GLExtLoader::glBindRenderbufferEXT;
PFNGLDELETERENDERBUFFERSEXTPROC					GLExtLoader::glDeleteRenderbuffersEXT;
PFNGLGENRENDERBUFFERSEXTPROC					GLExtLoader::glGenRenderbuffersEXT;
PFNGLGENFRAMEBUFFERSEXTPROC						GLExtLoader::glGenFramebuffersEXT;
PFNGLRENDERBUFFERSTORAGEEXTPROC					GLExtLoader::glRenderbufferStorageEXT;
PFNGLDELETEFRAMEBUFFERSEXTPROC					GLExtLoader::glDeleteFramebuffersEXT;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				GLExtLoader::glFramebufferRenderbufferEXT;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC				GLExtLoader::glFramebufferTexture1DEXT;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC				GLExtLoader::glFramebufferTexture2DEXT;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC				GLExtLoader::glFramebufferTexture3DEXT;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	GLExtLoader::glGetFramebufferAttachmentParameterivEXT;

PFNGLATTACHOBJECTARBPROC			GLExtLoader::glAttachObjectARB;
PFNGLCOMPILESHADERARBPROC			GLExtLoader::glCompileShaderARB;
PFNGLCREATEPROGRAMOBJECTARBPROC		GLExtLoader::glCreateProgramObjectARB;
PFNGLCREATESHADEROBJECTARBPROC		GLExtLoader::glCreateShaderObjectARB;
PFNGLDELETEOBJECTARBPROC			GLExtLoader::glDeleteObjectARB;
PFNGLGETINFOLOGARBPROC				GLExtLoader::glGetInfoLogARB;
PFNGLGETOBJECTPARAMETERIVARBPROC	GLExtLoader::glGetObjectParameterivARB;
PFNGLLINKPROGRAMARBPROC				GLExtLoader::glLinkProgramARB;
PFNGLSHADERSOURCEARBPROC			GLExtLoader::glShaderSourceARB;

 bool GLExtLoader::checkExtension(const char* aExtensions, const char* aExtension)
{
    char* extensionMatch = NULL;

    while ( (extensionMatch = (char*)strstr(aExtensions, aExtension)) != NULL )
    {
        // Vérification: avons-nous la même chaine que l'extension voulue ?
        if ( strncmp(extensionMatch,aExtension,strlen(aExtension)) == 0 )
        {
            // Vérification: le nom trouvé se termine-t-il bien ici ?
            if ( extensionMatch[strlen(aExtension)] == ' ')
            {
                return true;
            }
        }

        // On déplace le pointeur de recherche après la chaine trouvée pour continuer la recherche
        aExtensions = extensionMatch + strlen(extensionMatch);
    }
    return false;
}
