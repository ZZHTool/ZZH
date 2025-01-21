// SD.cpp: 实现文件
//
#include "pch.h"
#include "afxdialogex.h"
#include "SD.h"
#include "resource.h"
#include <afx.h>
#include <afxwin.h>
#include <minwindef.h>
#include <afxdd_.h>
#include <afxmsg_.h>
#include <cstdio>
#include <cstdlib>
#include <WinUser.h>
#include <atltypes.h>
#include <windef.h>
//#pragma warning(disable : 4996)

// SD 对话框

IMPLEMENT_DYNAMIC(SD, CDialogEx)

SD::SD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHUTDOWN, pParent)
{
	time = 60;
}

void SD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_EDIT1, m_time);
}


BEGIN_MESSAGE_MAP(SD, CDialogEx)
	ON_BN_CLICKED(IDOK2, &SD::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &SD::OnBnClickedOk3)
	ON_EN_CHANGE(IDC_EDIT1, &SD::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &SD::OnBnClickedButton1)
END_MESSAGE_MAP()


// SD 消息处理程序
BOOL SD::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_combo.AddString(L"定时关机");
	m_combo.AddString(L"定时重启");
	m_combo.SetCurSel(0);
	Time.Format(L"%d", time);
	m_time.SetWindowTextW(Time);
	return TRUE;
}

void SD::OnPaint()
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
HCURSOR SD::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void SD::OnBnClickedOk3()
{
	EndDialog(0);
}


void SD::OnEnChangeEdit1()
{
	time = GetDlgItemInt(IDC_EDIT1);
}

void SD::OnBnClickedOk2()
{
	int i = m_combo.GetCurSel();
	if (i == 0)
	{
		char cmd[100];
		sprintf_s(cmd, "shutdown -s -t %d", time);
		system(cmd);
		MessageBoxA(0, "已成功设置定时关机！", "成功！", MB_OK);
	}
	else
	{
		char cmd[100];
		sprintf_s(cmd, "shutdown -r -t %d", time);
		system(cmd);
		MessageBoxA(0, "已成功设置定时重启！", "成功！", MB_OK);
	}
}

void SD::OnBnClickedButton1()
{
	system("shutdown -a");
}
