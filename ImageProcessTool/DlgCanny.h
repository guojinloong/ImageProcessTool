#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgCanny �Ի���

class CDlgCanny : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCanny)

public:
	CDlgCanny(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCanny();

// �Ի�������
	enum { IDD = IDD_DIALOG_CANNY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool m_bChanged1;
	bool m_bChanged2;
	bool m_bProcessed;
	int m_cannyThreshold1;
	int m_oldCannyThreshold1;
	int m_cannyThreshold2;
	int m_oldCannyThreshold2;
	int m_apertureSize;
	CComboBox m_comboBox;
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL ContinueModal();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
};
