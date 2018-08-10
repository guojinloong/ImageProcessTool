#pragma once
#include "afxwin.h"


// CDlgConvexHull 对话框

class CDlgConvexHull : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConvexHull)

public:
	CDlgConvexHull(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConvexHull();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONVEXHULL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_colorR;
	int m_colorG;
	int m_colorB;
	bool m_direction;
	CString m_path;
	CEdit m_edit;
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CButton m_button1;
	CButton m_button2;
	CComboBox m_comboBox;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	BOOL m_select;
	BOOL m_bRandColor;
};
