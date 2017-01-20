// ExeFileName.cpp: implementation of the ExeFileName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExeFileName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

char ExeFileName::m_szPath[_MAX_PATH];
char ExeFileName::m_szExt[_MAX_EXT];
char ExeFileName::m_szFname[_MAX_FNAME];
char ExeFileName::m_szDir[_MAX_DIR];
char ExeFileName::m_szDrive[_MAX_DRIVE];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExeFileName::ExeFileName()
{
   char szCmdLine[512];
   char*pcExeFileEnd;

   strcpy_s(szCmdLine, GetCommandLine());
   pcExeFileEnd = strchr(szCmdLine+1, '\"');

   strncpy_s(m_szPath, (szCmdLine+1), pcExeFileEnd-(szCmdLine+1));
   m_szPath[pcExeFileEnd-(szCmdLine+1)] = 0;
   _splitpath_s(m_szPath, m_szDrive, m_szDir, m_szFname, m_szExt);
}

ExeFileName::~ExeFileName()
{

}
