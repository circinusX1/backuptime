# Microsoft Developer Studio Project File - Name="BackUpiIme" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BackUpiIme - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BackUpiIme.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BackUpiIme.mak" CFG="BackUpiIme - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BackUpiIme - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BackUpiIme - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BackUpiIme - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CFG_WIN32_WIN" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/BackUpWatch.exe"

!ELSEIF  "$(CFG)" == "BackUpiIme - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_CFG_WIN32_WIN" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BackUpiIme - Win32 Release"
# Name "BackUpiIme - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BackUpiIme.ico
# End Source File
# Begin Source File

SOURCE=.\res\BackUpiIme.rc2
# End Source File
# Begin Source File

SOURCE=.\res\backupin.ico
# End Source File
# Begin Source File

SOURCE=.\res\enabled1.ico
# End Source File
# Begin Source File

SOURCE=.\res\filetypes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\BackUpiIme.cpp
# End Source File
# Begin Source File

SOURCE=.\BackUpiIme.h
# End Source File
# Begin Source File

SOURCE=.\BackUpiIme.rc
# End Source File
# Begin Source File

SOURCE=.\BackUpiImeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BackUpiImeDlg.h
# End Source File
# Begin Source File

SOURCE=.\BackupProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\BackupProcess.h
# End Source File
# Begin Source File

SOURCE=.\DlgBackupDest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBackupDest.h
# End Source File
# Begin Source File

SOURCE=.\DlgFolderSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFolderSelect.h
# End Source File
# Begin Source File

SOURCE=.\DlgPickFolders.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPickFolders.h
# End Source File
# Begin Source File

SOURCE=.\DlgSchedule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSchedule.h
# End Source File
# Begin Source File

SOURCE=.\FolderTree.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderTree.h
# End Source File
# Begin Source File

SOURCE=.\Frm.cpp
# End Source File
# Begin Source File

SOURCE=.\Frm.h
# End Source File
# Begin Source File

SOURCE=.\Ftp_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Ftp_Client.h
# End Source File
# Begin Source File

SOURCE=.\FtpFolderTree.cpp
# End Source File
# Begin Source File

SOURCE=.\FtpFolderTree.h
# End Source File
# Begin Source File

SOURCE=.\FtpSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\FtpSettings.h
# End Source File
# Begin Source File

SOURCE=.\GListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tabdlgsel.cpp
# End Source File
# Begin Source File

SOURCE=.\tabdlgsel.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# End Target
# End Project
