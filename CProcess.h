#include "afxdialogex.h"
#include <afx.h>
#include <afxwin.h>
#include <MyHead.h>
#include <ClistCtrlEx.h>
#include <minwindef.h>
#include <WinUser.h>


// CTask 对话框

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = nullptr);   // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void OnRclickListTask(NMHDR* pNMHDR, LRESULT* pResult);
	void OnEndprocess();
	void OnSuspenprocess();
	void OnResumeprocess();
	void OnOpenfile();
	afx_msg void OnFileinfo();
	afx_msg void OnEnummoudle();
	afx_msg void OnEnumthread();
	DECLARE_MESSAGE_MAP()
public:
	void Fun();
	CMenu m_Menu;
	ClistCtrlEx m_List_Task;
	afx_msg void OnLvnItemchangedListTask(NMHDR* pNMHDR, LRESULT* pResult);
};
