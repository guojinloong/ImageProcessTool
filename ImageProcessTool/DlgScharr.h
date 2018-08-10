#pragma once
#include "afxwin.h"


// CDlgScharr �Ի���

class CDlgScharr : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScharr)

public:
	CDlgScharr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgScharr();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCHARR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ddepth;
	CComboBox m_dx;
	CComboBox m_dy;
	double m_scale;
	double m_delta;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	int m_depth;
	int m_dX;
	int m_dY;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
};
