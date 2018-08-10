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
