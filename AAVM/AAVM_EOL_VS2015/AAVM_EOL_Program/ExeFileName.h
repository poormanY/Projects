// ExeFileName.h: interface for the ExeFileName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXEFILENAME_H__B57F2B32_BF69_4A87_BB89_E875B230B7CB__INCLUDED_)
#define AFX_EXEFILENAME_H__B57F2B32_BF69_4A87_BB89_E875B230B7CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ExeFileName
{
public:
	static char m_szPath[_MAX_PATH];
	static char m_szExt[_MAX_EXT];
	static char m_szFname[_MAX_FNAME];
	static char m_szDir[_MAX_DIR];
	static char m_szDrive[_MAX_DRIVE];
	ExeFileName();
	virtual ~ExeFileName();

};

#endif // !defined(AFX_EXEFILENAME_H__B57F2B32_BF69_4A87_BB89_E875B230B7CB__INCLUDED_)
