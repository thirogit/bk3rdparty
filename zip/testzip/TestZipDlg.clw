; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHyperLink
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TestZipDlg.h"

ClassCount=9
Class1=CTestZipDlgApp
Class2=CTestZipDlgDlg

ResourceCount=8
Resource2=IDD_TESTZIPDLG_DIALOG
Resource3=IDD_TEST_REPORT
Class3=CListCtrlEnh
Resource1=IDR_MAINFRAME
Class4=COptionsDlg
Resource4=IDD_TESTZIPDLG_DIALOG (English (U.S.))
Resource5=IDR_DLGMENU
Class5=CAboutDlg
Resource6=IDD_OPTIONS
Class6=CCommentDlg
Class7=CResizableDialog
Resource7=IDD_DIALOG1
Class8=CDlgTestReport
Class9=CHyperLink
Resource8=IDD_FILECOMMENT

[CLS:CTestZipDlgApp]
Type=0
HeaderFile=TestZipDlg.h
ImplementationFile=TestZipDlg.cpp
Filter=N

[CLS:CTestZipDlgDlg]
Type=0
HeaderFile=TestZipDlgDlg.h
ImplementationFile=TestZipDlgDlg.cpp
Filter=W
BaseClass=CResizableDialog
VirtualFilter=dWC
LastObject=CTestZipDlgDlg



[DLG:IDD_TESTZIPDLG_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CTestZipDlgDlg

[DLG:IDD_TESTZIPDLG_DIALOG (English (U.S.))]
Type=1
Class=CTestZipDlgDlg
ControlCount=4
Control1=IDC_CONTENTS,SysListView32,1350631425
Control2=IDC_COMM_TXT,static,1342308352
Control3=IDC_EDIT1,edit,1486950596
Control4=IDC_PROGRESS1,msctls_progress32,1082130433

[DLG:IDD_OPTIONS]
Type=1
Class=COptionsDlg
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_RADIO1,button,1342308361
Control5=IDC_RADIO2,button,1342177289
Control6=IDC_RADIO3,button,1342177289
Control7=IDC_EDIT1,edit,1350639746
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CHECK1,button,1342242819
Control10=IDC_CHECK2,button,1342242819
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT2,edit,1350639744
Control13=IDC_SPIN1,msctls_updown32,1342177462
Control14=IDC_STATIC,button,1342177287
Control15=IDC_EDIT3,edit,1350631552
Control16=IDC_STATIC,static,1342308352

[CLS:CListCtrlEnh]
Type=0
HeaderFile=ListCtrlEnh.h
ImplementationFile=ListCtrlEnh.cpp
BaseClass=CListCtrl
LastObject=CListCtrlEnh

[CLS:COptionsDlg]
Type=0
HeaderFile=OptionsDlg.h
ImplementationFile=OptionsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

[MNU:IDR_DLGMENU]
Type=1
Class=CTestZipDlgDlg
Command1=ID_ARCHIVE_NEW
Command2=ID_ARCHIVE_OPEN
Command3=ID_ARCHIVE_CLOSE
Command4=ID_ARCHIVE_OPTIONS
Command5=ID_ARCHIVE_EXIT
Command6=ID_ACTION_ADD_FILES
Command7=ID_ACTION_ADD_FOLODER
Command8=ID_ACTION_DELETE
Command9=ID_ACTION_EXTRACT
Command10=ID_ACTION_TEST
Command11=ID_HELP_ABOUT
CommandCount=11

[DLG:IDD_DIALOG1]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_WEB,static,1342308352

[CLS:CAboutDlg]
Type=0
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_FILE_NEW
VirtualFilter=dWC

[DLG:IDD_FILECOMMENT]
Type=1
Class=CCommentDlg
ControlCount=3
Control1=IDC_EDIT1,edit,1352732868
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[CLS:CCommentDlg]
Type=0
HeaderFile=CommentDlg.h
ImplementationFile=CommentDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CResizableDialog]
Type=0
HeaderFile=ResizableDialog.h
ImplementationFile=ResizableDialog.cpp
BaseClass=CDialog

[DLG:IDD_TEST_REPORT]
Type=1
Class=CDlgTestReport
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1353779396

[CLS:CDlgTestReport]
Type=0
HeaderFile=DlgTestReport.h
ImplementationFile=DlgTestReport.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgTestReport

[CLS:CHyperLink]
Type=0
HeaderFile=HyperLink.h
ImplementationFile=HyperLink.cpp
BaseClass=CStatic

