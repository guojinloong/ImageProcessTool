#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgMorphology 对话框

class CDlgMorphology : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMorphology)

public:
	CDlgMorphology(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMorphology();

// 对话框数据
	enum { IDD = IDD_DIALOG_MORPHOLOGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_elementShape;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	int shape;
	CString m_string;
	CSliderCtrl m_slider;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_structElementSize;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
};
