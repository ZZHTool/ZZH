#include "afxdialogex.h"
#include "CHome.h"
#include "CSettings.h"
#include "pch.h"
#include <afx.h>
#include <afxdd_.h>
#include <afxmsg_.h>
#include <afxstr.h>
#include <afxwin.h>
#include <atltypes.h>
#include <libloaderapi.h>
#include <minwindef.h>
#include <resource.h>
#include <WinBase.h>
#include <windef.h>
#include <winerror.h>
#include <winnt.h>
#include <winreg.h>
#include <WinUser.h>
#include <tchar.h>
#include <processenv.h>
#include <shellapi.h>
#pragma comment(lib, "shlwapi.lib")

int GetRegistryIntValue(const wchar_t* keyPath, const wchar_t* valueName)
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
		MessageBoxW(0,L"开机自启相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
	}
	return intValue;
}
// CSettings 对话框

IMPLEMENT_DYNAMIC(CSettings, CDialogEx)

CSettings::CSettings(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS, pParent)
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_COMBO2, m_combo1);
	DDX_Control(pDX, IDC_COMBO3, m_combo2);
	DDX_Control(pDX, IDC_STATIC6, m_ziqi);
	DDX_Control(pDX, IDC_STATIC9, m_clean);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}


BEGIN_MESSAGE_MAP(CSettings, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettings::OnBnClickedOk2)
	ON_BN_CLICKED(IDC_BUTTON2, &CSettings::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CSettings::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CSettings 消息处理程序
BOOL CSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_combo.AddString(L"开启");
	m_combo.AddString(L"关闭");
	m_combo1.AddString(L"开启");
	m_combo1.AddString(L"关闭");
	m_combo2.AddString(L"开启");
	m_combo2.AddString(L"关闭");
	CString Str;
	int a = GetRegistryIntValue(L"SOFTWARE\\ZZH System Tool\\Translucent", L"setting");
	Str.Format(L"%d",a);
	m_edit1.SetWindowTextW(Str);
	m_edit1.SetLimitText(3);
	CString str;
	CString string;
	HKEY hKey1;
	HKEY hKey2;
	HKEY hKey3;
	HKEY hKey4;
	CString lpRun = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	CString lpRun1 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
	CString lpRun2 = L"SOFTWARE\\ZZH System Tool";
	CString lpRun3 = L"SOFTWARE\\ZZH System Tool\\NoAutoClean";
	CString lpRun4 = L"SOFTWARE\\ZZH System Tool\\Translucent";
	CString lpRun5 = L"SOFTWARE\\ZZH System Tool\\Translucent\\NoTranslucent";
	long lRet1 = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun1, 0, KEY_ALL_ACCESS, &hKey1);
	long lRet2 = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_ALL_ACCESS, &hKey2);
	long lRet3 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun2, 0, KEY_ALL_ACCESS, &hKey3);
	long lRet4 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey4);
	if (lRet1 != ERROR_SUCCESS || lRet2 != ERROR_SUCCESS)
	{
		MessageBoxW(L"开机自启相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
		str.Format(L"当前:无法获取");
		m_ziqi.SetWindowTextW(str);
		m_combo.SetCurSel(0);
	}
	else
	{
		//找到程序自身路径
		TCHAR pFileName[MAX_PATH] = {};
		GetModuleFileName(NULL, pFileName, MAX_PATH);
		//判断是否已经设置开机启动
		TCHAR PowerBoot[MAX_PATH] = {};
		DWORD nLongth = MAX_PATH;
		long result1 = RegGetValue(hKey1, NULL, L"ZZH系统工具", RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);
		long result2 = RegGetValue(hKey2, NULL, L"ZZH系统工具", RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);
		if (result1 == ERROR_SUCCESS || result2 == ERROR_SUCCESS)
		{
			str.Format(L"当前:开启");
			m_ziqi.SetWindowTextW(str);
			m_combo.SetCurSel(1);
		}
		else
		{
			str.Format(L"当前:关闭");
			m_ziqi.SetWindowTextW(str);
			m_combo.SetCurSel(0);
		}
	}
	if (lRet3 != ERROR_SUCCESS)
	{
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈!", L"软件错误", MB_TOPMOST | MB_ICONERROR);
		str.Format(L"当前:无法获取");
		m_clean.SetWindowTextW(str);
		m_combo.SetCurSel(0);
	}
	else
	{
		long lRet4 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun3, 0, KEY_ALL_ACCESS, &hKey3);
		if (lRet4 != ERROR_SUCCESS)
		{
			string.Format(L"当前:开启");
			m_clean.SetWindowTextW(string);
			m_combo1.SetCurSel(1);
		}
		else
		{
			string.Format(L"当前:关闭");
			m_clean.SetWindowTextW(string);
			m_combo1.SetCurSel(0);
		}
	}
	long lRet5 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun5, 0, KEY_ALL_ACCESS, &hKey4);
	if (lRet4 != ERROR_SUCCESS)
	{
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
	}
	else if (a == 100)
	{
		m_combo2.SetCurSel(0);
		m_edit1.EnableWindow(FALSE);
	}
	else if (lRet5 != ERROR_SUCCESS)
	{
		m_combo2.SetCurSel(1);
		m_edit1.EnableWindow(TRUE);
	}
	else
	{
		m_combo2.SetCurSel(0);
		m_edit1.EnableWindow(FALSE);
	}
	RegCloseKey(hKey1);
	RegCloseKey(hKey2);
	RegCloseKey(hKey3);
	RegCloseKey(hKey4);
	return TRUE;
}

void restartApplication()
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

void CSettings::OnBnClickedOk2()
{
	HKEY hKey{};
	HKEY hKey1;
	HKEY hKey2;
	HKEY hKey3;
	HKEY hKey4;
	//找到系统的启动项
	CString lpRun = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	CString lpRun1 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
	CString lpRun2 = L"SOFTWARE\\ZZH System Tool";
	CString lpRun3 = L"SOFTWARE\\ZZH System Tool\\NoAutoClean";
	CString lpRun4 = L"SOFTWARE\\ZZH System Tool\\Translucent";
	CString lpRun5 = L"SOFTWARE\\ZZH System Tool\\Translucent\\NoTranslucent";
	//打开启动项
	long lRet1 = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun1, 0, KEY_ALL_ACCESS, &hKey1);
	long lRet2 = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_ALL_ACCESS, &hKey2);
	long lRet3 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun2, 0, KEY_ALL_ACCESS, &hKey3);
	long lRet4 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey4);
	if (lRet1 != ERROR_SUCCESS || lRet2 != ERROR_SUCCESS)
	{
		MessageBoxW(L"开机自启相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题！", L"软件错误！", MB_TOPMOST | MB_ICONERROR);
	}
	else
	{
		//找到程序自身路径
		TCHAR pFileName[MAX_PATH] = {};
		GetModuleFileName(NULL, pFileName, MAX_PATH);
		//判断是否已经设置开机启动
		TCHAR ZZH[MAX_PATH] = {};
		DWORD nLongth = MAX_PATH;
		/*long result1 = RegGetValue(hKey1, NULL, L"ZZH系统工具", RRF_RT_REG_SZ, 0, ZZH, &nLongth);
		long result2 = RegGetValue(hKey2, NULL, L"ZZH系统工具", RRF_RT_REG_SZ, 0, ZZH, &nLongth);*/
		int i = m_combo.GetCurSel();
		if (i == 0)
		{
			//取消自动启动
			lRet1 = RegDeleteValue(hKey1, L"ZZH系统工具");
			lRet2 = RegDeleteValue(hKey2, L"ZZH系统工具");
			if (lRet1 == ERROR_SUCCESS || lRet2 == ERROR_SUCCESS)
			{
				MessageBoxW(L"已成功关闭开机自启！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
			}
		}
		else
		{
			//设置自启
			lRet1 = RegSetValueEx(hKey1, L"ZZH系统工具", 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1) * sizeof(TCHAR));
			lRet2 = RegSetValueEx(hKey2, L"ZZH系统工具", 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1) * sizeof(TCHAR));
			if (lRet1 == ERROR_SUCCESS || lRet2 == ERROR_SUCCESS)
			{
				MessageBoxW(L"已成功设置开机自启！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
			}
		}
	}
	if (lRet3 != ERROR_SUCCESS)
	{
		MessageBoxW(L"自动优化内存相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题！", L"软件错误！", MB_TOPMOST | MB_ICONERROR);
	}
	else
	{
		int j = m_combo1.GetCurSel();
		if (j == 0)
		{
			long result = RegCreateKeyExW(HKEY_CURRENT_USER, lpRun3, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey3, NULL);
			if (result == ERROR_SUCCESS)
			{
				MessageBoxW(L"关闭自动优化内存成功，点击下方重启软件按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
				CWnd* hWndReceiver = FindWindowW(0, L"ZZH系统工具");
				if (hWndReceiver)
				{
					// 发送自定义消息
					::SendMessageW(HWND(hWndReceiver), WM_AUTOCLEAN, 1, 0);
				}
			}
		}
		else
		{
			long result = RegDeleteKeyW(HKEY_CURRENT_USER, lpRun3);
			if (result == ERROR_SUCCESS)
			{
				MessageBoxW(L"开启自动优化内存成功，点击下方重启软件按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
				CWnd* hWndReceiver = FindWindowW(0, L"ZZH系统工具");
				if (hWndReceiver)
				{
					// 发送自定义消息
					::SendMessageW(HWND(hWndReceiver), WM_AUTOCLEAN, 1, 0);
				}
			}
		}
	}
	if (lRet4 != ERROR_SUCCESS)
	{
		MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
	}
	else
	{
		long lRet5 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun5, 0, KEY_ALL_ACCESS, &hKey1);
		int k = m_combo2.GetCurSel();
		if (k == 0)
		{
			if (lRet5 != ERROR_SUCCESS)
			{
				long result = RegCreateKeyExW(HKEY_CURRENT_USER, lpRun5, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey1, NULL);
				if (result == ERROR_SUCCESS)
				{
					int set = 100;
					RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
					RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
					MessageBoxW(L"关闭窗体透明成功，点击下方重启软件按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
				}
				else
				{
					MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
				}
			}
		}
		else
		{
			if (lRet5 == ERROR_SUCCESS)
			{
				long result = RegDeleteKeyW(HKEY_CURRENT_USER, lpRun5);
				if (result == ERROR_SUCCESS)
				{
					HKEY hKey;
					long result1 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
					int set = GetDlgItemInt(IDC_EDIT1);
					int settings = GetRegistryIntValue(lpRun4,L"setting");
					if (set != settings)
					{
						if (set < 20)
						{
							int c = MessageBoxW(L"当前设置的不透明度过低!将难以看清软件界面，是否继续设置透明度？", L"透明度过低！", MB_TOPMOST | MB_ICONERROR | MB_YESNO);
							if (c == IDYES)
							{
								goto shezhi1;
							}
						}
						else if (set == 100)
						{
							MessageBoxW(L"当不透明度为100%时，窗体透明将关闭！", L"温馨提示", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
							set == 100;
							RegCreateKeyExW(HKEY_CURRENT_USER, lpRun5, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey1, NULL);
							RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
							RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
						}
						else
						{
						shezhi1:
							long result2 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
							long result = RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
							if (result == ERROR_SUCCESS && result2 == ERROR_SUCCESS)
							{
								MessageBoxW(L"窗体透明设置成功，点击下方重启软件按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
							}
							else
							{
								MessageBoxW(L"软件透明度设置失败，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
							}
						}
					}
					else if (set == 100)
					{
						MessageBoxW(L"当不透明度为100%时，窗体透明将关闭！", L"温馨提示", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
						set == 100;
						RegCreateKeyExW(HKEY_CURRENT_USER, lpRun5, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey1, NULL);
						RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
						RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
					}
				}
				else
				{
					MessageBoxW(L"相关服务错误，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
				}
			}
			else
			{
				HKEY hKey;
				long result1 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
				int set = GetDlgItemInt(IDC_EDIT1);
				int settings = GetRegistryIntValue(lpRun4, L"setting");
				if (set != settings)
				{
					if (set < 20)
					{
						int c = MessageBoxW(L"当前设置的透明度过低!将难以看清软件界面，是否继续设置透明度？", L"透明度过低！", MB_TOPMOST | MB_ICONERROR | MB_YESNO);
						if (c == IDYES)
						{
							goto shezhi;
						}
					}
					else if (set == 100)
					{
						MessageBoxW(L"当不透明度为100%时，窗体透明将关闭！", L"温馨提示", MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
						set == 100;
						RegCreateKeyExW(HKEY_CURRENT_USER, lpRun5, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey1, NULL);
						RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
						RegSetValueExW(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
					}
					else
					{
						shezhi:
						long result2 = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun4, 0, KEY_ALL_ACCESS, &hKey);
						long result = RegSetValueEx(hKey, L"setting", 0, REG_DWORD, reinterpret_cast<BYTE*>(&set), sizeof(DWORD));
						if (result == ERROR_SUCCESS && result2 == ERROR_SUCCESS)
						{
							MessageBoxW(L"窗体透明设置成功，点击下方重启软件按钮生效！", L"成功！", MB_ICONINFORMATION | MB_TOPMOST);
						}
						else
						{
							MessageBoxW(L"软件透明度设置失败，请使用管理员身份重新启动程序，或向ZZH反馈问题!", L"软件错误", MB_TOPMOST | MB_ICONERROR | MB_OK);
						}
					}
				}
			}
		}
	}
	RegCloseKey(hKey);
	RegCloseKey(hKey1);
	RegCloseKey(hKey2);
	RegCloseKey(hKey3);
	RegCloseKey(hKey4);
	EndDialog(0);
}


void CSettings::OnBnClickedButton2()
{
	EndDialog(0);
}

void CSettings::OnPaint()
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
HCURSOR CSettings::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSettings::OnCbnSelchangeCombo3()
{
	int k = m_combo2.GetCurSel();
	if (k == 0)
	{
		m_edit1.EnableWindow(FALSE);
	}
	else
	{
		m_edit1.EnableWindow(TRUE);
	}
}
