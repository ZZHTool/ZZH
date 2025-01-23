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

// 注入DLL到指定进程
BOOL InjectDLL(DWORD processId, const wchar_t* dllPath) 
{
	// 打开目标进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (hProcess == NULL) 
	{
		return FALSE;
	}
	// 在目标进程中分配内存
	SIZE_T dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);
	LPVOID remoteDllPath = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT, PAGE_READWRITE);
	if (remoteDllPath == NULL) 
	{
		CloseHandle(hProcess);
		return FALSE;
	}
	// 将DLL路径写入目标进程
	if (!WriteProcessMemory(hProcess, remoteDllPath, dllPath, dllPathSize, NULL)) 
	{
		VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}
	// 获取LoadLibraryW函数地址
	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryW");
	if (loadLibraryAddr == NULL) 
	{
		VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	// 创建远程线程执行LoadLibraryW
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, remoteDllPath, 0, NULL);
	if (hThread == NULL) 
	{
		VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}
	// 等待线程结束
	WaitForSingleObject(hThread, INFINITE);
	// 清理资源
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return TRUE;
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
	}
	else 
	{
		m_combo1.SetCurSel(0);
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
			lRet = RegGetValueW(hKey, NULL, L"IsShortcut", RRF_RT_ANY, 0, ab, abc);
			if (lRet != ERROR_SUCCESS)
			{
				lRet = RegSetValueExW(hKey, L"IsShortcut", 0, REG_DWORD, 0, 0);
				if (lRet != ERROR_SUCCESS)
				{
					MessageBoxW(L"打开注册表失败！", L"失败！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
				}
				else
				{
					ShellExecuteW(0, L"runas", L"cmd.exe", L"/c taskkill /f /im explorer.exe & start explorer.exe", 0, SW_HIDE);
					MessageBoxW(L"开启快捷方式小箭头成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
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
		//ShellExecuteW(0, L"runas", L"E:\\系统\\C++\\ZZH系统工具\\x64\\Release\\register.cmd",0, 0, SW_HIDE);
		const wchar_t* explorerName = L"explorer.exe";
		const wchar_t* dllPath = L"ExplorerBlurMica.dll";
		// 获取资源管理器进程ID
		DWORD explorerProcessId = GetProcessId(explorerName);
		if (explorerProcessId == 0) 
		{
			MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
		}
		// 注入DLL
		if (InjectDLL(explorerProcessId, dllPath)) 
		{
			MessageBoxW(L"开启文件资源管理器背景透明成功！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
		}
		else 
		{
			MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
		}
	}
	EndDialog(0);
}
