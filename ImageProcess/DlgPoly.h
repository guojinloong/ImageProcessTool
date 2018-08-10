#pragma once
#include "afxwin.h"


// CDlgPoly �Ի���

class CDlgPoly : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPoly)

public:
	CDlgPoly(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPoly();

// �Ի�������
	enum { IDD = IDD_DIALOG_POLY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_colorR;
	int m_colorG;
	int m_colorB;
	BOOL m_bRandColor;
	BOOL m_select1;
	BOOL m_select2;
	BOOL m_select3;
	BOOL m_select4;
	BOOL m_select5;
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CButton m_button;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
