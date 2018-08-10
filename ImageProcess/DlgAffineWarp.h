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
	CComboBox m_comboBox;
	int m_colorR;
	int m_colorG;
	int m_colorB;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	int m_interWays;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	virtual BOOL ContinueModal();
	CPoint m_point[3];
	CPoint m_anchor;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int m_affineWays;
	int m_pointSelect;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	CvvImage image;
	CRect rect;
	HDC hDC;
	double m_angle;
	double m_scale;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit4();
	int m_button;
	CButton m_radio;
	float m_viewScale;
	CComboBox m_comboBox1;
	afx_msg void OnCbnSelchangeCombo2();
	int m_affineDirection;
};
