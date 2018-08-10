#pragma once
#include "afxwin.h"


// CDlgAffineWarp 对话框

class CDlgAffineWarp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAffineWarp)

public:
	CDlgAffineWarp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAffineWarp();

// 对话框数据
	enum { IDD = IDD_DIALOG_AFFINE_WARP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HCURSOR m_hCursor;
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboBox1;
	int m_colorR;
	int m_colorG;
	int m_colorB;
	int m_interWays;
	int m_affineWays;
	int m_affineDirection;
	int m_pointSelect;
	CPoint m_point[3];
	CPoint m_anchor;
	double m_angle;
	double m_scale;
	int m_button;
	float m_viewScale;
	CButton m_radio;
	CComboBox m_comboBox2;
	CvvImage image;
	CRect rect;
	HDC hDC;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	virtual BOOL ContinueModal();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnCbnSelchangeCombo2();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
