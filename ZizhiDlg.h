#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <windef.h>


// ZizhiDlg 对话框

class ZizhiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ZizhiDlg)

public:
	ZizhiDlg(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZIZHI };
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
	afx_msg void OnBnClickedBotton();
	afx_msg void OnBnClickedBotton2();
	afx_msg void OnBnClickedBotton3();
	afx_msg void OnBnClickedBotton4();
	afx_msg void OnBnClickedBotton5();
};
