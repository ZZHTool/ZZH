#include "afxdialogex.h"
#include "MFCApplication2Dlg.h"
#include "MyClearUpDlg.h"
#include "MyFileDlg.h"
#include "MyServiceDlg.h"
#include "MyStartupDlg.h"
#include "MyUninstallDlg.h"
#include "XtDlg.h"
#include "ZizhiDlg.h"
#include <afx.h>
#include <afxwin.h>
#include <basetsd.h>
#include <CProcess.h>
#include <minwindef.h>
#include <windef.h>
#include <WinUser.h>
#include <atltypes.h>
#include <afxstr.h>
#include <wingdi.h>
#include <afxbutton.h>
#define WM_AUTOCLEAN  WM_USER+101

// CHome 对话框

class CHome : public CDialogEx
{
	DECLARE_DYNAMIC(CHome)

public:
	CHome(CWnd* pParent = nullptr);// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOME };
#endif
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CRect rect = lpDrawItemStruct->rcItem;
		// 绘制按钮背景，使用不透明颜色
		pDC->FillSolidRect(rect, RGB(255, 255, 255));
		// 继续绘制按钮的其他元素，如文本等
		CString strText;
		GetWindowText(strText);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	void CreateAndShowDialog(CDialogEx& dlg, UINT nIDTemplate, int x, int y);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Clean();
	static UINT clean(void* pParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShow();
	afx_msg void OnHide();
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	LRESULT AutoCleanMemory(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	CMenu m_Menu;
	CWinThread* m_pCleanThread;

	CMFCApplication2Dlg homedlg;
	XtDlg xtdlg;
	ZizhiDlg zizhidlg;
	MyClearUpDlg cleandlg;
	MyFileDlg myfiledlg;
	MyUnistallDlg uninstalldlg;
	MyServiceDlg servicedlg;
	MyStartupDlg startupdlg;
	CProcess processdlg;

	CMFCButton m_home;
	CMFCButton m_xt;
	CMFCButton m_zizhi;
	CMFCButton m_clean;
	CMFCButton m_explorer;
	CMFCButton m_uninstall;
	CMFCButton m_service;
	CMFCButton m_startup;
	CMFCButton m_process;
	CMFCButton m_hide;
	CMFCButton m_restart;

	CStatic m_xiancheng;
	CStatic m_cpu;
	CStatic m_ram;
	CStatic m_battery;
	UINT_PTR m_nTimerID;
	UINT_PTR m_CleanTimer;
	afx_msg void OnBnClickedButton12();
};

