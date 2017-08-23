/*
 * SceneIntro.cpp
 *
 *  Created on: 12 jul 2011
 *      Author: !!M
 */

#include "SceneIntro.h"
#define PI180			3.14f / 180
#define CHAR_SIZE		8
#define FLASH_RES		16
#define JUMP_AMPLITUDE	2.0f

static	Flash						*flash			= NULL;
static	Icosahedron					*icoVBO			= NULL;
static	BitmapFont					*font			= NULL;
static	Texture						*fontTex		= NULL;
static	Texture						*autoScrollTex	= NULL;
static	Texture						*bkgTex[3]		= {NULL, NULL, NULL};
static	PlaneMesh					*bkgVBO			= NULL;
static	PlaneMesh					*ctrlVBO		= NULL;
static	PlaneMesh					*autoscrollVBO	= NULL;
static	Texture						*texArrow		= NULL;
static	RibbonMesh					*ribbonVBO		= NULL;
static	double						timeElapsed;
static	float						bkgOpacity;
static	float						oldBkgOpacity;
static	int							bkgIndex;
static	bool						changeBkg;
static	float						jumpTable		[540];
static	int							jumpIndex;
static	float						icoScaleX;
static	float						icoScaleY;
static	float						icoScaleZ;
static	std::vector<const char*>	nfo;
static	float						scrollPos;
static	bool						scrollFlag;
static	bool						autoScroll;
static	bool						leftFlag;
static	float						bkgOffset;
static	float						icoOffsetY;
static	bool						fileFound;
static	float						upCtrlOffset;
static	float						downCtrlOffset;

SceneIntro::SceneIntro(GLWindow* aGLWindow, const char* aFilename) :
Scene		(aGLWindow)
{
	// precalc te jumping table
	int i;
	int j;
	float rad;
	for (i = 0, j = 0; i < 180; i++)
	{
		rad = (float) j * 0.0174532f;
		jumpTable[i] = sinf(rad);

		if (!((i + 1) % 2)) j++;
	}
	for (i = 90, j = 90; i < 270; i++)
	{
		rad = (float) j * 0.0174532f;
		jumpTable[i + 90] = sinf(rad);

		if (!((i + 1) % 2)) j++;
	}
	for (i = 180; i < 270; i++)
	{
		rad = (float) i * 0.0174532f;
		jumpTable[i + 180] = sinf(rad);
	}
	for (i = 270; i < 360; i++)
	{
		rad = (float) i * 0.0174532f;
		jumpTable[i + 180] = sinf(rad);
	}

	fontTex			= Utils::loadTextureFromFile(TEX_FNT, GL_CLAMP, GL_NEAREST);//SOIL_LOAD_AUTO, GL_CLAMP, GL_NEAREST);
	autoScrollTex	= Utils::loadTextureFromFile(TEX_AUTOSCROLL, GL_CLAMP, GL_NEAREST);//SOIL_LOAD_AUTO, GL_CLAMP, GL_NEAREST);
	bkgTex[0]		= Utils::loadTextureFromFile(TEX_BKG1, GL_REPEAT, GL_NEAREST);//SOIL_LOAD_AUTO, GL_REPEAT, GL_NEAREST);
	bkgTex[1]		= Utils::loadTextureFromFile(TEX_BKG2, GL_REPEAT, GL_NEAREST);//SOIL_LOAD_AUTO, GL_REPEAT, GL_NEAREST);
	bkgTex[2]		= Utils::loadTextureFromFile(TEX_BKG3, GL_REPEAT, GL_NEAREST);//SOIL_LOAD_AUTO, GL_REPEAT, GL_NEAREST);
	texArrow		= Utils::loadTextureFromFile(TEX_ARROW, GL_CLAMP, GL_NEAREST);//SOIL_LOAD_AUTO, GL_REPEAT, GL_NEAREST);

	font			= new BitmapFont(fontTex->getWidth(), CHAR_SIZE, CHAR_SIZE, 2.0f);
	flash			= new Flash(FLASH_RES);
	icoVBO			= new Icosahedron(T4F(0.9f, 0.9f, 0.9f, 0.6f), T4F(0.12f, 0.18f, 0.4f, 0.5f));
	ribbonVBO		= new RibbonMesh(aGLWindow->getHeight(), 10.0f, (float) aGLWindow->getWidth()*0.7f, (float)aGLWindow->getWidth()*0.02f, T4F(0.0f, 0.0f, 0.0f, 0.50f));
	bkgVBO			= new PlaneMesh(27.0f, 27.0f, 10.0f);
	ctrlVBO			= new PlaneMesh(texArrow->getWidth(), texArrow->getHeight(), 1.0f);
	autoscrollVBO	= new PlaneMesh(autoScrollTex->getWidth(), autoScrollTex->getHeight(), 1.0f);


	// We will try to load the nfo from text file (encoding: ISO-8859-15 plz)
	//if no file was found in the cmd line, fallback to readme.nfo, then fallback to an internal nfo
	FILE *f = fopen(aFilename, "rt");
	if(f == NULL) f = fopen("readme.nfo", "rt");
	if(f != NULL)
	{
		char buffer[82]; // should be just enought, since only 80 char can be displayed correclty to fit the left ribbon
		while (fgets(buffer, sizeof(buffer), f) != NULL)
		{
			char* line = new char[sizeof(buffer)];
			sprintf(line, "%s", buffer);
			nfo.push_back(line);
		}
		fileFound = true;
		fclose(f);
	}
	else
	{
			nfo.push_back("\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xfe\xfe\xdf\xdf\xdb\xdb\xfe\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xfe\xfe\xdf\xdf\xdb\xdb\xfe\xfe\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xfe\xdf\xdf\xdb\xdb\xfe\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc");
			nfo.push_back("           \xdb    \x22\xb0\xb1\xb2\xb2\xdb   \xdc\xfe\xdf\xdf\xdb\xdb\xfe\xdc \xdc\xdf  _  \x22\xb0\xb1\xb2\xb2\xdb\xdc \xdc\xdf    \x22\xb0\xb1\xb2\xdb");
			nfo.push_back("  \xdc\xfe\xdc  \xf8  \xdb       \xb0\xb0\xb1\xb2\xdb\xdc\xdf   \x22\xb0\xb1\xb2\xb2\xdc\xdf  (_)   \xb0\xb0\xb1\xb2\xdb\xdc\xdf   \xb0\xb1  \xb0\xb1\xb2\xdb    _   \xf8  o");
			nfo.push_back(" \xde_\x22\xb0\xdd    \xdb    \xdc\xdc   \xb0\xb0\xdc\xdf      \xb0\xb0\xb1\xdb     \xb0\xb1    \xb0\xb0\xb1\xdb   \xb0\xb1    \xb0\xb0\xb1\xdb  (_)          O");
			nfo.push_back("  \xdf\xfe\xdf  _  \xdb\xb0  \xde_\xb0\xdd   \xb0\xdb   \xb0\xb1    \xb0\xdb\xb0     \xb0\xb1    \xb0\xb0\xdb\xb0          \xb0\xb0\xdb     \xf8  \xdc\xdc\xdc\xdc");
			nfo.push_back("      (_)  \xdb\xb0  \xdf\xdf    \xb0\xdb\xdc   \xb0\xb1    \xdf\xdb\xb1            \xdb\xb2    \xdc\xdf\xdf\xdb\xdc   \xb0\xdb     \xdc\xdf \xb0\xb0\xb1\xdb\xdc");
			nfo.push_back("           \xdb\xb1\xb0        \xb0\xdb\xb0        \xb1\xdb\xb1\xb0    \xdc\xdc\xdc\xdc    \xdb\xb0  \xde  \x22\xb0\xb1\xdd   \xb0\xdb   \xde_    \xb0\xb1\xb0\xdd");
			nfo.push_back("   \xdc\xdf\xdb\xdc   \xdb\xb0     _   \xb0\xb1\xdb\xb1\xb0       \xb0\xb1\xdb\xb0  \xdc\xdf \xb0\xb0\xb1\xdb\xdc  \xde   \xde\xdc_  \xb0\xdd   \xb0\xb2\xdb  \xde\xb2_      \xdd");
			nfo.push_back("\xdf\xdf\xde_ \x22\xb0\xdd\xdf\xdb\xdf     (_)  \xb0\xdb\xb0   \xdc\xdf\xdb\xdc   \xb0\xdb\xb0 \xde_    \xb0\xb1\xb0\xdd      \xdf\xdb\xdc\xdc\xdf     \xb0\xb2\xdb\xdf\xdf\xdf\xdb\xdc\xdc  \xdc\xdf\xdf\xdf\xdf");
			nfo.push_back("   \xdf\xdc\xdc\xdf  \xdb   \xb0\xb1    \xb0\xb0\xb0\xdb   \xde_ \x22\xb0\xdd\xb0\xb0\xdc\xdb\xb0 \xde\xb2_      \xdd                 \xb0\xb2\xdb   \xdf\xdf\xdf\xdf");
			nfo.push_back("         \xdb_   \xb0\xb1  \xb0\xb0\xb1\xdb   \xb0\xb1\xdf\xdc\xdc\xdf\xb0\xb0\xb1\xdb\xb0   \xdf\xdb\xdc\xdc  \xdc\xdf  \xdc\xdf\xdb\xdc  \xb0\xb1\xb0\xb0      \xb0\xb1\xb2\xdb        o");
			nfo.push_back("   _  \xf8  \xdf\xdb\xdc\xfe\xdf\xdf\xdb\xdb\xfe\xdc \xb0\xdb_   \xb0\xb1    \xb0\xb0\xdb      \xdf\xdf\xdf\xdf\xb1\xb0 \xde_ \x22\xb0\xdd\xb0\xb1\xb0\xb0   \xdc\xfe\xdc  \xb0\xb1\xb2\xdb    \xdc\xdc");
			nfo.push_back("  (_)    \xdc\xdf   \x22\xb0\xb1\xb2\xb2\xdb\xdc\xdf\xdb\xdc\xfe\xdf\xdf\xdb\xdb\xfe\xdc \xdc\xdb        \xb0\xb1\xb0\xb0   \xdf\xdc\xdc\xdf\xb0\xb0\xb0    \xde_\x22\xb0\xdd \xb0\xb1\xb2\xdb   \xde_\xb0\xdd");
			nfo.push_back("        \xdc\xdf      \xb0\xb0\xb1\xb2\xdb\xdc\xdf   \x22\xb0\xb1\xb2\xb2\xdb\xdb       _          \xb0\xb0\xb0   \xde   \xdf\xfe\xdf   \xb0\xb1\xb2\xdb   \xdf\xdf");
			nfo.push_back("\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdb   \xb0\xb1    \xb0\xb0\xdc\xdf  \xdc\xfe\xdc \xb0\xb0\xb1\xb1\xdb      (_)  \xde            \xb0\xdb     _  \xb0\xb0\xb1\xdb\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc");
			nfo.push_back("    _   \xdb\xdc_  \xb0\xb1    \xb0\xdb  \xde_\x22\xb0\xdd  \xb0\xdb    \xdc\xdc      \xb0\xdb   \xb0\xb1     \xb0\xb1\xdb    (_)  \xb0\xb0\xdb    _");
			nfo.push_back("   (_)  \xdf\xdb\xb2_       \xb0\xdb\xdc_ \xdf\xfe\xdf    \xdb   \xde_\xb0\xdd    \xb0\xb1\xdb_   \xb0\xb1   \xb0\xb0\xdc\xdb\xb2_       \xb0\xdc\xdf   (_)");
			nfo.push_back("         \xdf\xdb\xb2\xb0_    \xb0\xdc\xdf\xdb\xb2_       \xdb\xdc_  \xdf\xdf    \xb0\xb0\xdc\xdb\xb2_       \xb0\xdc\xdf\xdf\xdb\xb2\xb0_    \xb0\xdc\xdf");
			nfo.push_back("  \xf8    o   \xdf\xfe\xdb\xdc\xdc\xdc\xfe\xdf  \xdf\xdb\xb2\xb0_    \xb0\xdb\xdb\xb2_       \xb0\xdc\xdf \xdb\xb2\xb0_    \xb0\xdc\xdf   \xdf\xfe\xdb\xdc\xdc\xdc\xfe\xdf    O    \xf8");
			nfo.push_back("               \xdc\xdc\xdc\xdc    \xdf\xfe\xdb\xdc\xdc\xdc\xfe\xdf \xdf\xdb\xb2\xb0_    \xb0\xdc\xdf   \xdf\xfe\xdb\xdc\xdc\xdc\xfe\xdf");
			nfo.push_back("\xdf\xdf\xdf\xdf\xdf\xdf\xdb\xdf\xdb\xdb\xdf\xdf\xdf\xdb\xdf \xb0\xb0\xb1\xdb\xdb\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xfe\xdb\xdc\xdc\xdc\xfe\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf");
			nfo.push_back("     \xde_ \x22\xb0\xdd \xde_    \xb0\xb1\xb0\xdd         O          \xde\xdf\xdf\xdc\xde\xdf\xdf\xdc\xde\xdf\xdf \xde\xdf\xdf\xde\xdf\xdf\xde\xdf\xdc \xdf\xdf\xde\xdf\xdf   o");
			nfo.push_back("  o   \xdf\xdc\xdc\xdf  \xde\xb2_      \xdd   \xdc\xdf\xdf\xdb\xdc      O      \xdd\xdc\xdf \xdd\xdc\xdf \xdd\xfe  \xdf\xdc \xdd\xfe \xdd \xdd \xdd \xdf\xdc       O");
			nfo.push_back("\xdc\xdc\xdc\xdc\xfe\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdb\xdb\xdc\xdc  \xdc\xdb\xdc\xdc\xdc\xde  \x22\xb0\xb1\xdd\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xde   \xde \xdf\xdc\xde\xdc\xdc \xdc\xdc\xdd\xde\xdc\xdc\xde \xde \xde \xdc\xdc\xdd\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc");
			nfo.push_back("  \xde_\x22\xb0\xdd \xdc\xdc     \xdf\xdf\xdf\xdf     \xde\xdc_  \xb0\xdd");
			nfo.push_back("   \xdf\xfe\xdf \xde_\xb0\xdd          o   \xdf\xdb\xdc\xdc\xdf  A KIND OF KIKOOLOL NFO READER... hOPE YOU WILL");
			nfo.push_back(" \xf8    o \xdf\xdf   \xf8   _              ENJOY IT. oR AT LEAST, HOPE IT MAKES YOU SMILE.");
			nfo.push_back("  \xdc\xdf\xdb\xdc          (_)");
			nfo.push_back(" \xde_ \x22\xb0\xdd  \xdc\xdf\xdf\xdb\xdc           cOMMANDS AND USAGE");
			nfo.push_back("  \xdf\xdc\xdc\xdf  \xde  \x22\xb0\xb1\xdd   \xdc\xdf\xdb\xdc");
			nfo.push_back(" \xf8    o \xde\xdc_  \xb0\xdd  \xde_ \x22\xb0\xdd    [F1] Flash screen   [F2] Toggle autoscroll");
			nfo.push_back(" \xdc\xfe\xdc     \xdf\xdb\xdc\xdc\xdf    \xdf\xdc\xdc\xdf");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc                  [\x1e] [\x1f] Scroll up/down \xc4\xc4 or use mouse wheel");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd  _");
			nfo.push_back("      \xdf\xdf  (_)   - By specifying a filename as command line parameters (but");
			nfo.push_back("  \xdc\xdf\xdb\xdc  o         without space in the file's path) it will load a custom");
			nfo.push_back(" \xde_ \x22\xb0\xdd  \xdc\xdf\xdf\xdb\xdc    file. But keep in mind that was designed to show files");
			nfo.push_back("  \xdf\xdc\xdc\xdf  \xde  \x22\xb0\xb1\xdd   with only 80 chars width and it use a fork of the \x22""Terminal\x22");
			nfo.push_back(" \xf8    o \xde\xdc_  \xb0\xdd   windows font.");
			nfo.push_back(" \xdc\xfe\xdc     \xdf\xdb\xdc\xdc\xdf");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc        - By putting a file called \x22""readme.nfo\x22 at the root of the");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd   o     executable, it will load automatically.");
			nfo.push_back("\xdc\xdc\xdc\xdc\xdc\xdc\xdb\xdb\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc");
			nfo.push_back("  \xdc\xdf\xdb\xdc  o");
			nfo.push_back(" \xde_ \x22\xb0\xdd  \xdc\xdf\xdf\xdb\xdc      i'VE MADE THIS LITTLE 'TOOL' JUST FOR FUN. i CAN'T REMEMBER");
			nfo.push_back("  \xdf\xdc\xdc\xdf  \xde  \x22\xb0\xb1\xdd  o  WHEN THE IDEA TO MAKE IT OCCURRED TO ME, BUT ONE REASON IS");
			nfo.push_back(" \xf8    o \xde\xdc_  \xb0\xdd     THE CANCELED MAIN 2011. aFTER THIS BAD NEWS, I HAD TOTALLY");
			nfo.push_back(" \xdc\xfe\xdc     \xdf\xdb\xdc\xdc\xdf  _   ABANDONNED THE PROJECT I WAS WORKING ON. tO CHANGE MY BAD");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc       (_)  FEELINGS, I STARTED THIS PROJECT.");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd  _");
			nfo.push_back("      \xdf\xdf  (_)  i HAVE ALSO MET NEW PEOPLE IN A TEAM CALLED THETA AND THEY HAVE");
			nfo.push_back(" \xdc\xdf\xdb\xdc  o       MOTIVATED ME. iNITIALLY, THIS APP SHOULD HAVE BEEN FOR THEM. bUT");
			nfo.push_back("\xde_ \x22\xb0\xdd  \xdc\xdf\xdf\xdb\xdc  YOU CAN USE IT AND RE-USE IT AS YOU WISH. dISTRIBUTE IT, HACK ON");
			nfo.push_back(" \xdf\xdc\xdc\xdf  \xde  \x22\xb0\xb1\xdd IT IF YOU WANT, YOU CAN EVEN DESTROY IT OR HAVE SEX WITH IT, BUT");
			nfo.push_back("     o \xde\xdc_  \xb0\xdd ALWAYS FOR FREE.");
			nfo.push_back(" \xdc\xfe\xdc    \xdf\xdb\xdc\xdc\xdf");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc         pEOPLE ON #DEMOFR AND THE BBS");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd");
			nfo.push_back("      \xdf\xdf   _   lOTS OF THINGS HAPPEN THERE AND I REALLY LIKE TO CHAT WITH YOU");
			nfo.push_back(" \xdc\xdf\xdb\xdc  o  (_)  ON THE CHAN. hOPE THE BBS WILL GROW MORE AND MORE PEOPLE WILL");
			nfo.push_back("\xde_ \x22\xb0\xdd         COME. i LIKE TO READ ANY THREAD ABOUT OPENGL AND VARIOUS RENDER-");
			nfo.push_back(" \xdf\xdc\xdc\xdf   \xdc\xdf\xdf\xdb\xdc  ING TECHNIQUES... mOST OF THE TIME I REALIZE THAT I DON'T HAVE");
			nfo.push_back("     o \xde\xdc_  \xb0\xdd ENOUGH SKILLS TO MAKE 1/10TH OF THE THINGS... bUT STILL READ THE");
			nfo.push_back(" \xdc\xfe\xdc    \xdf\xdb\xdc\xdc\xdf  STUFF. nOW I CALL IT THE 'HQ'.");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd      iT'S REALLY COOL TO HAVE AN ANCHOR SOMEWHERE FOR OUR HOBBIES, TO");
			nfo.push_back("      \xdf\xdf   \xf8   LEARN, SHARE, MEET NEW CRAZY PEOPLE, AND WASTE TIME :) pLUS THE");
			nfo.push_back(" \xdc\xdf\xdb\xdc  o       SHORT DRINKS OF COURSE!!");
			nfo.push_back("\xde_ \x22\xb0\xdd  \xdc\xdf\xdf\xdb\xdc                          _");
			nfo.push_back(" \xdf\xdc\xdc\xdf  \xde  \x22\xb0\xb1\xdd  \xdc\xdc       _  o  _    o (_) o    o      KEEP UP THE GOOD WORK :D");
			nfo.push_back("     o \xde\xdc_  \xb0\xdd \xde_\xb0\xdd  _  (_)   (_) \xf8         O     \xf8");
			nfo.push_back(" \xdc\xfe\xdc    \xdf\xdb\xdc\xdc\xdf   \xdf\xdf  (_)                              O  \xdc\xfe\xdc");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc                                               \xde_\x22\xb0\xdd");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd      oTHER PEOPLE ALL OVER THE WORLDS APART   \xdf\xfe\xdf");
			nfo.push_back("      \xdf\xdf   _");
			nfo.push_back(" \xdc\xdf\xdb\xdc  o  (_)   KEEP UP THE GOOD WORK TOO!!!             \xdc\xfe\xdf\xdf\xdf\xdb\xdb\xfe\xdc");
			nfo.push_back("\xde_ \x22\xb0\xdd               _                                 \xdc\xdf    \x22\xb0\xb1\xb2\xb2\xdb\xdc");
			nfo.push_back(" \xdf\xdc\xdc\xdf   \xdc\xdf\xdf\xdb\xdc    _  (_) o           \xf8                 \xdc\xdf  \xb0    \xb0\xb0\xb1\xb2\xdb\xdc");
			nfo.push_back("     o \xde\xdc_  \xb0\xdd  (_)        O   \xf8                      \xdb   \xb0\xb0\xb1    \xb0\xb0\xb1\xdb");
			nfo.push_back(" \xdc\xfe\xdc    \xdf\xdb\xdc\xdc\xdf                                         \xdb\xdc_  \xb0\xb0\xb1    \xb0\xb0\xdb");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc                                              \xdf\xdb\xb2_   \xb0\xb1   \xb0\xdc\xdf");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd   hALL OF FAME (in alphabetical order)       \xdf\xdb\xb2\xb0_     \xb0\xdc\xdf");
			nfo.push_back("      \xdf\xdf                                                 \xdf\xfe\xdb\xdc\xdc\xdc\xdc\xfe\xdf");
			nfo.push_back(" \xdc\xdf\xdb\xdc  o      \xf8    Adoru  . . . . . . . . . . . . . . . . . . . . . . . .  ^o^");
			nfo.push_back("\xde_ \x22\xb0\xdd  \xdc\xdf\xdb\xdc");
			nfo.push_back(" \xdf\xdc\xdc\xdf  \xde \x22\xb0\xb1\xdd  Keops . . . . . . . . . . . . . . . . . . . . . . . . . SAIJEAN");
			nfo.push_back("     o  \xdf\xdb\xdc\xdf");
			nfo.push_back(" \xdc\xfe\xdc          LLB . . . . . . . . . . . . . . . . . . . . . . . . .  La flemme");
			nfo.push_back("\xde_\x22\xb0\xdd \xdc\xdc");
			nfo.push_back(" \xdf\xfe\xdf \xde_\xb0\xdd   loridaz  . . . . . . Mais non enfin, \x87""a n'a rien de compliqu\x82! <o>");
			nfo.push_back("  _   \xdf\xdf");
			nfo.push_back(" (_)       maracuja . . . . . . . . . . . . . . . . . . tout chez moi l'habite");
			nfo.push_back("       \xf8");
			nfo.push_back("  \xdc\xdf\xdf\xdb\xdc   MooZ . . . . . . . . . . . . . . . . . . . . . . . . . . . saimarine");
			nfo.push_back(" \xde\xdc_  \xb0\xdd");
			nfo.push_back("  \xdf\xdb\xdc\xdc\xdf   MsK`  . . . . . . . . . . . . . . . . . . . . . . . . . . . .  kapw\x82");
			nfo.push_back(" o");
			nfo.push_back("       o  ponce  . . . . . . . . . . . . . . . .  Le futur c'\x82""tait mieux avant");
			nfo.push_back(" \xdc\xdf\xdb\xdc");
			nfo.push_back("\xde_ \x22\xb0\xdd    Rez . . . . . . . . . . . . . . . . . . . . . pirate of the 777 seas");
			nfo.push_back(" \xdf\xdc\xdc\xdf   _");
			nfo.push_back("       (_) sam . . . . . . . . . . . . . . . . . . . . . . . . . FOR THE LULZ!");
			nfo.push_back("  \xdc\xdf\xdf\xdb\xdc");
			nfo.push_back(" \xde  \x22\xb0\xb1\xdd    skypers . . . . . . . . . . je me bats avec OpenGL sous mac, srsly");
			nfo.push_back(" \xde\xdc_  \xb0\xdd \xf8");
			nfo.push_back("  \xdf\xdb\xdc\xdc\xdf       titeiko  . . . . . . . . . j'pr\x82""f\x8a""re partir plut\x93""t qu'd'entendre");
			nfo.push_back("           o                                         \x87""a plut\x93""t qu'd'\x88""tre sourd");
			nfo.push_back(" \xdc\xdf\xdb\xdc  \xf8");
			nfo.push_back("\xde_ \x22\xb0\xdd  \xdc\xdf\xdf\xdb\xdc    zerkman  . . . . . . . . . .  je pr\x82""f\x8a""re l'explosion de caca!");
			nfo.push_back(" \xdf\xdc\xdc\xdf  \xde\xdc_  \xb0\xdd");
			nfo.push_back("  _     \xdf\xdb\xdc\xdc\xdf      aND ALSO guardian, wullon, xtrium, RaHoW, NoNameNo, MrDEN,");
			nfo.push_back(" (_)  \xdc\xdc             M`vy, Liandri, latortue, jegougou...");
			nfo.push_back("     \xde_\xb0\xdd   \xdc\xdf\xdb\xdc       AND CERTAINLY SOME OTHER PEOPLE I FORGOT (SRY).");
			nfo.push_back("      \xdf\xdf   \xde_ \x22\xb0\xdd");
			nfo.push_back(" \xdc\xdf\xdb\xdc    \xf8  \xdf\xdc\xdc\xdf  \xdc\xdf\xdf\xdb\xdc   \xdc\xdf\xdb\xdc  o   \xf8");
			nfo.push_back("\xde_ \x22\xb0\xdd  \xdc\xdc       \xde  \x22\xb0\xb1\xdd \xde_ \x22\xb0\xdd       cODE & gfx:               Msk:");
			nfo.push_back(" \xdf\xdc\xdc\xdf  \xde_\xb0\xdd \xf8    \xde\xdc_  \xb0\xdd  \xdf\xdc\xdc\xdf   \xdc\xdc   !!M / rOM#1   \xdc\xfe\xdf\xdf\xdb\xdb\xfe\xdc    dubmood    aNSI:");
			nfo.push_back("        \xdf\xdf     \xf8  \xdf\xdb\xdc\xdc\xdf         \xde_\xb0\xdd              \xdc\xdf   \x22\xb0\xb1\xb2\xb2\xdb\xdc            sam[D8]");
			nfo.push_back("    \xf8       \xdc\xdc\xdc\xdc                 \xdf\xdf    \xdc\xfe\xfe\xdc      \xdc\xdf      \xb0\xb0\xb1\xb2\xdb\xdc");
			nfo.push_back("\xdf\xdf\xdf\xdb\xdf\xdb\xdb\xdf\xdf\xdf\xdb\xdf \xb0\xb0\xb1\xdb\xdb\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xde_ \x22\xb0\xdd\xdf\xdf\xdf\xdf\xdf\xdb   \xb0\xb1    \xb0\xb0\xb1\xdb\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf");
			nfo.push_back("  \xde_ \x22\xb0\xdd \xde_    \xb0\xb1\xb0\xdd  O      \xf8          \xdf\xfe\xfe\xdf      \xdb\xdc_  \xb0\xb1    \xb0\xb0\xdb");
			nfo.push_back("   \xdf\xdc\xdc\xdf  \xde\xb2_      \xdd    \xdc\xdf\xdf\xdb\xdc     \xf8           o   \xdf\xdb\xb2_       \xb0\xdc\xdf       \xb3  _ \xb3");
			nfo.push_back("\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdb\xdb\xdc\xdc  \xdc\xdb\xdc\xdc\xdc\xdc\xde  \x22\xb0\xb1\xdd\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdb\xdb\xb2\xb0_    \xb0\xdc\xdb\xdc\xdc\xdc\xdc\xdc\xdc\xdc \xb3_(_)\xb3_ \xdc\xdc");
			nfo.push_back("            \xdf\xdf\xdf\xdf       \xdf\xdc\xdc\xdc\xdf                        \xdf\xfe\xdb\xdc\xdc\xdc\xfe\xdf");
			nfo.push_back("");

	}
}

SceneIntro::~SceneIntro()
{
	if(fileFound)
	{
		for(unsigned int i=0; i<nfo.size(); i++)
			delete(nfo[i]);
	}
	nfo.clear();
	delete(ctrlVBO);
	delete(texArrow);
	delete(autoScrollTex);
	delete(flash);
	delete(font);
	delete(fontTex);
	delete(icoVBO);
	delete(autoscrollVBO);
	delete(bkgVBO);
	delete(bkgTex[0]);
	delete(bkgTex[1]);
	delete(bkgTex[2]);
	delete(ribbonVBO);
}

bool SceneIntro::render(double aDelta)
{
	// Calculate some values
	float	width			= (float)this->window->getWidth();
	float	height			= (float)this->window->getHeight();
	float	ratio			= (float)this->window->getWidth() / this->window->getHeight();
	float	sin30			= sinf(((float)timeElapsed*30)*PI180);
	float	cos30			= cosf(((float)timeElapsed*30)*PI180);
	int		moduleRow		= HIWORD(uFMOD_GetRowOrder());
	int		moduleColumn	= LOWORD(uFMOD_GetRowOrder());
	timeElapsed	+= aDelta;

	// First flash
	if(moduleRow == 1)
	{
		if(moduleColumn == 15)
		{
				if(!leftFlag) flash->spawn();
		}
		// When flash is brightfull, show the left part
		if(flash->getState() == false) leftFlag = true;
	}

	// Time to change background?
	if(!changeBkg) if(((int)timeElapsed % 10) == 9) changeBkg = !changeBkg;
	if(changeBkg)
	{
		if(oldBkgOpacity > 0.1f)	oldBkgOpacity -= 0.01f;
		if(bkgOpacity < 0.9f)		bkgOpacity += 0.01f;
		if((oldBkgOpacity < 0.1f) && (bkgOpacity > 0.9f))
		{
			oldBkgOpacity	= 1.0f;
			bkgOpacity		= 0.0f;
			changeBkg		= false;
			bkgIndex++;
			if(bkgIndex == 3) bkgIndex = 0;
		}
	}

	// Synchro background with the sound
	if((moduleColumn % 16) == 15)
	{
		bkgOffset += 0.08f;
	}
	if((moduleColumn % 16) == 0)
	{
		if(bkgOffset > 0.0f) bkgOffset -= 0.05f;
	}
	if((moduleColumn % 16) == 1)
	{
		bkgOffset += 0.03f;
	}
	if((moduleColumn % 16) >= 2)
	{
		if(bkgOffset > 0.0f) bkgOffset -= 0.020f;
	}

	// Start drawing
	glClear				(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3d perspective
	glMatrixMode		(GL_PROJECTION);
	glLoadIdentity		();
	gluPerspective		(45.0f, ratio, 0.1f, 500.0f);
	glMatrixMode		(GL_MODELVIEW);
	glLoadIdentity		();
	glPushMatrix		();

	// Drawing background
	glTranslatef			(bkgOffset, bkgOffset, -10.0f);
	glTranslatef			(sin30, cos30, (cos30*sin30*(sin30*2))*10);
	glRotatef				(180, 0.0f, 1.0f, 0.0f);
	glRotatef				((sin30*0.5f)*180, 0.0f, 0.0f, 1.0f);
	glEnable				(GL_BLEND);
	bkgVBO->setState		();
	switch(bkgIndex)
	{
		case 0:
			glColor4f				(1.0f, 1.0f, 1.0f, oldBkgOpacity);
			glBindTexture			(GL_TEXTURE_2D, bkgTex[2]->getId());
			bkgVBO->render			();

			glColor4f				(1.0f, 1.0f, 1.0f, bkgOpacity);
			glBindTexture			(GL_TEXTURE_2D, bkgTex[0]->getId());
			bkgVBO->render			();
		break;

		case 1:
			glColor4f				(1.0f, 1.0f, 1.0f, oldBkgOpacity);
			glBindTexture			(GL_TEXTURE_2D, bkgTex[0]->getId());
			bkgVBO->render			();

			glColor4f				(1.0f, 1.0f, 1.0f, bkgOpacity);
			glBindTexture			(GL_TEXTURE_2D, bkgTex[1]->getId());
			bkgVBO->render			();
		break;

		case 2:
			glColor4f				(1.0f, 1.0f, 1.0f, oldBkgOpacity);
			glBindTexture			(GL_TEXTURE_2D, bkgTex[1]->getId());
			bkgVBO->render			();

			glColor4f				(1.0f, 1.0f, 1.0f, bkgOpacity);
			glBindTexture			(GL_TEXTURE_2D, bkgTex[2]->getId());
			bkgVBO->render			();
		break;

		default:
		break;
	}
	glBindTexture			(GL_TEXTURE_2D, 0);
	bkgVBO->unsetState		();
	glDisable				(GL_BLEND);

	// Show Ico
	if(leftFlag)
	{
		// Update icosahedron position & deform
		jumpIndex += 7;
		jumpIndex %= 540;
		if(jumpIndex < 100)
		{
			if(icoScaleX > 0.5f) icoScaleX -= 0.04f;
			if(icoScaleZ > 0.5f) icoScaleZ -= 0.04f;
		}
		else
		{
			if(icoScaleX < 1.0f) icoScaleX += 0.08f;
			if(icoScaleZ < 1.0f) icoScaleZ += 0.08f;
		}
		if(jumpIndex < 50)
		{
			if(icoScaleX > 0.5f) icoScaleX += 0.06f;
			if(icoScaleZ > 0.5f) icoScaleZ += 0.06f;
		}
		else
		{
			if(icoScaleX < 1.0f) icoScaleX -= 0.04f;
			if(icoScaleZ < 1.0f) icoScaleZ -= 0.04f;
		}
		if(jumpIndex > 350)
		{
			if(icoScaleY > 0.5f) icoScaleY -= 0.03f;
		}
		else if(icoScaleY < 1.0f)
		{
			icoScaleY += 0.03f;
		}
		if(icoOffsetY < -0.2f)	icoOffsetY+=0.1f;

		// Draw ico
		glPopMatrix				();
		glTranslatef			(3.0f+(cos30*sin30)*0.5f, icoOffsetY+(jumpTable[jumpIndex]*JUMP_AMPLITUDE), -7.0f);
		glScalef				(icoScaleX, icoScaleY, icoScaleZ);
		glRotatef				((float)timeElapsed*50, 1.0f, 0.0f, 0.0f);
		glRotatef				((float)timeElapsed*66, 0.0f, 1.0f, 0.0f);
		glRotatef				(-(cos30*45), 0.0f, 0.0f, 1.0f);
		glEnable				(GL_LIGHTING);
		icoVBO->setState		();
		icoVBO->render			();
		icoVBO->unsetState		();
		glDisable				(GL_LIGHTING);

		// Init 2d view
		glMatrixMode			(GL_PROJECTION);
		glLoadIdentity			();
		glOrtho					(0, width, height, 0, 0.0f, 1.0f);
		glMatrixMode			(GL_MODELVIEW);

		// Show the ribbon
		glLoadIdentity			();
		glTranslatef			(0.0f, height*0.5f, 0.0f);
		glRotatef				(-90.0f, 0.0f, 0.0f, 1.0f);
		glEnable				(GL_BLEND);
		ribbonVBO->setState		();
		ribbonVBO->update		((float)timeElapsed*50);
		ribbonVBO->render		();
		glTranslatef			(0.0f, -6.0f, 0.0f);
		ribbonVBO->render		();
		glTranslatef			(0.0f, -6.0f, 0.0f);
		ribbonVBO->render		();
		ribbonVBO->unsetState	();
		glDisable				(GL_BLEND);

		if(autoScroll)
		{
			if(((int)timeElapsed%10) == 9)
			{
				if(scrollPos < -((int)nfo.size()*CHAR_SIZE)) scrollPos = (int)height;
				scrollPos	-= 4;
				scrollFlag	 = true;
			}
		}
		
		if(scrollFlag)
		{
			if(((int)-scrollPos%(int)height) < 0)
			{
				scrollPos += (-(scrollPos*0.03f) - 0.1f);
			}
			else scrollFlag = false;
		}
		else if(scrollPos < -((int)nfo.size()*CHAR_SIZE))
		{
				scrollPos = (int)height+1;
				scrollFlag	 = true;
		}
		else if(scrollPos > (int) height)
		{
			scrollPos = (int)height+1;
			scrollFlag	 = true;
		}

		// print some text (find a way to improve that)
		glBindTexture(GL_TEXTURE_2D, fontTex->getId());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		float yOffset = 0.0f;
		for(unsigned int i=0; i<nfo.size(); i++)
		{
			float y = scrollPos+yOffset;
			if((y>-CHAR_SIZE) && (y<height))
			{
				glLoadIdentity			();
				glPushMatrix();
				/* Disable shadow because the background is too opaque (and speed rendering)
				glTranslatef			(CHAR_SIZE+2.0f, y+2.0f, 0.0f);
				glColor4f				(0.05f, 0.05f, 0.05f, 1.0f);
				font->print				(nfo[i]);
				*/
				glPopMatrix();
				glTranslatef			(CHAR_SIZE, y, 0.0f);
				glColor4f				(0.7f +(sinf((float)timeElapsed*2+(i*2)*PI180)*0.3f), 0.5f +(cosf((float)timeElapsed*1.5f+(i*3)*PI180)*0.5f), 0.5f +(sinf((float)timeElapsed+(i*4)*PI180)*0.5f), 1.0f);
				font->print				(nfo[i]);
			}
			yOffset					+= CHAR_SIZE;
		}
		glColor4f				(1.0f, 1.0f, 1.0f, 1.0f);

		// Draw ctrl
		glLoadIdentity			();
		glPushMatrix			();
		glTranslatef			(width*0.67f, 16.0f-upCtrlOffset-bkgOffset*10, 0.0f);
		ctrlVBO->setState		();
		glBindTexture			(GL_TEXTURE_2D, texArrow->getId());
		ctrlVBO->render			();

		glPopMatrix				();
		glTranslatef			(width*0.67f, 16.0f-upCtrlOffset-bkgOffset*10, 0.0f);
		glTranslatef			(0.0f,12.0f+downCtrlOffset+upCtrlOffset+(bkgOffset*10)*2, 0.0f);
		glRotatef				(180.0f, 0.0f, 0.0f, 1.0f);
		ctrlVBO->render			();
		ctrlVBO->unsetState		();

		if(autoScroll)
		{
			glLoadIdentity			();
			glTranslatef			(width*0.35f, (height-autoScrollTex->getWidth()*0.2f)-bkgOffset*20, 0.0f);
			glBindTexture			(GL_TEXTURE_2D, autoScrollTex->getId());
			autoscrollVBO->setState		();
			autoscrollVBO->render		();
			autoscrollVBO->unsetState	();
		}
		glDisable				(GL_BLEND);
		glBlendFunc				(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture			(GL_TEXTURE_2D, 0);

		if(upCtrlOffset > 0) upCtrlOffset -= 0.2f;
		if(downCtrlOffset > 0) downCtrlOffset -= 0.2f;
	}

	// flash
	if(flash->isAlive()) flash->render();
	return true;
}


void SceneIntro::onEnter()
{
	timeElapsed		= 0.0f;
	bkgOpacity		= 1.0f;
	oldBkgOpacity	= 0.0f;
	bkgIndex		= 0;
	changeBkg		= false;
	icoScaleX		= 1.0f;
	icoScaleY		= 1.0f;
	icoScaleZ		= 1.0f;
	scrollPos		= this->window->getHeight() + 1.0f;
	scrollFlag		= true;
	autoScroll		= true;
	leftFlag		= false;
	bkgOffset		= 0.0f;
	icoOffsetY		= -5.0f;
	fileFound		= false;

	// Reset viewport
	glViewport(0, 0, (GLsizei) this->window->getWidth(),(GLsizei)this->window->getHeight());

	//Init light param
	glEnable(GL_COLOR_MATERIAL);

	T4F ambient		= T4F(1.0f, 1.0f, 1.0f, 1.0f);
	T4F ambientDiff	= T4F(0.0f, 0.0f, 0.0f, 1.0f);
	T3F diffuse		= T3F(0.0f, 0.0f, 0.0f);

	SYSTEMTIME st;
	GetSystemTime(&st);
	T3F	specular;
	if(st.wHour>=0 && st.wHour<8) specular = T3F(0.0f, 0.6f, 0.6f);
	else if(st.wHour>=8 && st.wHour<16) specular = T3F(0.0f, 0.6f, 0.0f);
	else if(st.wHour>=16 && st.wHour<=23) specular = T3F(0.6f, 0.0f, 0.6f);
	else specular = T3F(0.6f, 0.0f, 0.6f);

	glMaterialfv	(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,	(GLfloat*)&ambientDiff);
	glMaterialfv	(GL_FRONT, GL_DIFFUSE,				(GLfloat*)&diffuse);
	glMaterialfv	(GL_FRONT, GL_SPECULAR,				(GLfloat*)&specular);
	glEnable		(GL_LIGHT0);

	T3F	lightPos	= T3F(3.0f, 20.0f, -0.0f);
	glLightModelfv	(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)&ambient);
	glLightfv		(GL_LIGHT0, GL_POSITION, (GLfloat*)&lightPos);
	glLightf		(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);

	glClearColor	(1.0f, 1.0f, 1.0f, 1.0f);
}

void SceneIntro::onExit()
{}

void SceneIntro::onKeyPressed(unsigned int aKey)
{
	switch (aKey)
	{
		// Handle keys pressed
		case VK_DOWN:
			if(!scrollFlag) scrollPos -= CHAR_SIZE;
			upCtrlOffset	= 5;
			if(autoScroll) autoScroll = !autoScroll;
		break;

		case VK_UP:
			if(!scrollFlag) scrollPos += CHAR_SIZE;
			downCtrlOffset	= 5;
			if(autoScroll) autoScroll = !autoScroll;
		break;

		case VK_NEXT:
			if(!scrollFlag) scrollPos -= CHAR_SIZE*10;
			upCtrlOffset	= 5;
			if(autoScroll) autoScroll = !autoScroll;
		break;

		case VK_PRIOR:
			if(!scrollFlag) scrollPos += CHAR_SIZE*10;
			downCtrlOffset	= 5;
			if(autoScroll) autoScroll = !autoScroll;
		break;

		// Hidden stuff
		case VK_F1:
			if(leftFlag && !flash->isAlive()) flash->spawn();
			changeBkg = !changeBkg;
		break;

		// Hidden stuff
		case VK_F2:
			autoScroll = !autoScroll;
		break;
	}
}

