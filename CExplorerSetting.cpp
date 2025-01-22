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
END_MESSAGE_MAP()


// CExplorerSetting 消息处理程序

BOOL CExplorerSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}