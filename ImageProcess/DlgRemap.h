#pragma once
#include "afxwin.h"


// CDlgRemap 对话框

class CDlgRemap : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRemap)

public:
	CDlgRemap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRemap();

// 对话框数据
	enum { IDD = IDD_DIALOG_AFFINE_REMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_remapWays;
	int m_colorR;
	int m_colorG;
	int m_colorB;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedRadio();
	CComboBox m_comboBox;
	virtual BOOL OnInitDialog();
	int m_interWays;
};
