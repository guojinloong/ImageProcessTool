#pragma once
#include "afxwin.h"


// CDlgProperty �Ի���

class CDlgProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProperty)

public:
	CDlgProperty(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProperty();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
