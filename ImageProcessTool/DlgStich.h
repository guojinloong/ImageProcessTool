#pragma once


// CDlgStich �Ի���

class CDlgStich : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStich)

public:
	CDlgStich(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStich();

// �Ի�������
	enum { IDD = IDD_DIALOG_STITCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString filePath[10];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
};
