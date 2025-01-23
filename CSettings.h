#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <windef.h>
#include <afxbutton.h>


// CSettings 对话框

class CSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit1;
	CComboBox m_combo;
	CComboBox m_combo1;
	CComboBox m_combo2;
	CStatic m_ziqi;
	CStatic m_clean;
	CMFCButton m_button;
	CMFCButton m_button1;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo3();
};
