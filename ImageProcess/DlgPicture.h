#pragma once


// CDlgPicture �Ի���

class CDlgPicture : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPicture)

public:
	CDlgPicture(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPicture();

// �Ի�������
	enum { IDD = IDD_DIALOG_PICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
