////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005, General Electric Co.      GE Confidential and Proprietary
//
// PRODUCT	    GE Power Management - EnerVista Viewpoint Maintenance
//
// FILE			ZipManager.cpp
//
// VERSION		1.0
//
// AUTHOR		
//
// DESCRIPTION	implementation file 
//
////////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "..\URPC.h"
#include "ZipManager.h"
#include "..\Split_Mend_DependentFiles\FileSplit.h"
#include "..\Split_Mend_DependentFiles\FileMend.h"
#include "..\EMailSrv.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZipManager::CZipManager()
{
	m_strSourceFolderPath	= "";
	m_strDestPathFileName	= "";
	m_dwSplitFileSize		= 1024;//1 KB
	
	m_bSingleFile			=	TRUE;
	
	m_arFileNames.RemoveAll();
}

CZipManager::~CZipManager()
{

}

BOOL CZipManager::ZipFolder()//CString strDeviceNm, CString strDeviceOrderCode, CString strVersion,CString strDeviceSerialNumber) 
{
	if(m_strSourceFolderPath.IsEmpty())
	{
		AfxMessageBox("Specify the source folder path...");
		return FALSE;
	}

	if(m_strDestPathFileName.IsEmpty())
	{
		AfxMessageBox("Specify the Destination path file name...");
		return FALSE;
	}

	CWaitCursor cursor;
	m_bAfterException = false;
	m_bRecurse = true;
	
	m_zip.SetAdvanced(1500000);

	if(!OnArchiveNew())
		return FALSE;

	if(!OnActionAddFolder())
		return FALSE;

	if(!OnArchiveClose())
		return FALSE;

	if(!SplitFile())
		return FALSE;

	if(m_bSingleFile)
	{
		//
		m_arFileNames.Add(m_strDestPathFileName);
	}
	else
	{
		//prepare the list of file names with full path present in the folder delimited with ";"
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		CString strDir = "";


		_splitpath( m_strDestPathFileName, drive, dir, fname, ext );
		strDir.Format("%s%s", drive, dir);

		CString	strFilePattern = "";
		strFilePattern.Format("%s%s", fname, ext);

		//BOOL b = ff.FindFile(strDir + _T("\\*"));
		CFileFind ff;
		BOOL b = ff.FindFile(m_strDestPathFileName + _T(".*"));
		while (b)
		{
			b = ff.FindNextFile();
			if (ff.IsDots())
				continue;
			if (ff.IsDirectory())
			{
				continue;
			}
			else
			{
				CString strFN = "";
				strFN = ff.GetFilePath();
				m_arFileNames.Add(strFN);
			}
		}
	}

	if(m_arFileNames.GetSize() == 0)
		return FALSE;

//	SendMail(strDeviceNm, strDeviceOrderCode, strVersion, strDeviceSerialNumber);

	return TRUE;
}

BOOL CZipManager::OnArchiveNew() 
{
	OnArchiveClose();
	try
	{
		m_zip.Open(m_strDestPathFileName, CZipArchive::create, 0);
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Create new failed"), MB_ICONSTOP);
		OnArchiveClose();
		return FALSE;
	}

	return TRUE;
}

BOOL CZipManager::OnArchiveClose() 
{
	bool berr = false;
	try
	{
		m_zip.Close(m_bAfterException);	
	}
	catch (CException* e)
	{
		e->Delete();
		berr = true;
	}
	catch(...) // thrown in the STL version
	{
		berr = true;		
	}
	if (berr)
		AfxMessageBox(_T("Close failed"), MB_ICONSTOP);
	
	if (m_bAfterException)
		m_bAfterException = false;

	if(berr)
		return FALSE;

	return TRUE;
}

BOOL CZipManager::OnActionAddFolder() 
{
	m_zip.SetPassword("MAINT2005VP");

	CAddInfoList list;
	AddFolder(m_strSourceFolderPath, list);
	BOOL bIfArchiveFound;

	try
	{
		POSITION pp = list.GetHeadPosition();

		while (pp)
		{
			bIfArchiveFound = FALSE;
			CAddInfo* padi = &list.GetNext(pp);
			CString strFileToAdd;
			strFileToAdd = padi->m_strName;
			
			int nPos = strFileToAdd.Find(".zip");

			if ((nPos > -1)) 
			{
				nPos = strFileToAdd.Find("\\Archive_");

				if (nPos > -1)
					bIfArchiveFound = TRUE;
			}

			if (!bIfArchiveFound)
			{
				//m_zip.AddNewFile(padi->m_strName, padi->m_bDir ? 0 : 9/*m_options.m_iLevel*/, true);
				m_zip.AddNewFile(padi->m_strName, 9, true,NULL,NULL,65535);
			}
		}
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;		
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Add folder failed.  Closing the archive."), MB_ICONSTOP);
		OnArchiveClose();
		return FALSE;
	}

	return TRUE;
}

void CZipManager::AddFolder(CString szFolder, CAddInfoList& list)
{
	szFolder.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(szFolder + _T("\\*"));
	CStringArray folders;
	while (b)
	{
		b = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			if  (!m_bRecurse)
				continue;
			folders.Add(ff.GetFilePath());
		}
		else
		{
			CAddInfo ai;
			ai.m_strName = ff.GetFilePath();
			CFileStatus fs;
			if (CFile::GetStatus(ai.m_strName, fs))
			{
				ai.m_bDir = false;
				ai.m_dwSize = fs.m_size;
				list.AddTail(ai);
			}
		}

	}
	for (int i = 0; i < folders.GetSize(); i++)
	{
		list.AddTail(CAddInfo(folders[i], 0, true));
												// add the folder before adding its files
												// it is not needed to add the root folder
		AddFolder(folders[i], list);
	}
}

void CZipManager::SetSourceFolderPath(CString&	strSourceFolderPath)
{
	m_strSourceFolderPath = strSourceFolderPath;
}

void CZipManager::SetDestPathFileName(CString&	strDestPathFileName)
{
	m_strDestPathFileName = strDestPathFileName;
}


BOOL CZipManager::ZipFiles()
{
	CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"));
	fd.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (fd.DoModal() != IDOK)
		return FALSE;

	m_zip.SetPassword("");//m_options.m_szPassword);
	POSITION pp = fd.GetStartPosition();
	CAddInfoList list;
	while (pp)
	{
		CString sz = fd.GetNextPathName(pp);
		CFileStatus fs;
		if (CFile::GetStatus(sz, fs))
		{
			list.AddTail(CAddInfo(sz, fs.m_size));
		}
	}

	try
	{
		pp = list.GetHeadPosition();
		while (pp)
		{
			CAddInfo* padi = &list.GetNext(pp);
			m_zip.AddNewFile(padi->m_strName, 9/*m_options.m_iLevel*/, true);
		}
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Add files failed.  Closing the archive."), MB_ICONSTOP);
		OnArchiveClose();
	}

	return TRUE;
}

BOOL CZipManager::SetSplitSize(DWORD& dwSplitFileSize)
{
	m_dwSplitFileSize = dwSplitFileSize;

	return TRUE;
}

BOOL CZipManager::SplitFile()
{
	
	try
	{
		CFile file;
		if(!file.Open(m_strDestPathFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | 
					CFile::shareExclusive, NULL))
		{
			return FALSE;
		}
		
		if(file.GetLength() < m_dwSplitFileSize)
		{
			m_bSingleFile = TRUE;
			return TRUE;
		}
	}
	catch (CFileException& e)
	{
		e.Delete();
		return FALSE;
	}
	catch(...) // thrown in the STL version
	{
		return FALSE;
	}

	//because the file got split, this flag is set to false
	m_bSingleFile = FALSE;

	CFileSplit FileMgr;
	
	if (!FileMgr.SetFilePath(m_strDestPathFileName)) 
	{
		
	  AfxMessageBox("Error: File does not exist ..An error has occurred", MB_ICONHAND);
	}
	
	// Set the destination folder
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strDir = "";


	_splitpath( m_strDestPathFileName, drive, dir, fname, ext );
	strDir.Format("%s%s", drive, dir);
	FileMgr.SetDestPath(strDir);
	
	// Assign a split size of 1 kb
	FileMgr.SetSplitSize(m_dwSplitFileSize);
	BOOL bErr = FALSE;
	
	bErr = FileMgr.Split();

	if(bErr)
	{
		AfxMessageBox("File split successful! ...Success", MB_OK);
	} 
	else 
	{
		AfxMessageBox("File split failed....An error has occurred", MB_ICONHAND);
	}

	try
	{
		CFile::Remove(m_strDestPathFileName);
	}
	catch (CFileException& e)
	{
		e.Delete();
		return FALSE;
	}
	catch(...) // thrown in the STL version
	{
		return FALSE;
	}

	return bErr;
}

BOOL CZipManager::MendFiles()
{
	CFileMend FileMgr;
	
	// Set the file to restore, in this case
	// we are mending all MyFile.ext pieces:
	// Set the destination folder
	CString strFirstFileName = m_strDestPathFileName + ".001";
	
	if (!FileMgr.SetPiecePath(strFirstFileName)) 
	{
		//TODO: Halt function after displaying message box
		AfxMessageBox("Error: File does not exist...An error has occurred", MB_ICONHAND);
	} 
	
	// Set the destination folder
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strDir = "";


	_splitpath( m_strDestPathFileName, drive, dir, fname, ext );
	strDir.Format("%s%s", drive, dir);
	FileMgr.SetDestPath(strDir);

	if (FileMgr.Mend()) 
	{
		AfxMessageBox("Mending completed successfully!...Success!", MB_OK);
	} else {
		AfxMessageBox("Mending failed....Failure.", MB_OK);
	}

	return TRUE;
}

BOOL CZipManager::SendMail(CString strDeviceNm, CString strOrderCode,CString strVersion, CString strSerialNumber)
{
	//smtp server not configured
	if (theApp.m_sRegistry.m_strSMTP.GetLength() == 0)
		return FALSE;

	LPCTSTR pszSMTP = _T(theApp.m_sRegistry.m_strSMTP);
	IEMailService* pMailSrv = ::GetEMailService(EASY_SMTPOBJ_SERVICE, (void *)pszSMTP);
	if (pMailSrv != NULL)
	{
		CString strTitleOfMail, strEmailBody;
		strTitleOfMail = "Maintenance email for the Relay : " + strDeviceNm;
		strEmailBody = "During Fault Diagnostics Operation the Relay has the following information :\r\n\r\n";
		strEmailBody += "Order Code       : " + strOrderCode;
		strEmailBody += "\r\n";
		strEmailBody += "Firmware Version : " + strVersion; 
		strEmailBody += "\r\n";
		strEmailBody += "Serial Number    : " + strSerialNumber; 
		
		strEmailBody += "\r\n\r\nContact Information";
		strEmailBody += "\r\n";
		strEmailBody += "Name    : " + theApp.m_sRegistry.m_strFromName; 
		strEmailBody += "\r\n";
		strEmailBody += "Company : " + theApp.m_sRegistry.m_strFromCompany; 
		strEmailBody += "\r\n";
		strEmailBody += "Phone   : " + theApp.m_sRegistry.m_strFromPhone; 

		strEmailBody += "\r\n\r\n";
		strEmailBody += "\r\n\r\nThis Information is generated by Viewpoint Maintenance through Fault Diagnostics Operation.";
		
		
		pMailSrv->SetEMailServer(pszSMTP);
		return pMailSrv->SendMessage(_T(theApp.m_sRegistry.m_strEmailAddress), _T("EnerVista PC"), _T(theApp.m_sRegistry.m_strFromEmailAddress), _T(strTitleOfMail), _T(strEmailBody), FALSE, m_arFileNames);
	}

	return TRUE;
}
