#pragma once


// CDlgStich 对话框

class CDlgStich : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStich)

public:
	CDlgStich(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStich();

// 对话框数据
	enum { IDD = IDD_DIALOG_STITCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString filePath[10];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
};
