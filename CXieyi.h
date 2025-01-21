#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <windef.h>

// CXieyi 对话框

class CXieyi : public CDialogEx
{
	DECLARE_DYNAMIC(CXieyi)

public:
	CXieyi(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
};
