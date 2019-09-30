//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include <direct.h>
#include "BackUpiIme.h"
#include "BackUpiImeDlg.h"

BOOL IsAdmin(void);
#define ACCESS_READ  1
#define ACCESS_WRITE 2

void DisplayError(WCHAR* szAPI) {

   LPTSTR szErrorMsg;
   DWORD  dwErrorCode = GetLastError();

   FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
         | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorCode,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &szErrorMsg,
         0, NULL);
   AfxMessageBox(MKSTR("%s failed with error %d: %s", szAPI, dwErrorCode, szErrorMsg));
   LocalFree(szErrorMsg);
}

void CBackUpiImeApp::ResolveSecurity()
{
    SECURITY_ATTRIBUTES  sa;
    SECURITY_DESCRIPTOR  sd;
    sa.nLength          = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle   = TRUE;
    sa.lpSecurityDescriptor = &sd;


    if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) 
    {
        AfxMessageBox("unable to InitializeSecurityDescriptor, err = %d",GetLastError());
        return ;
    }   

    if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)0, FALSE)) 
    {
        AfxMessageBox("unable to SetSecurityDescriptorDacl, err = %d",GetLastError());
        return ;
    }
    _adminMode = IsAdmin();

/*    
    HANDLE token; 
    BOOL b = LogonUser("marius",
                       "GE113619",
                       "elamar",
                       LOGON32_LOGON_INTERACTIVE,
                       LOGON32_PROVIDER_DEFAULT,&token);
    DWORD dw= GetLastError(); 
    if(b)
    {
        ImpersonateLoggedOnUser(token);
        dw= GetLastError(); 
    }


    FILE* pf = fopen("C:\\Documents and Settings\\Administrator\\Local Settings\\Application Data\\Microsoft\\Windows\\UsrClass.dat.LOG","rb");
    if(pf)  
    {
        char c[128];
        fgets(c,128,pf);
        AfxMessageBox(MKSTR("File Opened. Admin Mode = %d %s",_adminMode, c));
        fclose(pf);
    }
    else
    {
        AfxMessageBox(MKSTR("cant open file: %d Admin Mode=%d"),GetLastError(),_adminMode);
    }

*/
} 

BOOL IsAdmin(void) {

   HANDLE hToken;
   DWORD  dwStatus;
   DWORD  dwAccessMask;
   DWORD  dwAccessDesired;
   DWORD  dwACLSize;
   DWORD  dwStructureSize = sizeof(PRIVILEGE_SET);
   PACL   pACL            = NULL;
   PSID   psidAdmin       = NULL;
   BOOL   bReturn         = FALSE;

   PRIVILEGE_SET   ps;
   GENERIC_MAPPING GenericMapping;

   PSECURITY_DESCRIPTOR     psdAdmin           = NULL;
   SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;
   
   __try {

      // AccessCheck() requires an impersonation token.
      ImpersonateSelf(SecurityImpersonation);

      if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, 
            &hToken)) {

         if (GetLastError() != ERROR_NO_TOKEN)
            __leave;

         // If the thread does not have an access token, we'll 
         // examine the access token associated with the process.
         if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, 
               &hToken))
            __leave;
      }

      if (!AllocateAndInitializeSid(&SystemSidAuthority, 2, 
            SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0, &psidAdmin))
         __leave;

      psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
      if (psdAdmin == NULL)
         __leave;

      if (!InitializeSecurityDescriptor(psdAdmin,
            SECURITY_DESCRIPTOR_REVISION))
         __leave;
  
      // Compute size needed for the ACL.
      dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
            GetLengthSid(psidAdmin) - sizeof(DWORD);

      // Allocate memory for ACL.
      pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
      if (pACL == NULL)
         __leave;

      // Initialize the new ACL.
      if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
         __leave;

      dwAccessMask= ACCESS_READ | ACCESS_WRITE;
      
      // Add the access-allowed ACE to the DACL.
      if (!AddAccessAllowedAce(pACL, ACL_REVISION2,
            dwAccessMask, psidAdmin))
         __leave;

      // Set our DACL to the SD.
      if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
         __leave;

      // AccessCheck is sensitive about what is in the SD; set
      // the group and owner.
      SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
      SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);

      if (!IsValidSecurityDescriptor(psdAdmin))
         __leave;

      dwAccessDesired = ACCESS_READ;

      // 
      // Initialize GenericMapping structure even though we
      // won't be using generic rights.
      // 
      GenericMapping.GenericRead    = ACCESS_READ;
      GenericMapping.GenericWrite   = ACCESS_WRITE;
      GenericMapping.GenericExecute = 0;
      GenericMapping.GenericAll     = ACCESS_READ | ACCESS_WRITE;

      if (!AccessCheck(psdAdmin, hToken, dwAccessDesired, 
            &GenericMapping, &ps, &dwStructureSize, &dwStatus, 
            &bReturn)) {
         printf("AccessCheck() failed with error %lu\n", GetLastError());
         __leave;
      }

      RevertToSelf();
   
   } __finally {

      // Cleanup 
      if (pACL) LocalFree(pACL);
      if (psdAdmin) LocalFree(psdAdmin);  
      if (psidAdmin) FreeSid(psidAdmin);
   }

   return bReturn;
}


