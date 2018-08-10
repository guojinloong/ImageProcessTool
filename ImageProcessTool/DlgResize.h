#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgResize �Ի���

class CDlgResize : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResize)

public:
	CDlgResize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResize();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool m_bChanged1;
	bool m_bChanged2;
	bool m_bProcessed;
	int m_width;
	int m_oldWidth;
	int m_height;
	int m_oldHeight;
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	CComboBox m_comboBox;
	CButton m_proportionCheck;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();
	int m_interWays;
	bool m_proportion;
	double m_scaleX;
	double m_scaleY;
	CEdit m_heightEdit;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL ContinueModal();
};
