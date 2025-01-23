// XtDlg.cpp: 实现文件
//
#include "afxdialogex.h"
#include "pch.h"
#include "resource.h"
#ifdef _DEBUG
#endif
#include <afx.h>
#include <afxwin.h>
#include "XtDlg.h"
#include <WinUser.h>
#include <minwindef.h>
#include <atltypes.h>
#include <windef.h>
#include "ZizhiDlg.h"
#include <afxmsg_.h>
#include <shellapi.h>
#include "MFCApplication2.h"
#include <MyHead.h>


// XtDlg 对话框

IMPLEMENT_DYNAMIC(XtDlg, CDialogEx)

XtDlg::XtDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XT, pParent)
{
}

void XtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XtDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BOTTON, &XtDlg::OnBnClickedBotton)
	ON_BN_CLICKED(IDC_BOTTON2, &XtDlg::OnBnClickedBotton2)
	ON_BN_CLICKED(IDC_BOTTON3, &XtDlg::OnBnClickedBotton3)
	ON_BN_CLICKED(IDC_BOTTON4, &XtDlg::OnBnClickedBotton4)
	ON_BN_CLICKED(IDC_BOTTON5, &XtDlg::OnBnClickedBotton5)
	ON_BN_CLICKED(IDC_BOTTON6, &XtDlg::OnBnClickedBotton6)
	ON_BN_CLICKED(IDC_BOTTON7, &XtDlg::OnBnClickedBotton7)
	ON_BN_CLICKED(IDC_BOTTON8, &XtDlg::OnBnClickedBotton8)
	ON_BN_CLICKED(IDC_BOTTON9, &XtDlg::OnBnClickedBotton9)
	ON_BN_CLICKED(IDC_BOTTON10, &XtDlg::OnBnClickedBotton10)
	ON_BN_CLICKED(IDC_BOTTON11, &XtDlg::OnBnClickedBotton11)
	ON_BN_CLICKED(IDC_BOTTON12, &XtDlg::OnBnClickedBotton12)
	ON_BN_CLICKED(IDC_BOTTON13, &XtDlg::OnBnClickedBotton13)
	ON_BN_CLICKED(IDC_BOTTON14, &XtDlg::OnBnClickedBotton14)
	ON_BN_CLICKED(IDC_BOTTON15, &XtDlg::OnBnClickedBotton15)
	ON_BN_CLICKED(IDC_BOTTON16, &XtDlg::OnBnClickedBotton16)
	ON_BN_CLICKED(IDC_BOTTON17, &XtDlg::OnBnClickedBotton17)
	ON_BN_CLICKED(IDC_BOTTON18, &XtDlg::OnBnClickedBotton18)
	ON_BN_CLICKED(IDC_BOTTON19, &XtDlg::OnBnClickedBotton19)
END_MESSAGE_MAP()

BOOL XtDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	return TRUE;
}

void XtDlg::OnPaint()
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
HCURSOR XtDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void XtDlg::OnBnClickedBotton()
{
	ShellExecute(0, L"runas", L"cmd.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton2()
{
	ShellExecute(0, L"runas", L"Taskmgr.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton3()
{
	ShellExecute(0, L"runas", L"regedit.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton4()
{
	ShellExecute(0, L"runas", L"gpedit.msc", 0, 0, SW_SHOW);
}

void XtDlg::OnBnClickedBotton5()
{
	ShellExecute(0, L"runas", L"winver.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton6()
{
	ShellExecute(0, L"runas", L"WF.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton7()
{
	ShellExecute(0, L"runas", L"UserAccountControlSettings.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton8()
{
	ShellExecute(0, L"runas", L"cleanmgr.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton9()
{
	ShellExecute(0, L"runas", L"taskschd.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton10()
{
	ShellExecute(0, L"runas", L"msinfo32.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton11()
{
	ShellExecute(0, L"runas", L"resmon.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton12()
{
	ShellExecute(0, L"runas",L"lusrmgr.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton13()
{
	ShellExecute(0, L"runas", L"ComputerDefaults.exe", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton14()
{
	ShellExecute(0, L"runas", L"comexp.msc", 0, 0, SW_SHOW);
}



void XtDlg::OnBnClickedBotton15()
{
	ShellExecute(0, L"runas", L"compmgmt.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton17()
{
	ShellExecute(0, L"runas", L"diskmgmt.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton16()
{
	ShellExecute(0, L"runas", L"eventvwr.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton18()
{
	ShellExecute(0, L"runas", L"services.msc", 0, 0, SW_SHOW);
}


void XtDlg::OnBnClickedBotton19()
{
	ShellExecute(0, L"runas", L"SystemPropertiesAdvanced.exe", 0, 0, SW_SHOW);
}

