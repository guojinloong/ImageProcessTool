#pragma once
#include "afxcmn.h"


// CDlgFilter 对话框

class CDlgFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilter)

public:
	CDlgFilter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFilter();

// 对话框数据
	enum { IDD = IDD_DIALOG_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_filterValue;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_slider;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
};
