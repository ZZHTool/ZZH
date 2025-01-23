#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <afxbutton.h>


// CExplorerSetting 对话框

class CExplorerSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CExplorerSetting)

public:
	CExplorerSetting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExplorerSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_EXPLORER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	bool touming;
	CComboBox m_combo;
	CComboBox m_combo1;
	CComboBox m_combo2;
	CMFCButton m_button;
	CMFCButton m_button1;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};
