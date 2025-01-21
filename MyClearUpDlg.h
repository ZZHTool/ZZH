#pragma once
#include "ClistCtrlEx.h"
#include "afxwin.h"
#include <afx.h>
#include <afxdialogex.h>
#include <afxstr.h>
#include <basetsd.h>
#include <minwindef.h>
#include <WTypesbase.h>
#include <winnt.h>


// MyClearUpDlg 对话框

class MyClearUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyClearUpDlg)

public:
	MyClearUpDlg(CWnd* pParent = NULL);   // 标准构造函数
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEARUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//BOOL EmptyDirectory(CString szPath, BOOL bDeleteDesktopIni, BOOL bWipeIndexDat);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	ClistCtrlEx m_List_File;
	CButton m_Button_Scan;
	CButton m_Button_Clear;

	BOOL IsScanFile;
	CString m_szTemp;
	DOUBLE m_AllFileSize;
	CStatic m_Static_Path;
	CString m_ScaningPath;

	DWORD TIMER_SCANPATH = 0x8888;
	DWORD TIMER_CLEARATH = 0x999;

	CString GetShowSize(DOUBLE dwSize);
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE, BOOL bWipeIndexDat = FALSE);
	BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);
	BOOL DeleteUrlCache();
	VOID CleanSystemTemp();
	VOID CleanUpdate();
	VOID CleanRubbishStation();
	VOID CleanFileFindHistory();
	VOID CleanBrowseAddrHistory();
	VOID CleanPasswordHistory();
	VOID CleanAddrBarHistory();
	VOID CleanInternetCookieTemp();

	BOOL m_systemtemp;
	BOOL m_rubbish;
	BOOL m_FindHistory;
	BOOL m_AddrHistory;
	BOOL m_PassHistory;

	CButton m_Check_SystemTemp;
	CButton m_Check_Rubbish;
	CButton m_Check_FindHistory;
	CButton m_Check_InterCookies;
	CButton m_Check_AddrHistory;
	CButton m_Check_PassHistory;
	CButton m_Check_BrowseHistroy;
	CButton m_update;
};
