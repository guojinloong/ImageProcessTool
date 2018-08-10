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
	//void *m_pMain;
	bool m_bChanged;
	bool m_bProcessed;
	int filterWays;
	int m_filterValue;
	int m_oldFilterValue;
	CSliderCtrl m_slider;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	virtual BOOL OnInitDialog();
//	virtual void OnOK();
//	virtual void OnCancel();
	virtual BOOL ContinueModal();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
