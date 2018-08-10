#pragma once
#include "afxwin.h"


// CDlgEdgeSobel 对话框

class CDlgEdgeSobel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEdgeSobel)

public:
	CDlgEdgeSobel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEdgeSobel();

// 对话框数据
	enum { IDD = IDD_DIALOG__EDGE_SOBEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ddepth;
	CComboBox m_apertureSize;
	CComboBox m_dx;
	CComboBox m_dy;
	double m_scale;
	double m_delta;
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	int m_depth;
	int m_kSize;
	int m_dX;
	int m_dY;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
};
