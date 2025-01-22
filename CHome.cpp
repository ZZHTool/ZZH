// CHome.cpp: 实现文件
#define WM_ICON_NOTIFY  WM_USER+100
#define ID_MENU_CLOSE 2001
#define ID_MENU_SHOW 2002
#define ID_MENU_HIDE 2003
#define ID_MENU_CLEAN 2004
#include "afxdialogex.h"
#include "CHome.h"
#include "CXieyi.h"
#include "pch.h"
#include "resource.h"
#include <afx.h>
#include <afxdd_.h>
#include <afxmsg_.h>
#include <afxstr.h>
#include <afxwin.h>
#include <atltypes.h>
#include <basetsd.h>
#include <minwindef.h>
#include <shellapi.h>
#include <synchapi.h>
#include <sysinfoapi.h>
#include <vector>
#include <WinBase.h>
#include <windef.h>
#include <winerror.h>
#include <winnt.h>
#include <winreg.h>
#include <WinUser.h>
#include <WTypesbase.h>
#include <tchar.h>
#include <libloaderapi.h>
#include <processenv.h>
#include <memory>
#include <wingdi.h>
void RestartApplication()
{
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	// 获取当前程序的路径
	CString strPath(szPath);
	// 获取当前程序的命令行参数
	CString strCmdLine = ::GetCommandLine();
	// 查找第一个空格的位置，以分隔程序路径和参数
	int nPos = strCmdLine.Find(_T(' '));
	if (nPos == -1)
	{
		// 如果没有空格，说明没有额外的命令行参数
		strCmdLine.Empty();
	}
	else
	{
		// 提取命令行参数部分
		strCmdLine = strCmdLine.Mid(nPos + 1);
	}
	// 使用 ShellExecute 来启动新的程序实例
	HINSTANCE hInstance = ::ShellExecuteW(NULL, L"runas", strPath, strCmdLine, NULL, SW_SHOWNORMAL);
	if ((int)hInstance > 32)
	{
		// 关闭当前程序
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_DESTROY, 0, 0);
	}
}
// CHome 对话框

IMPLEMENT_DYNAMIC(CHome, CDialogEx)
CHome::CHome(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOME, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCleanThread = nullptr;
}
void CHome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_xiancheng);
	DDX_Control(pDX, IDC_STATIC2, m_cpu);
	DDX_Control(pDX, IDC_STATIC3, m_ram);
	DDX_Control(pDX, IDC_STATIC4, m_battery);
	DDX_Control(pDX, IDC_BUTTON1, m_home);
	DDX_Control(pDX, IDC_BUTTON2, m_xt);
	DDX_Control(pDX, IDC_BUTTON3, m_zizhi);
	DDX_Control(pDX, IDC_BUTTON4, m_clean);
	DDX_Control(pDX, IDC_BUTTON5, m_explorer);
	DDX_Control(pDX, IDC_BUTTON6, m_uninstall);
	DDX_Control(pDX, IDC_BUTTON7, m_service);
	DDX_Control(pDX, IDC_BUTTON8, m_startup);
	DDX_Control(pDX, IDC_BUTTON11, m_process);
	DDX_Control(pDX, IDC_BUTTON10, m_hide);
	DDX_Control(pDX, IDC_BUTTON12, m_restart);
}


BEGIN_MESSAGE_MAP(CHome, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ICON_NOTIFY, &CHome::OnTrayNotification)
	ON_MESSAGE(WM_AUTOCLEAN, &CHome::AutoCleanMemory)
	ON_BN_CLICKED(ID_MENU_CLOSE, &CHome::OnClose)
	ON_BN_CLICKED(ID_MENU_SHOW, &CHome::OnShow)
	ON_BN_CLICKED(ID_MENU_HIDE, &CHome::OnHide)
	ON_BN_CLICKED(ID_MENU_CLEAN, &CHome::Clean)
	ON_BN_CLICKED(IDC_BUTTON1, &CHome::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CHome::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CHome::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CHome::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CHome::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CHome::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CHome::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CHome::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON10, &CHome::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CHome::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CHome::OnBnClickedButton12)
END_MESSAGE_MAP()

int GetregistryIntValue(const wchar_t* keyPath, const wchar_t* valueName)
{
	HKEY hKey;
	LONG result;
	DWORD valueSize = sizeof(DWORD);
	DWORD valueRead = 0;
	int intValue = 0;

	result = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS)
	{
		result = RegQueryValueExW(hKey, valueName, 0, NULL, reinterpret_cast<BYTE*>(&valueRead), &valueSize);
		if (result == ERROR_SUCCESS)
		{
			intValue = static_cast<int>(valueRead);
		}
		RegCloseKey(hKey);
	}
	else
	{
		MessageBoxW(0, L"开机自启相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
	}
	return intValue;
}
// CHome 消息处理程序
void CHome::CreateAndShowDialog(CDialogEx& dlg, UINT nIDTemplate, int x, int y)
{
	dlg.Create(nIDTemplate, this);
	dlg.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	dlg.ShowWindow(SW_HIDE);
}
BOOL CHome::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyle(0, WS_MINIMIZEBOX);
	SetTimer(m_nTimerID, 500, 0);
	SetIcon(m_hIcon, TRUE);// 设置大图标
	SetIcon(m_hIcon, FALSE);// 设置小图标
	NOTIFYICONDATA NotifyIcon{};
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon = m_hIcon;  //上面那句也可以
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, L"ZZH系统工具");
	NotifyIcon.uCallbackMessage = WM_ICON_NOTIFY;//自定义消息
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //添加系统托盘 

	CreateAndShowDialog(homedlg, IDD_MFCAPPLICATION2_DIALOG, 60, 70);
	homedlg.ShowWindow(SW_SHOW);
	CreateAndShowDialog(xtdlg, IDD_XT, 40, 55);
	CreateAndShowDialog(zizhidlg, IDD_ZIZHI, 80, 70);
	CreateAndShowDialog(cleandlg, IDD_CLEARUP_DIALOG, 50, 80);
	CreateAndShowDialog(myfiledlg, IDD_FILEINFO_DIALOG, 5, 70);
	CreateAndShowDialog(uninstalldlg, IDD_UNINSTALL_DIALOG, 5, 70);
	CreateAndShowDialog(servicedlg, IDD_SERVICE_DIALOG, 5, 70);
	CreateAndShowDialog(startupdlg, IDD_STARTUP_DIALOG, 5, 70);
	CreateAndShowDialog(processdlg, IDD_TASK_DIALOG, 5, 70);

	m_home.SetMouseCursorHand();
	m_xt.SetMouseCursorHand();
	m_zizhi.SetMouseCursorHand();
	m_clean.SetMouseCursorHand();
	m_explorer.SetMouseCursorHand();
	m_uninstall.SetMouseCursorHand();
	m_service.SetMouseCursorHand();
	m_startup.SetMouseCursorHand();
	m_process.SetMouseCursorHand();
	m_hide.SetMouseCursorHand();
	m_restart.SetMouseCursorHand();

	int set = 90;
	HKEY hKey;
	HKEY hKey1;
	CString lpRun = L"SOFTWARE\\ZZH System Tool";
	CString lpRun1 = L"SOFTWARE\\ZZH System Tool\\NoAutoClean";
	CString lpRun2 = L"SOFTWARE\\ZZH System Tool\\Translucent";
	CString lpRun3 = L"SOFTWARE\\ZZH System Tool\\Translucent\\NoTranslucent";
	long lRet = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		long result = RegCreateKeyExW(HKEY_CURRENT_USER, lpRun, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
		long result1 = RegCreateKeyExW(HKEY_CURRENT_USER, lpRun2, 0, NULL, 0, KEY_WRITE, NULL, &hKey1, NULL);
		long result2 = RegCreateKeyExW(HKEY_CURRENT_USER, lpRun2, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
		RegOpenKeyExW(HKEY_CURRENT_USER, lpRun2, 0, KEY_ALL_ACCESS, &hKey1);
		long result3 = RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
		if (result != ERROR_SUCCESS || result1 != ERROR_SUCCESS || result2 != ERROR_SUCCESS || result3 != ERROR_SUCCESS)
		{
			MessageBoxW(L"软件相关服务出现错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
			system("taskkill -f -im ZZH系统工具.exe");
		}
	}
	lRet = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun2, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		long result = RegCreateKeyExW(HKEY_CURRENT_USER, lpRun2, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
		long result1 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun2, 0, KEY_ALL_ACCESS, &hKey1);
		long result2 = RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
		if (result != ERROR_SUCCESS || result1 != ERROR_SUCCESS || result2 != ERROR_SUCCESS)
		{
			MessageBoxW(L"软件相关服务出现错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
			system("taskkill -f -im ZZH系统工具.exe");
		}
	}
	lRet = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun3, 0, KEY_ALL_ACCESS, &hKey1);
	if (lRet != ERROR_SUCCESS)
	{
		int a = GetregistryIntValue(L"SOFTWARE\\ZZH System Tool\\Translucent", L"setting");
		int b = a * 0.01 * 255;
		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(0, b, LWA_ALPHA);
	}

	CXieyi xieyi;
	xieyi.DoModal();

	lRet = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun1, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		m_pCleanThread = AfxBeginThread(clean, this);
	}
	RegCloseKey(hKey);
	RegCloseKey(hKey1);
	return TRUE;
}

void CHome::OnPaint()
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
HCURSOR CHome::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CHome::clean(void* pParam)
{
	CHome* Cleaning = (CHome*)pParam;
	Cleaning->Clean();
	return 0;
}

void CHome::Clean()
{
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	auto m_szStatus = std::make_unique<CString>();
	m_szStatus->Format(L"已为您自动清理内存！本次清理 : %.2f MB,程序将在后台持续优化内存！", preUsedMem - afterUsedMem);
	MessageBox(m_szStatus->GetString(), L"内存清理完成！", MB_ICONINFORMATION);
	SetTimer(m_CleanTimer, 10000, 0);
	/*if (m_pCleanThread != nullptr)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_pCleanThread->m_hThread, 0))
		{
			delete m_pCleanThread;
			m_pCleanThread = nullptr;
		}
	}*/
}



void CHome::OnBnClickedButton1()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_SHOW);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}


void CHome::OnBnClickedButton2()
{
	m_xt.EnableWindow(FALSE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_SHOW);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}


void CHome::OnBnClickedButton3()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(FALSE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_SHOW);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}


void CHome::OnBnClickedButton4()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(FALSE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_SHOW);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}


void CHome::OnBnClickedButton5()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(FALSE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_SHOW);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}


void CHome::OnBnClickedButton6()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(FALSE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_SHOW);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}

void CHome::OnBnClickedButton7()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(FALSE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_SHOW);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_HIDE);
}

void CHome::OnBnClickedButton8()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(FALSE);
	m_process.EnableWindow(TRUE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_SHOW);
	processdlg.ShowWindow(SW_HIDE);
}

void CHome::OnBnClickedButton10()
{
	this->ShowWindow(SW_HIDE);
	KillTimer(m_nTimerID);
}


void CHome::OnBnClickedButton11()
{
	m_xt.EnableWindow(TRUE);
	m_zizhi.EnableWindow(TRUE);
	m_clean.EnableWindow(TRUE);
	m_explorer.EnableWindow(TRUE);
	m_uninstall.EnableWindow(TRUE);
	m_service.EnableWindow(TRUE);
	m_startup.EnableWindow(TRUE);
	m_process.EnableWindow(FALSE);
	homedlg.ShowWindow(SW_HIDE);
	xtdlg.ShowWindow(SW_HIDE);
	zizhidlg.ShowWindow(SW_HIDE);
	cleandlg.ShowWindow(SW_HIDE);
	myfiledlg.ShowWindow(SW_HIDE);
	servicedlg.ShowWindow(SW_HIDE);
	uninstalldlg.ShowWindow(SW_HIDE);
	startupdlg.ShowWindow(SW_HIDE);
	processdlg.ShowWindow(SW_SHOW);
}



void CHome::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nTimerID)
	{
		TASKLIST nTaskList = {};
		// 使用智能指针管理 CString 对象
		auto nStr = std::make_unique<CString>();
		auto nstr = std::make_unique<CString>();
		auto Nstr = std::make_unique<CString>();
		auto tring = std::make_unique<CString>();
		auto string = std::make_unique<CString>();
		// 使用智能指针管理 DWORD 对象
		auto nNum = std::make_unique<DWORD>(0);

		int a, b, c;
		std::vector<TASKLIST> A;
		std::vector<std::unique_ptr<DWORD>> B;
		std::vector<std::unique_ptr<CString>> C;
		std::vector<int> powernum;

		A.push_back(nTaskList);
		B.push_back(std::make_unique<DWORD>(*nNum));
		C.push_back(std::make_unique<CString>(*nStr));
		C.push_back(std::make_unique<CString>(*nstr));
		C.push_back(std::make_unique<CString>(*Nstr));
		C.push_back(std::make_unique<CString>(*tring));
		C.push_back(std::make_unique<CString>(*string));
		powernum.push_back(a);
		powernum.push_back(b);
		powernum.push_back(c);

		*nNum = EnumTaskList(nTaskList); // 枚举进程列表
		nStr->Format(L"进程数:%d", *nNum);
		m_xiancheng.SetWindowTextW(*nStr);

		GetCpuState(*Nstr);
		Nstr->Format(L"CPU使用率:%s%% ", *Nstr);
		m_cpu.SetWindowTextW(*Nstr);

		*nNum = GetMemState();
		tring->Format(L"运行内存:%d%% ", *nNum);
		m_ram.SetWindowTextW(*tring);

		SYSTEM_POWER_STATUS powerStatus;
		GetSystemPowerStatus(&powerStatus);
		a = powerStatus.BatteryLifePercent;
		b = powerStatus.ACLineStatus;
		c = powerStatus.BatteryFlag;
		if (c == 128)
		{
			nstr->Format(L"电池：没有电池");
			m_battery.SetWindowTextW(*nstr);
		}
		else if (b == 1)
		{
			nstr->Format(L"正在充电:%d%%", a);
			m_battery.SetWindowTextW(*nstr);
		}
		else
		{
			nstr->Format(L"电池电量:%d%%", a);
			m_battery.SetWindowTextW(*nstr);
		}

		A.clear();
		B.clear();
		C.clear();
		powernum.clear();
	}
	else if (nIDEvent == m_CleanTimer)
	{
		ClearMemory();
		//SetSystemFileCacheSize(NULL, 0, FILE_CACHE_MIN_HARD_ENABLE);
	}
	CDialogEx::OnTimer(nIDEvent);
}



void CHome::OnClose()
{
	if (MessageBoxW(L"退出后将无法在后台优化内存，确定要退出程序吗？", L"退出提示", MB_ICONINFORMATION | MB_YESNO | MB_SYSTEMMODAL | MB_TOPMOST) == IDNO){return;}
	DestroyWindow();
	CDialogEx::OnClose();
}

void CHome::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(m_nTimerID);
	KillTimer(m_CleanTimer);
	NOTIFYICONDATA nd;
	nd.cbSize = sizeof(NOTIFYICONDATA);
	nd.hWnd = m_hWnd;
	nd.uID = IDR_MAINFRAME;
	nd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nd.uCallbackMessage = WM_ICON_NOTIFY;
	nd.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_DELETE, &nd);
	if (m_pCleanThread != nullptr)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_pCleanThread->m_hThread, 0))
		{
			delete m_pCleanThread;
			m_pCleanThread = nullptr;
		}
	}
	PostQuitMessage(0);
	//ShellExecute(0, L"runas", L"cmd.exe", L"/c taskkill /f /im ZZH系统工具.exe", 0, SW_HIDE);
}

void CHome::OnShow()
{
	SetTimer(m_nTimerID, 500, 0);
	this->ShowWindow(SW_SHOW);
}

void CHome::OnHide()
{
	this->ShowWindow(SW_HIDE);
	KillTimer(m_nTimerID);
}

LRESULT CHome::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此处添加实现代码.
	switch (lParam)
	{
	case WM_RBUTTONDOWN://鼠标右键点击图标
	{
		POINT point;
		GetCursorPos(&point);//检索鼠标光标的位置（以屏幕坐标为单位）
		CMenu menu;
		menu.CreatePopupMenu();//创建弹出式菜单
		menu.AppendMenuW(MF_STRING, ID_MENU_SHOW, L"显示");
		menu.AppendMenuW(MF_STRING, ID_MENU_HIDE, L"隐藏");
		menu.AppendMenuW(MF_STRING, ID_MENU_CLEAN, L"内存清理");
		menu.AppendMenuW(MF_STRING, ID_MENU_CLOSE, L"关闭程序");
		menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	case WM_LBUTTONDBLCLK:
	{
		OnShow();
	}
	case WM_LBUTTONUP:
	{
		OnShow();
	}
	break;
	}
	return LRESULT();
}

LRESULT CHome::AutoCleanMemory(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		if (m_pCleanThread != nullptr)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(m_pCleanThread->m_hThread, 0))
			{
				delete m_pCleanThread;
				m_pCleanThread = nullptr;
			}
		}
		m_pCleanThread = AfxBeginThread(clean, this);
	}
	else
	{
		if (m_pCleanThread != nullptr)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(m_pCleanThread->m_hThread, 0))
			{
				delete m_pCleanThread;
				m_pCleanThread = nullptr;
			}
		}
	}
	return LRESULT();
}



BOOL CHome::PreTranslateMessage(MSG* pMsg)
{
	//ESC键按下
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		OnClose();
		return TRUE;
	}

	//屏蔽Enter键按下
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && pMsg->wParam)
	{
		return TRUE;
	}

	//操作其他msg
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CHome::OnBnClickedButton12()
{
	RestartApplication();
}
