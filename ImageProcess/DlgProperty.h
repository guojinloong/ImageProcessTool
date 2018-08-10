#pragma once
#include "afxwin.h"


// CDlgProperty 对话框

class CDlgProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProperty)

public:
	CDlgProperty(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProperty();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_colorMode;
	int m_grayMode;
	int m_depth;
	int m_channels;
	int m_ndims;
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
	CComboBox m_comboBox3;
	CComboBox m_comboBox4;
	CComboBox m_comboBox5;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo5();
};
