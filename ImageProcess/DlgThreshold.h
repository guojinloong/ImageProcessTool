#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgThreshold 对话框

class CDlgThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThreshold)

public:
	CDlgThreshold(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgThreshold();

// 对话框数据
	enum { IDD = IDD_DIALOG_THRESHOLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_threshold;
	int m_maxValue;
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	CComboBox m_thresholdType;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	virtual BOOL OnInitDialog();
	int m_type;
};
