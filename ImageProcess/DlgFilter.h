#pragma once
#include "afxcmn.h"


// CDlgFilter �Ի���

class CDlgFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilter)

public:
	CDlgFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFilter();

// �Ի�������
	enum { IDD = IDD_DIALOG_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_filterValue;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
};
