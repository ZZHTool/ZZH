#include "afxdialogex.h"
#include "CExplorerSetting.h"
#include "pch.h"
#include "resource.h"
#include <Psapi.h>
#include <TlHelp32.h>

// 检查指定进程是否加载了指定的DLL
bool IsDllLoadedInProcess(const wchar_t* dllName, DWORD processId) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (hProcess == NULL) {
		return false;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;

	// 枚举进程加载的所有模块
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			wchar_t szModName[MAX_PATH];
			// 获取模块的完整路径
			if (GetModuleFileNameExW(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(wchar_t))) {
				// 提取文件名
				wchar_t* fileName = wcsrchr(szModName, L'\\');
				if (fileName != NULL) {
					fileName++;
					// 检查文件名是否与目标DLL名称匹配
					if (wcscmp(fileName, dllName) == 0) {
						CloseHandle(hProcess);
						return true;
					}
				}
			}
		}
	}

	CloseHandle(hProcess);
	return false;
}

// 根据进程名获取进程ID
DWORD GetProcessIdByName(const wchar_t* processName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32W pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(hSnapshot, &pe32)) {
		do {
			if (wcscmp(pe32.szExeFile, processName) == 0) {
				CloseHandle(hSnapshot);
				return pe32.th32ProcessID;
			}
		} while (Process32NextW(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return 0;
}

// CExplorerSetting 对话框

IMPLEMENT_DYNAMIC(CExplorerSetting, CDialogEx)

CExplorerSetting::CExplorerSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_EXPLORER, pParent)
{

}

CExplorerSetting::~CExplorerSetting()
{
}

void CExplorerSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_COMBO2, m_combo1);
}


BEGIN_MESSAGE_MAP(CExplorerSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CExplorerSetting::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CExplorerSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CExplorerSetting 消息处理程序

BOOL CExplorerSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_combo.AddString(L"开启");
	m_combo.AddString(L"关闭");
	m_combo1.AddString(L"开启");
	m_combo1.AddString(L"关闭");
	HKEY hKey;
	CString lpRun = L"lnkfile";
	LONG lRet = RegOpenKeyExW(HKEY_CLASSES_ROOT, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		PVOID ab = nullptr;
		LPDWORD abc = nullptr;
		lRet = RegGetValueW(hKey,NULL, L"IsShortcut", RRF_RT_ANY , 0, ab, abc);
		if (lRet == ERROR_SUCCESS)
		{
			m_combo.SetCurSel(1);
		}
		else
		{
			m_combo.SetCurSel(0);
		}
	}
	else
	{
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
		EndDialog(0);
	}
	RegCloseKey(hKey);
	const wchar_t* processName = L"explorer.exe";
	const wchar_t* dllName = L"ExplorerBlurMica.dll";
	DWORD processId = GetProcessIdByName(processName);
	if (processId == 0) 
	{
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
		EndDialog(0);
	}
	// 检查目标DLL是否被加载
	if (IsDllLoadedInProcess(dllName, processId)) 
	{

	}
	else 
	{

	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExplorerSetting::OnBnClickedButton2()
{
	EndDialog(0);
}

void CExplorerSetting::OnBnClickedOk()
{
	int i = m_combo.GetCurSel();
	HKEY hKey;
	CString lpRun = L"lnkfile";
	PVOID ab = nullptr;
	LPDWORD abc = nullptr;
	if (i == 0)
	{
		long lRet = RegOpenKeyExW(HKEY_CLASSES_ROOT, lpRun, 0, KEY_ALL_ACCESS, &hKey);
		if (lRet == ERROR_SUCCESS)
		{
			lRet = RegGetValueW(hKey, NULL, L"IsShortcut", RRF_RT_ANY, 0, ab, abc);
			if (lRet == ERROR_SUCCESS)
			{
				lRet = RegDeleteValueW(hKey, L"IsShortcut");
				if (lRet == ERROR_SUCCESS)
				{
					ShellExecuteW(0, L"runas", L"cmd.exe", L"/c taskkill /f /im explorer.exe & start explorer.exe", 0, SW_HIDE);
					MessageBoxW(L"去除快捷方式小箭头成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
				}
			}
		}
		else
		{
			MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST);
		}
		RegCloseKey(hKey);
	}
	else
	{
		long lRet = RegOpenKeyExW(HKEY_CLASSES_ROOT, lpRun, 0, KEY_ALL_ACCESS, &hKey);
		if (lRet != ERROR_SUCCESS)
		{
			MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST);
		}
		else
		{
			lRet = RegGetValueW(hKey, NULL, L"IsShortcut", RRF_RT_ANY, 0, ab, abc);
			if (lRet != ERROR_SUCCESS)
			{
				lRet = RegSetValueExW(hKey, L"IsShortcut", 0, REG_DWORD, 0, 0);
				if (lRet != ERROR_SUCCESS)
				{
					MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST);
				}
				else
				{
					ShellExecuteW(0, L"runas", L"cmd.exe", L"/c taskkill /f /im explorer.exe & start explorer.exe", 0, SW_HIDE);
					MessageBoxW(L"开启快捷方式小箭头成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
				}
			}
		}
		RegCloseKey(hKey);
	}
	int j = m_combo1.GetCurSel();
	if (j == 0)
	{
		//system("uninstall.cmd");
		ShellExecuteW(0, L"runas", L"E:\\系统\\C++\\ZZH系统工具\\x64\\Release\\uninstall.cmd", 0, 0, SW_HIDE);
	}
	else
	{
		//system("register.cmd");
		ShellExecuteW(0, L"runas", L"E:\\系统\\C++\\ZZH系统工具\\x64\\Release\\register.cmd",0, 0, SW_HIDE);
	}
	EndDialog(0);
}
