#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <afxstr.h>
#include <windef.h>


// SD 对话框

class SD : public CDialogEx
{
	DECLARE_DYNAMIC(SD)

public:
	SD(CWnd* pParent = nullptr);   // 标准构造函数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHUTDOWN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo;
	int time;
	CString Time;
	CEdit m_time;
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
};
