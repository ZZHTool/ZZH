#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <windef.h>
#include <afxbutton.h>


// XtDlg 对话框

class XtDlg : public CDialogEx
{
	DECLARE_DYNAMIC(XtDlg)

public:
	XtDlg(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBotton();
	afx_msg void OnBnClickedBotton2();
	afx_msg void OnBnClickedBotton3();
	afx_msg void OnBnClickedBotton4();
	afx_msg void OnBnClickedBotton5();
	afx_msg void OnBnClickedBotton6();
	afx_msg void OnBnClickedBotton7();
	afx_msg void OnBnClickedBotton8();
	afx_msg void OnBnClickedBotton9();
	afx_msg void OnBnClickedBotton10();
	afx_msg void OnBnClickedBotton11();
	afx_msg void OnBnClickedBotton12();
	afx_msg void OnBnClickedBotton13();
	afx_msg void OnBnClickedBotton14();
	afx_msg void OnBnClickedBotton15();
	afx_msg void OnBnClickedBotton16();
	afx_msg void OnBnClickedBotton17();
	afx_msg void OnBnClickedBotton18();
	afx_msg void OnBnClickedBotton19();
};
