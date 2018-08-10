#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgThresholdAdaptive 对话框

class CDlgThresholdAdaptive : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThresholdAdaptive)

public:
	CDlgThresholdAdaptive(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgThresholdAdaptive();

// 对话框数据
	enum { IDD = IDD_DIALOG_THRESHOLD_ADAPTIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool m_bChanged1;
	bool m_bChanged2;
	bool m_bProcessed;
	int m_blockSize;
	int m_oldBlockSize;
	int m_maxValue;
	int m_oldMaxValue;
	int m_adaptiveMethod;
	int m_thresholdType;
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL ContinueModal();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
};
