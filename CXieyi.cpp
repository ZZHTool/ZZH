// CXieyi.cpp: 实现文件
//
#include "pch.h"
#include "afxdialogex.h"
#include "CXieyi.h"
#include "resource.h"
#include <afx.h>
#include <afxwin.h>
#include <atltypes.h>
#include <minwindef.h>
#include <windef.h>
#include <WinUser.h>
#include <afxmsg_.h>
#include "MFCApplication2.h"


// CXieyi 对话框

IMPLEMENT_DYNAMIC(CXieyi, CDialogEx)

CXieyi::CXieyi(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORMVIEW, pParent)
{

}

void CXieyi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXieyi, CDialogEx)
	ON_BN_CLICKED(IDC_EXIT, &CXieyi::OnBnClickedExit)
END_MESSAGE_MAP()


// CXieyi 消息处理程序
BOOL CXieyi::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CXieyi::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXieyi::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CXieyi::OnBnClickedExit()
{
	EndDialog(0);
}
