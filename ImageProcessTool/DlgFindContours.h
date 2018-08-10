#pragma once
#include "afxwin.h"


// CDlgFindContours 对话框

class CDlgFindContours : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFindContours)

public:
	CDlgFindContours(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFindContours();

// 对话框数据
	enum { IDD = IDD_DIALOG_FINDCONTOURS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_radio1;
	CButton m_radio2;
	int m_mode;
	int m_method;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int m_colorR;
	int m_colorG;
	int m_colorB;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton1();
	CButton m_check;
	afx_msg void OnBnClickedCheck1();
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CButton m_button;
	bool m_bRandColor;
};
