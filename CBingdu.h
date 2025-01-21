#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>


// CBingdu 对话框

class CBingdu : public CDialogEx
{
	DECLARE_DYNAMIC(CBingdu)

public:
	CBingdu(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINGDU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo;
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk2();
};
