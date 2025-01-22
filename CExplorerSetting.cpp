// CExplorerSetting.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "CExplorerSetting.h"
#include <resource.h>


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
	ON_BN_CLICKED(IDOK, &CExplorerSetting::OnBnClickedOk)
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
	}
	RegCloseKey(hKey);
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
	EndDialog(0);
}
