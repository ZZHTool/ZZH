// Xuan.cpp: 实现文件
//
#include "pch.h"
#include "afxdialogex.h"
#include "Xuan.h"
#include "Resource.h"
#include <afx.h>
#include <afxwin.h>
#include <afxmsg_.h>
#include <random>
#include <minwindef.h>
#include <WinUser.h>
#include <afxdd_.h>
#include <basetsd.h>
#include <vector>
#include <ctime>

// Xuan 对话框

IMPLEMENT_DYNAMIC(Xuan, CDialogEx)

Xuan::Xuan(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XUAN, pParent)
{
	Edit1 = 1;
	Edit2 = 44;
}

void Xuan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_static);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_START, m_start);
	DDX_Control(pDX, IDC_END, m_end);
}


BEGIN_MESSAGE_MAP(Xuan, CDialogEx)
	ON_BN_CLICKED(IDC_START, &Xuan::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Xuan::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &Xuan::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &Xuan::OnEnChangeEdit2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_END, &Xuan::OnBnClickedButton11)
END_MESSAGE_MAP()

BOOL Xuan::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	edit1.Format(L"%d",Edit1);
	m_edit1.SetWindowText(edit1);
	m_edit1.SetLimitText(7);
	edit2.Format(L"%d", Edit2);
	m_edit2.SetWindowText(edit2);
	m_edit1.SetLimitText(7);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// Xuan 消息处理程序

void Xuan::OnBnClickedButton1()
{
	Edit1 = GetDlgItemInt(IDC_EDIT1);
	Edit2 = GetDlgItemInt(IDC_EDIT2);
	edit:
	edit1.Format(L"%d", Edit1);
	m_edit1.SetWindowText(edit1);
	edit2.Format(L"%d", Edit2);
	m_edit2.SetWindowText(edit2);
	while(Edit2 < Edit1)
	{
		MessageBoxA(0, "最小值不能大于最大值！", "输入错误", MB_OK | MB_ICONHAND | MB_TOPMOST);
		Edit1 = 0;
		Edit2 = 44;
		goto edit;
	}
	while(Edit1 > Edit2)
	{
		MessageBoxA(0, "最大值不能小于最小值！", "输入错误", MB_OK | MB_ICONHAND | MB_TOPMOST);
		Edit1 = 0;
		Edit2 = 44;
		goto edit;
	}
	m_start.EnableWindow(FALSE);
	SetTimer(m_timer, 1, 0);
}

void Xuan::OnEnChangeEdit1()
{
	Edit1 = GetDlgItemInt(IDC_EDIT1);
}

void Xuan::OnEnChangeEdit2()
{
	Edit2 = GetDlgItemInt(IDC_EDIT2);
}

void Xuan::OnBnClickedButton2()
{
	KillTimer(m_timer);
	OnClose();
}

void Xuan::OnClose()
{
	KillTimer(m_timer);
	DestroyWindow();
}


void Xuan::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent = m_timer)
	{
		std::default_random_engine xuanhao;
		std::uniform_int_distribution<int> u(Edit1, Edit2);
		// 使用 std::vector 来存储多个随机数
		std::vector<int> randomNumbers;
		xuanhao.seed(time(0));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		randomNumbers.push_back(u(xuanhao));
		CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC1);
		if (pStatic != NULL)
		{
			// 创建字体对象
			CFont font;
			font.CreatePointFont(200, L"0");
			// 设置静态控件的字体
			pStatic->SetFont(&font);
			int i = 0;
			while (i < 10)
			{
				for (int num : randomNumbers)
				{
					number.Format(L"%d", num);
					m_static.SetWindowText(number);
				}
				i++;
			}
			font.DeleteObject();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void Xuan::OnBnClickedButton11()
{
	m_start.EnableWindow(TRUE);
	KillTimer(m_timer);
	/*std::default_random_engine xuanhao;
	std::uniform_int_distribution<int> u(Edit1, Edit2);
	xuanhao.seed(time(0));
	int n = u(xuanhao);
	number.Format(L"%d", n);
	m_static.SetWindowText(number);*/
}

BOOL Xuan::PreTranslateMessage(MSG* pMsg)
{
	////屏蔽ESC键按下
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	//{
	//	return TRUE;
	//}

	//屏蔽Enter键按下
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && pMsg->wParam)
	{
		return TRUE;
	}

	//操作其他msg
	return CDialogEx::PreTranslateMessage(pMsg);
}
