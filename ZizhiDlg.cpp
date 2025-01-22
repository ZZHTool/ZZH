#define WM_ICON_NOTIFY  WM_USER+100
#include "afxdialogex.h"
#include "CBingdu.h"
#include "MFCApplication2.h"
#include "pch.h"
#include "resource.h"
#include "SD.h"
#include "Xuan.h"
#include "ZizhiDlg.h"
#include <afx.h>
#include <afxmsg_.h>
#include <afxstr.h>
#include <afxwin.h>
#include <atltypes.h>
#include <libloaderapi.h>
#include <minwindef.h>
#include <shellapi.h>
#include <windef.h>
#include <winerror.h>
#include <winnt.h>
#include <winreg.h>
#include <WinUser.h>
#include <WinBase.h>
#include <CExplorerSetting.h>



// ZizhiDlg 对话框

IMPLEMENT_DYNAMIC(ZizhiDlg, CDialogEx)

ZizhiDlg::ZizhiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZIZHI, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ZizhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ZizhiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BOTTON, &ZizhiDlg::OnBnClickedBotton)
	ON_BN_CLICKED(IDC_BOTTON2, &ZizhiDlg::OnBnClickedBotton2)
	ON_BN_CLICKED(IDC_BOTTON3, &ZizhiDlg::OnBnClickedBotton3)
	ON_BN_CLICKED(IDC_BOTTON4, &ZizhiDlg::OnBnClickedBotton4)
	ON_BN_CLICKED(IDC_BOTTON5, &ZizhiDlg::OnBnClickedBotton5)
END_MESSAGE_MAP()


// ZizhiDlg 消息处理程序
BOOL ZizhiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void ZizhiDlg::OnPaint()
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
HCURSOR ZizhiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ZizhiDlg::OnBnClickedBotton()
{
	Xuan*dlg = new Xuan;
	dlg->Create(IDD_XUAN, this);
	dlg->ShowWindow(SW_SHOW);
}


void ZizhiDlg::OnBnClickedBotton2()
{
	CBingdu* dlg = new CBingdu;
	dlg->Create(IDD_BINGDU, this);
	dlg->ShowWindow(SW_SHOW);
}


void ZizhiDlg::OnBnClickedBotton3()
{
	SD* dlg = new SD;
	dlg->Create(IDD_SHUTDOWN, this);
	dlg->ShowWindow(SW_SHOW);
}


void ZizhiDlg::OnBnClickedBotton4()
{
	ShellExecuteW(0, L"runas", L"cmd.exe", L"/c taskkill /f /im explorer.exe & start explorer.exe", 0, SW_HIDE);
	MessageBoxW(L"重启Windows文件资源管理器成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
	NOTIFYICONDATA NotifyIcon{};
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon = m_hIcon;  //上面那句也可以
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, L"ZZH系统工具");
	NotifyIcon.uCallbackMessage = WM_ICON_NOTIFY;//自定义消息
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //添加系统托盘 
}


void ZizhiDlg::OnBnClickedBotton5()
{
	/*int i = MessageBoxW(L"点击是去除快捷方式小箭头，点击否恢复快捷方式小箭头", L"去除快捷方式小箭头", MB_ICONINFORMATION | MB_TOPMOST | MB_YESNOCANCEL);
	if (i == IDYES)
	{
		HKEY hKey;
		CString lpRun = L"lnkfile";
		LONG lRet = RegOpenKeyExW(HKEY_CLASSES_ROOT, lpRun, 0, KEY_ALL_ACCESS, &hKey);
		if (lRet == ERROR_SUCCESS) 
		{
			lRet = RegDeleteValueW(hKey, L"IsShortcut");
			if (lRet == ERROR_SUCCESS)
			{
				ShellExecuteW(0, L"runas", L"cmd.exe", L"/c taskkill /f /im explorer.exe & start explorer.exe", 0, SW_HIDE);
				MessageBoxW(L"去除快捷方式小箭头成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
			}
		}
		else
		{
			MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST);
		}
		RegCloseKey(hKey);
	}
	else if (i == IDNO)
	{
		HKEY hKey;
		CString lpRun = L"lnkfile";
		long lRet = RegOpenKeyExW(HKEY_CLASSES_ROOT, lpRun, 0, KEY_ALL_ACCESS, &hKey);
		if (lRet != ERROR_SUCCESS)
		{
			MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST);
		}
		else
		{
			lRet = RegSetValueExW(hKey, L"IsShortcut", 0, REG_DWORD, 0, 0);
			if (lRet != ERROR_SUCCESS)
			{
				MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST);
			}
			else
			{
				ShellExecute(0, L"runas", L"cmd.exe", L"/c taskkill /f /im explorer.exe & start explorer.exe", 0, SW_HIDE);
				MessageBoxW(L"恢复快捷方式小箭头成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
			}
		}
		RegCloseKey(hKey);
	}*/
	CExplorerSetting* dlg = new CExplorerSetting;
	dlg->Create(IDD_SETTINGS_EXPLORER, this);
	dlg->ShowWindow(SW_SHOW);
}
