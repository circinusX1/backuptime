////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005, General Electric Co.      GE Confidential and Proprietary
//
// PRODUCT	    GE Power Management - EnerVista Viewpoint Maintenance
//
// FILE			ZipManager.h
//
// VERSION		1.0
//
// AUTHOR		
//
// DESCRIPTION	Header file 
//	Manages Zipping, Splitting and Mailing the Folders, Files as part of Fault Diagnostics Operation.
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZIPMANAGER_H__A151E381_EC51_4C06_AA71_94A1D4A68347__INCLUDED_)
#define AFX_ZIPMANAGER_H__A151E381_EC51_4C06_AA71_94A1D4A68347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipArchive.h"
#include <afxtempl.h>

struct CAddInfo
{
	CAddInfo(){}
	CAddInfo(const CString& strName, DWORD dwSize, bool bDir = false)
		:m_strName(strName), m_dwSize(dwSize), m_bDir(bDir){}
	CString m_strName;
	bool m_bDir;
	DWORD m_dwSize;
};
typedef CList<CAddInfo, CAddInfo> CAddInfoList;


class CZipManager  
{
public:
	CZipManager();
	virtual ~CZipManager();

protected:
	BOOL				m_bAfterException;
	BOOL				m_bRecurse;
	CString				m_strSourceFolderPath;
	CString				m_strDestPathFileName;
	CZipArchive			m_zip;

	BOOL				m_bSingleFile;
	CStringArray		m_arFileNames;

	DWORD				m_dwSplitFileSize;

protected:
	BOOL	OnArchiveNew();
	BOOL	OnArchiveClose();
	BOOL	OnActionAddFolder();
	void	AddFolder(CString szFolder, CAddInfoList& list);

	BOOL SplitFile();
public:
	BOOL SendMail(CString strDeviceNm, CString strOrderCode,CString strVersion, CString strSerialNumber);
	
public:
	void SetSourceFolderPath(CString&);
	void SetDestPathFileName(CString&);

	BOOL	ZipFolder();//CString strDeviceNm, CString strDeviceOrderCode, CString strVersion, CString strDeviceSerialNumber);
	BOOL	ZipFiles();
	BOOL	MendFiles();

	BOOL SetSplitSize(DWORD& dwSplitFileSize);

};

#endif // !defined(AFX_ZIPMANAGER_H__A151E381_EC51_4C06_AA71_94A1D4A68347__INCLUDED_)
