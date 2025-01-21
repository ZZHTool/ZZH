#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <afxstr.h>
#include <basetsd.h>
#include <WinUser.h>


// Xuan 对话框

class Xuan : public CDialogEx
{
	DECLARE_DYNAMIC(Xuan)

public:
	Xuan(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XUAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int Edit1;
	int Edit2;
	CString number;
	CString edit1;
	CString edit2;
	CEdit m_edit1;
	CEdit m_edit2;
	CButton m_start;
	CButton m_end;
	CStatic m_static;
	UINT_PTR m_timer;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton11();
	BOOL PreTranslateMessage(MSG* pMsg);
};
