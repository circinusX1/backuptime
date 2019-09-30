//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include <stdarg.h>
#include "backupiime.h"
#include "Ftp_Client.h"



Ftp_Client::Ftp_Client()
{
    m_hWndMain = 0;
    p_FtpConnection = 0;
    p_INETSESS = 0;
    v_lastList.reserve(256);
    m_dwLastRespInfo = 0;
}

Ftp_Client::~Ftp_Client()
{
    Close();
    v_lastList.clear();
}

void    Ftp_Client::SetLogOutWindow(HWND h)
{
    m_hWndMain = h;
    if(p_INETSESS)
        p_INETSESS->m_pMainWnd = h;
}

const TCHAR* Ftp_Client::GetLastResponceInfo()
{
	DWORD  dwLength = 255, dwError;
	static TCHAR strInfo[256];
	InternetGetLastResponseInfo(&dwError, strInfo, &dwLength);
    m_dwLastRespInfo = dwError;
	return strInfo;
}

void    Ftp_Client::Close()
{
    v_lastList.clear();
    if(p_FtpConnection)
        p_FtpConnection->Close();
    if(p_INETSESS)
        p_INETSESS->Close();
    
    delete p_INETSESS;
    delete p_FtpConnection;
    p_INETSESS = 0;
    p_FtpConnection = 0;
    
}

BOOL Ftp_Client::OpenSession(LPCTSTR appName, HWND hWnd, DWORD preconfig/*=PRE_CONFIG_INTERNET_ACCESS*/)
{
    m_hWndMain = hWnd;
    if(p_INETSESS)
        Close();
    p_INETSESS = new CNetSess(appName, 1, preconfig);
    if(0 == p_INETSESS)
    {
        return FALSE;
    }
    p_INETSESS->m_pMainWnd = hWnd;
    p_INETSESS->EnableStatusCallback();
	int nTimeout = 60;
    try{
	    p_INETSESS->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeout * 1000);
	    p_INETSESS->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, nTimeout * 1000);
	    p_INETSESS->SetOption(INTERNET_OPTION_SEND_TIMEOUT, nTimeout * 1000);
    }catch(...)
    {
        Close();
        return FALSE;
    }
    return TRUE;
}


BOOL Ftp_Client::Connect(LPCTSTR hostname, LPCTSTR szlogin, 
                         LPCTSTR password, UINT innport, BOOL pasivemode)
{
    CString strObject, serverName;
    DWORD   dwServiceType = 0;

    WORD    nport = (WORD)innport;
	if (!AfxParseURL(hostname, dwServiceType, serverName, strObject, nport))
	{
		CString strFtpURL = _T("ftp://");
		strFtpURL += hostname;

		if (!AfxParseURL(strFtpURL, dwServiceType, serverName, strObject, nport))
		{
            LogOut("Invalid Server Name \r\nTry: ftp://<server> or ftp://<IP> or just <IP>", serverName);
            Close();
			return FALSE;
		}
	}

    if (dwServiceType == INTERNET_SERVICE_FTP && 
         !serverName.IsEmpty())
    {
        try{
	        p_FtpConnection = p_INETSESS->GetFtpConnection(serverName, 
                                                           szlogin, 
                                                           password, 
                                                           nport, 
                                                           pasivemode);
        }
        catch (CInternetException* pEx)
        {
            TCHAR   szerror[256] = {0};
            DWORD   length = 254;
            DWORD   dwError = 0;

			InternetGetLastResponseInfo(&dwError, szerror, &length);
            if (::strlen(szerror))
            {
				LogOut(szerror);
            }
			// catch errors from WinINet
			if (pEx->GetErrorMessage(szerror, 254))
			{
                LogOut(szerror);
			}
            LogOut("Error Openning Connection to: %s\r\n", serverName);
			pEx->Delete();
            Close();
            return FALSE;
        }
        catch(...)
        {
            return FALSE;
        }
        return TRUE;
    }
    LogOut("Invalid Server Name ");
    return FALSE;
}


BOOL Ftp_Client::SetCurrentDirectory(LPCTSTR strPath)
{
    if(0==p_FtpConnection)
        return FALSE;
	if (!p_FtpConnection->SetCurrentDirectory(strPath))
	{
        m_dwLastRespInfo=0;
		LogOut(GetLastResponceInfo());
		return FALSE;
	}
    LogOut("Receiving listing of \"%s\"...\r\n", strPath);
    sz_lastdir = strPath;
    return TRUE;
}

void    Ftp_Client::LogOut(LPCTSTR p,...)
{

    static TCHAR _s[512];
    try{
        if(m_hWndMain && ::IsWindow(m_hWndMain))
        {
            va_list args;
            va_start(args, p);
            ::_vsnprintf(_s, sizeof(_s) / sizeof(TCHAR), p, args);
            va_end(args);
            ::SendMessage(m_hWndMain, WM_FTP_LOGTEXT, 0, (LPARAM)_s);
        }
    }
    catch(...)
    {
    }
}

BOOL    Ftp_Client::CreateDirectory(LPCTSTR szDir)
{
	if (!p_FtpConnection)
		return FALSE;
    if(p_FtpConnection->CreateDirectory(szDir))
    {
        return TRUE;
    }
    DWORD dw = GetLastError();
    m_dwLastRespInfo=0;
    GetLastResponceInfo();
    if(0==m_dwLastRespInfo)
        m_dwLastRespInfo = dw;
    return FALSE;
}


BOOL Ftp_Client::IsStillConnected()
{
	if (!p_FtpConnection)
		return FALSE;

	CString strDirName;
	if (p_FtpConnection->GetCurrentDirectory(strDirName))
	{
		LogOut("FTP status: Connected.");
		return TRUE;
	}
    Close();
    LogOut("FTP status: Close by remote... !");
	return FALSE;
}


const vector<WIN32_FIND_DATA>&   Ftp_Client::ListLocation(LPCTSTR szDirectory)
{
    v_lastList.clear();

    if(p_FtpConnection==0)
        return v_lastList; 

    if(szDirectory)
    {
        if(!p_FtpConnection->SetCurrentDirectory(szDirectory))
        {
            LogOut(MKSTR("Cannot Change Directory To '%s'.", szDirectory));
            return v_lastList;
        }
    }

 	CFtpFileFind ftpFind(p_FtpConnection);
	BOOL bContinue = ftpFind.FindFile("");
	if (!bContinue)
	{
		DWORD dwError = GetLastError();
		ftpFind.Close();
		if (dwError == ERROR_INTERNET_TIMEOUT)
			LogOut("The request has Timed Out.");
		else
			LogOut(MKSTR("\"%s\" is empty.", szDirectory));
		return v_lastList;
	}

    try{
	    while (bContinue) 
	    {
		    bContinue = ftpFind.FindNextFile();
		    if (ftpFind.IsDots())
			    continue;

            WIN32_FIND_DATA dwd = {0};

            ftpFind.GetCreationTime(&dwd.ftCreationTime);
            ftpFind.GetLastWriteTime(&dwd.ftLastWriteTime);
            dwd.nFileSizeHigh   =    0;
            dwd.nFileSizeLow    =    ftpFind.GetLength();
            //if(ftpFind.IsNormal())
            {
                ::_tcscpy(dwd.cFileName, ftpFind.GetFileName());
        
                CString& fullpath    =    ftpFind.GetFileURL();

                dwd.dwFileAttributes |= ftpFind.IsReadOnly()   ? FILE_ATTRIBUTE_READONLY  : 0;
                dwd.dwFileAttributes |= ftpFind.IsDirectory()  ? FILE_ATTRIBUTE_DIRECTORY : 0;
                dwd.dwFileAttributes |= ftpFind.IsSystem()     ? FILE_ATTRIBUTE_SYSTEM    : 0;
                v_lastList.push_back(dwd);
            }
	    }
    }catch(...)
    {
        LogOut("Exception ListLocation. \r\n");
    }
	ftpFind.Close();
    return v_lastList;
}


void CNetSess::OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLen)
{
	static TCHAR strStatus[256];

    strStatus[0]=0;

	switch(dwInternetStatus)
	{
	case INTERNET_STATUS_STATE_CHANGE:	
        ::_tcscpy(strStatus," Busy");
		break;

	case INTERNET_STATUS_REQUEST_COMPLETE:
		::_tcscpy(strStatus," request complete");
		break;
	case INTERNET_STATUS_CONNECTING_TO_SERVER:
		::_tcscpy(strStatus," Connecting to server...");
		break;

	case INTERNET_STATUS_RESOLVING_NAME:
		::_stprintf(strStatus, " resolving name for %s", lpvStatusInformation);
		break;

	case INTERNET_STATUS_NAME_RESOLVED:	
		::_stprintf(strStatus, " resolved name for %s!", lpvStatusInformation);
		break;

	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		::_tcscpy(strStatus," Connected to server!");
		break;

	case INTERNET_STATUS_SENDING_REQUEST:	
		::_tcscpy(strStatus," Sending request...");
		break;

	case INTERNET_STATUS_REQUEST_SENT:	
		::_tcscpy(strStatus," Request sent");
		break;
	case INTERNET_STATUS_RECEIVING_RESPONSE:
		::_tcscpy(strStatus," Receiving response...");
		break; 
	case INTERNET_STATUS_RESPONSE_RECEIVED:	
		::_tcscpy(strStatus," Response received!");
		break;
	case INTERNET_STATUS_CLOSING_CONNECTION:
		::_tcscpy(strStatus," Closing connection...");
		break;
	case INTERNET_STATUS_CONNECTION_CLOSED:	
		::_tcscpy(strStatus," Connection close!");
		break;
	case INTERNET_STATUS_HANDLE_CLOSING:
		::_tcscpy(strStatus," Closing Connection...");
		break;
	case INTERNET_STATUS_HANDLE_CREATED:
		::_tcscpy(strStatus," Connection created!");
		break;
	case INTERNET_STATUS_REDIRECT:
		::_tcscpy(strStatus," Redirected to URL");
		break;
	case INTERNET_STATUS_CTL_RESPONSE_RECEIVED:
	default:
		::_stprintf(strStatus," Unknown status: %d", dwInternetStatus);
		break;
	}

	if(m_pMainWnd && IsWindow(m_pMainWnd) && strStatus[0])
    {
		::SendMessage(m_pMainWnd, WM_FTP_LOGTEXT, 0, (LPARAM)strStatus);
    }
}




const TCHAR* CNetSess::GetErrorString(DWORD dwErrorCode)
{
	static TCHAR   strResult[256];
	
	switch(dwErrorCode)
	{
		case ERROR_INTERNET_OUT_OF_HANDLES:
			::_tcscpy(strResult,"No more handles could be generated at this time.");
			break;
		case ERROR_INTERNET_TIMEOUT:
            ::_tcscpy(strResult,"The request has timed out.");
			break;
		case ERROR_INTERNET_EXTENDED_ERROR:
            // An extended error was returned from the server. This is
			//	typically a string or buffer containing a verbose error
            //   message. Call InternetGetLastResponseInfo to retrieve the error text.
		case ERROR_INTERNET_INTERNAL_ERROR:
			::_tcscpy(strResult,"An internal error has occurred.");
			break;
		case ERROR_INTERNET_INVALID_URL:
			::_tcscpy(strResult,"The URL is invalid.");
			break;
		case ERROR_INTERNET_UNRECOGNIZED_SCHEME:
			::_tcscpy(strResult,"The URL scheme could not be recognized or is not supported.");
			break;
		case ERROR_INTERNET_NAME_NOT_RESOLVED:
			::_tcscpy(strResult,"The server name could not be resolved.");
			break;
		case ERROR_INTERNET_PROTOCOL_NOT_FOUND:
			::_tcscpy(strResult,"The requested protocol could not be located.");
			break;
		case ERROR_INTERNET_INVALID_OPTION:
			::_tcscpy(strResult,"A request to InternetQueryOption or InternetSetOption specified an invalid option value.");
			break;
		case ERROR_INTERNET_BAD_OPTION_LENGTH:
			::_tcscpy(strResult,"The length of an option supplied to InternetQueryOption or "
						"InternetSetOption is incorrect for the type of option specified.");
			break;
		case ERROR_INTERNET_OPTION_NOT_SETTABLE:
			::_tcscpy(strResult,"The request option cannot be set, only queried.");
			break;
		case ERROR_INTERNET_SHUTDOWN:
			::_tcscpy(strResult,"The Win32 Internet function support is being shut down or unloaded.");
			break;
		case ERROR_INTERNET_INCORRECT_USER_NAME:
			::_tcscpy(strResult,"The request to connect and log on to an FTP server could "
						"not be completed because the supplied user name is incorrect.");
			break;
		case ERROR_INTERNET_INCORRECT_PASSWORD:
			::_tcscpy(strResult,"The request to connect and log on to an FTP server could "
						"not be completed because the supplied password is incorrect.");
			break;
		case ERROR_INTERNET_LOGIN_FAILURE:
			::_tcscpy(strResult,"The request to connect to and log on to an FTP server failed.");
			break;
		case ERROR_INTERNET_INVALID_OPERATION:
            ::_tcscpy(strResult,"The requested operation is invalid.");
			break;
		case ERROR_INTERNET_OPERATION_CANCELLED:
            ::_tcscpy(strResult,"The operation was canceled, usually because the handle on "
						"which the request was operating was closed before the operation completed.");
			break;
		case ERROR_INTERNET_INCORRECT_HANDLE_TYPE:
            ::_tcscpy(strResult,"The type of handle supplied is incorrect for this operation.");
			break;
		case ERROR_INTERNET_INCORRECT_HANDLE_STATE:
            ::_tcscpy(strResult,"The requested operation cannot be carried out because the "
						"handle supplied is not in the correct state. ");
			break;
		case ERROR_INTERNET_NOT_PROXY_REQUEST:
            ::_tcscpy(strResult,"The request cannot be made via a proxy.");
			break;
		case ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND:
            ::_tcscpy(strResult,"A required registry value could not be located.");
			break;
		case ERROR_INTERNET_BAD_REGISTRY_PARAMETER:
            ::_tcscpy(strResult,"A required registry value was located but is an incorrect type or has an invalid value.");
			break;
		case ERROR_INTERNET_NO_DIRECT_ACCESS:
            ::_tcscpy(strResult,"Direct network access cannot be made at this time.");
			break;
		case ERROR_INTERNET_NO_CONTEXT:
            ::_tcscpy(strResult,"An asynchronous request could not be made because a zero context value was supplied.");
			break;
		case ERROR_INTERNET_NO_CALLBACK:
            ::_tcscpy(strResult,"An asynchronous request could not be made because a callback function has not been set.");
			break;
		case ERROR_INTERNET_REQUEST_PENDING:
            ::_tcscpy(strResult,"The required operation could not be completed because one or more requests are pending.");
			break;
		case ERROR_INTERNET_INCORRECT_FORMAT:
            ::_tcscpy(strResult,"The format of the request is invalid.");
			break;
		case ERROR_INTERNET_ITEM_NOT_FOUND:
            ::_tcscpy(strResult,"The requested item could not be located.");
			break;
		case ERROR_INTERNET_CANNOT_CONNECT:
            ::_tcscpy(strResult,"The attempt to connect to the server failed.");
			break;
		case ERROR_INTERNET_CONNECTION_ABORTED:
            ::_tcscpy(strResult,"The connection with the server has been terminated.");
			break;
		case ERROR_INTERNET_CONNECTION_RESET:
            ::_tcscpy(strResult,"The connection with the server has been reset.");
			break;
		case ERROR_INTERNET_FORCE_RETRY:
            ::_tcscpy(strResult,"Calls for the Win32 Internet function to redo the request.");
			break;
		case ERROR_INTERNET_INVALID_PROXY_REQUEST:
            ::_tcscpy(strResult,"The request to the proxy was invalid.");
			break;
		case ERROR_INTERNET_HANDLE_EXISTS:
            ::_tcscpy(strResult,"The request failed because the handle already exists.");
			break;
		case ERROR_FTP_TRANSFER_IN_PROGRESS:
            ::_tcscpy(strResult,"The requested operation cannot be made on the FTP session "
						" handle because an operation is already in progress.");
			break;
		case ERROR_FTP_DROPPED:
            ::_tcscpy(strResult,"The FTP operation was not completed because the session was aborted.");
			break;
	}
	return strResult;
}


