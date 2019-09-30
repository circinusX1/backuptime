; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgSchedule
LastTemplate=CCmdTarget
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "backupiime.h"
LastPage=0

ClassCount=13
Class1=CBackUpiImeApp
Class2=CAboutDlg
Class3=CBackUpiImeDlg
Class4=TabDlgSel

ResourceCount=8
Resource1=DLG_PICKFOLDERS
Resource2=DLG_SCHEDULE
Class5=CDlgPickFolders
Class6=CGListCtrl
Resource3=IDD_ABOUTBOX
Class7=CDlgFolderSelect
Class8=CFolderTree
Resource4=DLG_BACKUPDETINATION
Class9=CDlgBackupDest
Resource5=IDD_DIALOG1
Class10=CFtpSettings
Resource6=IDD_BACKUPIIME_DIALOG
Class11=CDlgSchedule
Resource7=DLG_SELECTFOLDER
Class12=CFrm
Class13=CTrayIcon
Resource8=IDI_TRAYICON

[CLS:CBackUpiImeApp]
Type=0
BaseClass=CWinApp
HeaderFile=BackUpiIme.h
ImplementationFile=BackUpiIme.cpp
Filter=N
VirtualFilter=AC
LastObject=CBackUpiImeApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=BackUpiImeDlg.cpp
ImplementationFile=BackUpiImeDlg.cpp

[CLS:CBackUpiImeDlg]
Type=0
BaseClass=CDialog
HeaderFile=BackUpiImeDlg.h
ImplementationFile=BackUpiImeDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDCANCEL

[CLS:TabDlgSel]
Type=0
BaseClass=CTabCtrl
HeaderFile=tabdlgsel.h
ImplementationFile=tabdlgsel.cpp
Filter=W
VirtualFilter=UWC
LastObject=TabDlgSel

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_BACKUPIIME_DIALOG]
Type=1
Class=CBackUpiImeDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=PB_APPLY,button,1342242816
Control3=TAB_CTRL,SysTabControl32,1342177280
Control4=ST_DLG,static,1073741831

[DLG:DLG_PICKFOLDERS]
Type=1
Class=CDlgPickFolders
ControlCount=5
Control1=IDC_STATIC,button,1342177287
Control2=IDC_LIST1,SysListView32,1350631437
Control3=PB_ADDFOLDER,button,1342242816
Control4=PB_DELETE_FOLDER,button,1476460544
Control5=PB_EDITFOLDER,button,1342242816

[CLS:CDlgPickFolders]
Type=0
HeaderFile=DlgPickFolders.h
ImplementationFile=DlgPickFolders.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgPickFolders

[CLS:CGListCtrl]
Type=0
HeaderFile=GListCtrl.h
ImplementationFile=GListCtrl.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CGListCtrl

[DLG:DLG_SELECTFOLDER]
Type=1
Class=CDlgFolderSelect
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TREE1,SysTreeView32,1342242823
Control4=IDC_STATIC_NOTE,static,1342308352
Control5=EF_FTYPES,edit,1342242944
Control6=IDC_STATIC,static,1342308352

[CLS:CDlgFolderSelect]
Type=0
HeaderFile=DlgFolderSelect.h
ImplementationFile=DlgFolderSelect.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgFolderSelect
VirtualFilter=dWC

[CLS:CFolderTree]
Type=0
HeaderFile=FolderTree.h
ImplementationFile=FolderTree.cpp
BaseClass=CTreeCtrl
Filter=W
VirtualFilter=GWC
LastObject=CFolderTree

[DLG:DLG_BACKUPDETINATION]
Type=1
Class=CDlgBackupDest
ControlCount=22
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342308359
Control3=IDC_STATIC,button,1342308359
Control4=IDC_RADIO1,button,1342308361
Control5=IDC_RADIO2,button,1342177289
Control6=EF_DETFOLDER,edit,1342244992
Control7=PB_NETEXPLORE,button,1476460544
Control8=PB_FTP_SETTINGS,button,1476460544
Control9=CK_REPORT,button,1073807363
Control10=CK_OVERWRITE,button,1342242819
Control11=ST_USER,static,1476526080
Control12=ST_PASSWORD,static,1476526080
Control13=ST_SERVER,static,1476526080
Control14=ST_PORT,static,1476526080
Control15=ST_LOCATION,static,1476526080
Control16=ST1,static,1476526080
Control17=ST2,static,1476526080
Control18=ST3,static,1476526080
Control19=ST4,static,1476526080
Control20=ST5,static,1476526080
Control21=CK_CREATETREE,button,1342242819
Control22=PB_FTP_SETTINGS2,button,1476460544

[CLS:CDlgBackupDest]
Type=0
HeaderFile=DlgBackupDest.h
ImplementationFile=DlgBackupDest.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgBackupDest

[DLG:IDD_DIALOG1]
Type=1
Class=CFtpSettings
ControlCount=21
Control1=IDOK,button,1476460545
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=EF_HOST,edit,1342242944
Control5=IDC_STATIC,static,1342308352
Control6=EF_PORT,edit,1342242944
Control7=IDC_STATIC,static,1342308352
Control8=EF_LOGIN,edit,1342242944
Control9=IDC_STATIC,static,1342308352
Control10=EF_PASSWORD,edit,1342242976
Control11=IDC_STATIC,static,1342308352
Control12=EF_INITIALPATH,edit,1342242944
Control13=IDC_TREE1,SysTreeView32,1342246951
Control14=PB_CONNECT,button,1342242816
Control15=EF_OUT,edit,1344342148
Control16=IDC_CHECK1,button,1342242819
Control17=ST_OUT,static,1342308352
Control18=LS_FILES,SysListView32,1342275589
Control19=CK_TIMECHECK,button,1342242819
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352

[CLS:CFtpSettings]
Type=0
HeaderFile=FtpSettings.h
ImplementationFile=FtpSettings.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CK_TIMECHECK

[DLG:DLG_SCHEDULE]
Type=1
Class=CDlgSchedule
ControlCount=15
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=CB_MONTH,combobox,1344339971
Control4=IDC_STATIC,static,1342308352
Control5=CB_DAY,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=CB_TIME,combobox,1344339971
Control8=CK_SHOWERRORMSGBOX,button,1073807363
Control9=CK_WORKINBACKGROUND,button,1073807363
Control10=CK_TASKBAR,button,1073807363
Control11=CK_LOGERRORS,button,1342242819
Control12=PB_BACKUPNOW,button,1342242816
Control13=CK_CHANGEFT,button,1342242819
Control14=IDC_EDIT1,edit,1344342084
Control15=CK_HOOKCHANGE,button,1342242819

[CLS:CDlgSchedule]
Type=0
HeaderFile=DlgSchedule.h
ImplementationFile=DlgSchedule.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CK_HOOKCHANGE

[CLS:CFrm]
Type=0
HeaderFile=Frm.h
ImplementationFile=Frm.cpp
BaseClass=CFrameWnd
Filter=T
VirtualFilter=fWC
LastObject=MN_STOP

[CLS:CTrayIcon]
Type=0
HeaderFile=TrayIcon.h
ImplementationFile=TrayIcon.cpp
BaseClass=CCmdTarget
Filter=N

[MNU:IDI_TRAYICON]
Type=1
Class=CFrm
Command1=MN_SETTINGS
Command2=MN_ENABLE
Command3=MN_DISABLE
Command4=MN_STOP
Command5=MN_EXIT
CommandCount=5

