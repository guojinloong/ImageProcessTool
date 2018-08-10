#pragma once
#include "afxcmn.h"


// CDlgHoughLines �Ի���

class CDlgHoughLines : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHoughLines)

public:
	CDlgHoughLines(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHoughLines();

// �Ի�������
	enum { IDD = IDD_DIALOG_HOUGHLINES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_threshold;
	double m_rho;
	double m_theta;
	double m_srn;
	double m_stn;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
};
