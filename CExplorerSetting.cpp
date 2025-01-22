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

}
