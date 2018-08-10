#pragma once
#include "afxcmn.h"


// CDlgHoughLinesP 对话框

class CDlgHoughLinesP : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHoughLinesP)

public:
	CDlgHoughLinesP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHoughLinesP();

// 对话框数据
	enum { IDD = IDD_DIALOG__HOUGHLINESP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_threshold;
	double m_rho;
	double m_theta;
	double m_maxLineLength;
	double m_maxLineGap;
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
