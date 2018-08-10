#pragma once
#include "afxwin.h"


// CDlgEdgeLaplacian 对话框

class CDlgEdgeLaplacian : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEdgeLaplacian)

public:
	CDlgEdgeLaplacian(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEdgeLaplacian();

// 对话框数据
	enum { IDD = IDD_DIALOG__EDGE_LAPLACIAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ddepth;
	CComboBox m_apertureSize;
	double m_scale;
	double m_delta;
	int m_depth;
	int m_kSize;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
};
