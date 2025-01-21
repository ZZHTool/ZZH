// CTask.cpp: 实现文件
//

#include "afxdialogex.h"
#include "ClistCtrlEx.h"
#include "CProcess.h"
#include "pch.h"
#include <afx.h>
#include <afxdd_.h>
#include <afxres.h>
#include <afxstr.h>
#include <afxwin.h>
#include <CommCtrl.h>
#include <minwindef.h>
#include <resource.h>
#include <shellapi.h>
#include <synchapi.h>
#include <tchar.h>
#include <windef.h>
#include <winnt.h>
#include <WinUser.h>
#include <afxmsg_.h>
#include <MyMoudleDlg.h>
#include <MyThreadDlg.h>

DWORD g_Pid = 0;
// CTask 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

CProcess::CProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK_DIALOG, pParent)
{

}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASK, m_List_Task);
}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TASK, &CProcess::OnRclickListTask)
	ON_COMMAND(ID_ENDPROCESS, &CProcess::OnEndprocess)
	ON_COMMAND(ID_ENUMMOUDLE, &CProcess::OnEnummoudle)
	ON_COMMAND(ID_OPENFILE, &CProcess::OnOpenfile)
	ON_COMMAND(ID_FILEINFO, &CProcess::OnFileinfo)
	ON_COMMAND(ID_SUSPENPROCESS, &CProcess::OnSuspenprocess)
	ON_COMMAND(ID_RESUMEPROCESS, &CProcess::OnResumeprocess)
	ON_COMMAND(ID_ENUMTHREAD, &CProcess::OnEnumthread)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TASK, &CProcess::OnLvnItemchangedListTask)
END_MESSAGE_MAP()


// CProcess 消息处理程序
void PrintTaskList(ClistCtrlEx &m_List_Task,TASKLIST &nTaskList, DWORD nTaskNum)
{
	HIMAGELIST nHiml = { 0 };//图标列表
	SHFILEINFO nPsfi = { 0 };//文件信息
	ImageList_Destroy(nHiml);//清除图标列表
	nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//订制图标框架
	ImageList_SetBkColor(nHiml, m_List_Task.GetBkColor());//设置图标列表底色
	m_List_Task.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标


	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nIndex = m_List_Task.GetItemCount();

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			 SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON| SHGFI_USEFILEATTRIBUTES| SHGFI_SMALLICON);
		DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//置入图标返回图标下标

		m_List_Task.InsertItem(nIndex, nTaskList.szExeFile[nIndex],nIco);
		m_List_Task.SetItemText(nIndex, 1, nTaskList.th32ProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 2, nTaskList.szPriority[nIndex]);
		m_List_Task.SetItemText(nIndex, 3, nTaskList.WorkingSetSize[nIndex]);
		m_List_Task.SetItemText(nIndex, 4, nTaskList.cntThreads[nIndex]);
		m_List_Task.SetItemText(nIndex, 5, nTaskList.th32ParentProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 6, nTaskList.szExeFilePath[nIndex]);
	}

}

//刷新进程列表
void RefershTaskList(ClistCtrlEx&m_List_Task, TASKLIST &nTaskList, DWORD nTaskNum)
{
	static HIMAGELIST nHiml = { 0 };//图标列表
	static SHFILEINFO nPsfi = { 0 };//文件信息
	//ImageList_Destroy(nHiml);//清除图标列表
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//订制图标框架
		ImageList_SetBkColor(nHiml, m_List_Task.GetBkColor());//设置图标列表底色
		m_List_Task.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标
	}

	DWORD nListNum = m_List_Task.GetItemCount();

	if (nListNum > nTaskNum)
	{
		DWORD nSub = nListNum - nTaskNum;

		for (DWORD i = 0; i < nSub; i++)
		{
			m_List_Task.DeleteItem(m_List_Task.GetItemCount()-1);
		}
	}
	else if(nTaskNum > nListNum)
	{
		DWORD nAdd = nTaskNum - nListNum;
		for (DWORD i = 0; i < nAdd; i++)
		{
			m_List_Task.InsertItem(m_List_Task.GetItemCount(),L"");
		}
	}

	ImageList_SetImageCount(nHiml, nListNum);

	DWORD nIndex = 0;

	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//置入图标返回图标下标
		ImageList_ReplaceIcon(nHiml,i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		m_List_Task.SetItem(nIndex, 0,2,TEXT(""),i,0,0,0);
		m_List_Task.SetItemText(nIndex, 0, nTaskList.szExeFile[nIndex]);
		m_List_Task.SetItemText(nIndex, 1, nTaskList.th32ProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 2, nTaskList.szPriority[nIndex]);
		m_List_Task.SetItemText(nIndex, 3, nTaskList.WorkingSetSize[nIndex]);
		m_List_Task.SetItemText(nIndex, 4, nTaskList.cntThreads[nIndex]);
		m_List_Task.SetItemText(nIndex, 5, nTaskList.th32ParentProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 6, nTaskList.szExeFilePath[nIndex]);

		nIndex++;
	}

}


void CProcess::Fun()
{
	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);
	RefershTaskList(m_List_Task, nTaskList, nNum);
	ClearTaskList(nTaskList);

}
DWORD WINAPI RefershThread(LPVOID lpThreadParameter)
{
	CProcess* nDlg = (CProcess*)lpThreadParameter;
	while (true)
	{
		//刷新进程
		nDlg->Fun();
		Sleep(1000);
	}
}
BOOL CProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_Menu);
	m_List_Task.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Task.InsertColumn(0, TEXT("进程名字"), 0, 250);
	m_List_Task.InsertColumn(1, TEXT("进程PID"), 0, 75);
	m_List_Task.InsertColumn(2, TEXT("优先级"), 0, 80);
	m_List_Task.InsertColumn(3, TEXT("内存使用"), 0, 100);
	m_List_Task.InsertColumn(4, TEXT("线程数量"), 0, 100);
	m_List_Task.InsertColumn(5, TEXT("父进程PID"), 0, 150);
	m_List_Task.InsertColumn(6, TEXT("进程路径"), 0, 350);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	MyRtlAdjustPrivilege();//提升权限

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//枚举进程列表
	PrintTaskList(m_List_Task, nTaskList, nNum);//打印进程到列表
	ClearTaskList(nTaskList);
	return FALSE;
}

void CProcess::OnRclickListTask(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem != -1)
	{
		CMenu* nMenu = m_Menu.GetSubMenu(0);
		POINT pos;
		GetCursorPos(&pos);
		nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
	}
}

//结束进程
void CProcess::OnEndprocess()
{
	DWORD nNow = m_List_Task.GetSelectionEx();
	CString szExeName = m_List_Task.GetItemText(nNow, 0);
	CString szPid = m_List_Task.GetItemText(nNow, 1);

	DWORD nPid;
	_stscanf_s(szPid, TEXT("%d"), &nPid);
	EndProcess(nPid);
}

void CProcess::OnSuspenprocess()
{
	DWORD nNow = m_List_Task.GetSelectionEx();
	CString szPid = m_List_Task.GetItemText(nNow, 1);
	DWORD nPid;
	_stscanf_s(szPid, TEXT("%d"), &nPid);
	SetProcessState(nPid, TRUE);
}

//恢复进程
void CProcess::OnResumeprocess()
{
	DWORD nNow = m_List_Task.GetSelectionEx();
	CString szPid = m_List_Task.GetItemText(nNow, 1);
	DWORD nPid;
	_stscanf_s(szPid, TEXT("%d"), &nPid);
	SetProcessState(nPid, FALSE);
}

//打开文件位置
void CProcess::OnOpenfile()
{
	DWORD nNow = m_List_Task.GetSelectionEx();

	CString szExeName = m_List_Task.GetItemText(nNow, 6);
	OpenFile(szExeName);
}

//文件属性
void CProcess::OnFileinfo()
{
	DWORD nNow = m_List_Task.GetSelectionEx();
	CString szExeName = m_List_Task.GetItemText(nNow, 6);
	OpenFileInfo(this->GetSafeHwnd(), szExeName);
	// TODO: 在此添加命令处理程序代码
}

void CProcess::OnEnummoudle()
{
	DWORD nNow = m_List_Task.GetSelectionEx();
	CString szPid = m_List_Task.GetItemText(nNow, 1);
	_stscanf_s(szPid, TEXT("%d"), &g_Pid);

	MyMoudleDlg * dlg = new MyMoudleDlg;
	dlg->Create(IDD_MYMOUDLEDIALOG, this);
	dlg->ShowWindow(SW_SHOW);
}

//查看线程
void CProcess::OnEnumthread()
{
	DWORD nNow = m_List_Task.GetSelectionEx();
	CString szPid = m_List_Task.GetItemText(nNow, 1);
	_stscanf_s(szPid, TEXT("%d"), &g_Pid);

	MyThreadDlg * dlg = new  MyThreadDlg;
	dlg->Create(IDD_MYTHREAD_DIALOG, this);
	dlg->ShowWindow(SW_SHOW);
}



void CProcess::OnLvnItemchangedListTask(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
