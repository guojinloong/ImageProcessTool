#pragma once


// CDlgPicture 对话框

class CDlgPicture : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPicture)

public:
	CDlgPicture(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPicture();

// 对话框数据
	enum { IDD = IDD_DIALOG_PICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
