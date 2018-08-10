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
};
