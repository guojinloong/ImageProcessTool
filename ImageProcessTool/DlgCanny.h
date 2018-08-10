#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgCanny 对话框

class CDlgCanny : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCanny)

public:
	CDlgCanny(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCanny();

// 对话框数据
	enum { IDD = IDD_DIALOG_CANNY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
