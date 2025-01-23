#include "afxdialogex.h"
#include "CExplorerSetting.h"
#include "pch.h"
#include "resource.h"
#include <Psapi.h>
#include <TlHelp32.h>
#include <fstream>

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
// 将WCHAR*转换为const char*
char* WCharToChar(const WCHAR* wstr) 
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	if (len == 0) 
	{
		return nullptr;
	}
	char* str = new char[len];
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	return str;
}
// 获取进程ID
DWORD GetProcessId(const wchar_t* processName) 
{
	PROCESSENTRY32W processEntry{};
	processEntry.dwSize = sizeof(PROCESSENTRY32W);
	// 创建进程快照
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE) 
	{
		return 0;
	}
	// 遍历进程
	if (Process32FirstW(snapshot, &processEntry)) 
	{
		do {
			if (wcscmp(processEntry.szExeFile, processName) == 0) 
			{  // 使用宽字符版本的字符串比较函数
				CloseHandle(snapshot);
				return processEntry.th32ProcessID;
			}
		} while (Process32NextW(snapshot, &processEntry));
	}
	CloseHandle(snapshot);
	return 0;
}
// 检查进程是否加载了指定的 DLL
bool IsDllLoaded(DWORD processId, const wchar_t* dllName) 
{
	MODULEENTRY32W moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32W);
	// 创建模块快照
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (hModuleSnap == INVALID_HANDLE_VALUE) 
	{
		return false;
	}
	// 遍历模块
	if (Module32FirstW(hModuleSnap, &moduleEntry)) 
	{
		do 
		{
			if (wcscmp(moduleEntry.szModule, dllName) == 0) 
			{
				CloseHandle(hModuleSnap);
				return true;
			}
		} while (Module32NextW(hModuleSnap, &moduleEntry));
	}
	CloseHandle(hModuleSnap);
	return false;
}

std::string getExecutableDirectory() {
	char buffer[MAX_PATH];
	// 获取当前可执行文件的完整路径
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string fullPath(buffer);
	// 找到最后一个反斜杠的位置
	size_t pos = fullPath.find_last_of("\\");
	if (pos != std::string::npos) {
		// 提取文件夹路径
		return fullPath.substr(0, pos);
	}
	return "";
}
LPCWSTR stringToLPCWSTR(const std::string& str) {
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (len == 0) {
		return nullptr;
	}
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	return wstr;
}
// CExplorerSetting 对话框

IMPLEMENT_DYNAMIC(CExplorerSetting, CDialogEx)

CExplorerSetting::CExplorerSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_EXPLORER, pParent)
{
	touming = false;
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
		lRet = RegGetValueW(hKey,0, L"IsShortcut", RRF_RT_ANY , 0, ab, abc);
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
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
		EndDialog(0);
	}
	RegCloseKey(hKey);
	const wchar_t* explorerName = L"explorer.exe";
	const wchar_t* targetDllName = L"ExplorerBlurMica.dll";
	DWORD explorerProcessId = GetProcessId(explorerName);
	if (explorerProcessId == 0) 
	{
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
	}
	if (IsDllLoaded(explorerProcessId, targetDllName)) 
	{
		m_combo1.SetCurSel(1);
		touming = true;
	}
	else 
	{
		m_combo1.SetCurSel(0);
		touming = false;
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
			lRet = RegGetValueW(hKey, 0, L"IsShortcut", RRF_RT_ANY, 0, ab, abc);
			if (lRet == ERROR_SUCCESS)
			{
				lRet = RegDeleteValueW(hKey, L"IsShortcut");
				if (lRet == ERROR_SUCCESS)
				{
					MessageBoxW(L"去除快捷方式小箭头成功，点击重启文件资源管理器按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
				}
			}
		}
		else
		{
			MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
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
			lRet = RegGetValueW(hKey, 0, L"IsShortcut", RRF_RT_ANY, 0, ab, abc);
			if (lRet != ERROR_SUCCESS)
			{
				lRet = RegSetValueExW(hKey, L"IsShortcut", 0, REG_DWORD, 0, 0);
				if (lRet != ERROR_SUCCESS)
				{
					MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
				}
				else
				{
					MessageBoxW(L"开启快捷方式小箭头成功，点击重启文件资源管理器按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
				}
			}
		}
		RegCloseKey(hKey);
	}
	std::string directory = getExecutableDirectory();
	LPCWSTR Directory = stringToLPCWSTR(directory);
	CString a = L"\\register.cmd";
	CString b = L"\\uninstall.cmd";
	int j = m_combo1.GetCurSel();
	if (j == 0)
	{
		ShellExecuteW(0, L"runas", Directory + b, 0, 0, SW_HIDE);
	}
	else
	{
		ShellExecuteW(0, L"runas", Directory + a, 0, 0, SW_HIDE);
	}
	delete[] Directory;
	EndDialog(0);
}