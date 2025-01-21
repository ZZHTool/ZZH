// MFCApplication2Dlg.cpp: 实现文件
#include "pch.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <afx.h>
#include <afxwin.h>
#include <afxmsg_.h>
#include <minwindef.h>
#include <windef.h>
#include <atltypes.h>
#include <afxstr.h>
#include <WinUser.h>
#include "ZizhiDlg.h"
#include "CXieyi.h"
#include "MFCApplication2.h"
#include "MyHead.h"
#include <afxdd_.h>
#include <basetsd.h>
#include <WinBase.h>
#include <winnt.h>
#include <sysinfoapi.h>
#include <minwinbase.h>
#include <CSettings.h>
#include <vector>
#include <memory>
#define INFO_BUFFER_SIZE 32767
TCHAR  infoBuf[INFO_BUFFER_SIZE] = { '\0' };
// CMFCApplication2Dlg 对话框

CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_name);
	DDX_Control(pDX, IDC_STATIC7, m_localtime);
	DDX_Control(pDX, IDC_STATIC8, m_systemtime);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 消息处理程序

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_nTimerID = SetTimer(1, 200, NULL);
	CString string;
	DWORD i = 0;
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf, &bufCharCount);
	string.Format(L"设备名称:%s", infoBuf);
	m_name.SetWindowTextW(string);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication2Dlg::OnPaint()
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
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nTimerID)
	{
		// 使用 unique_ptr 管理 CString 向量
		auto A = std::make_unique<std::vector<CString>>();
		// 使用 unique_ptr 管理 SYSTEMTIME 向量
		auto B = std::make_unique<std::vector<SYSTEMTIME>>();

		SYSTEMTIME systime{};
		CString nStr;
		CString string;
		SYSTEMTIME systemtime;

		// 将未赋值的 nStr 和 string 加入到向量 A 中
		A->push_back(nStr);
		A->push_back(string);
		// 将初始化为零的 systime 加入到向量 B 中
		B->push_back(systime);

		// 获取当前的本地时间，存储在 systime 中
		GetLocalTime(&systime);
		// 将本地时间的小时、分钟、秒格式化为字符串，存储在 nStr 中
		nStr.Format(L"当地时间:%d:%d:%d", systime.wHour, systime.wMinute, systime.wSecond);
		m_localtime.SetWindowTextW(nStr);

		// 获取当前的世界时间，存储在 systemtime 中
		GetSystemTime(&systemtime);
		// 将世界时间的小时、分钟、秒格式化为字符串，存储在 string 中
		string.Format(L"世界时间:%d:%d:%d", systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
		m_systemtime.SetWindowTextW(string);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMFCApplication2Dlg::OnBnClickedButton1()
{
	CSettings dlg;
	dlg.DoModal();
}


void CMFCApplication2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(m_nTimerID);
}
