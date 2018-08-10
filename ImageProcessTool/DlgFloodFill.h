#pragma once
#include "afxwin.h"


// CDlgFloodFill 对话框

class CDlgFloodFill : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFloodFill)

public:
	CDlgFloodFill(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFloodFill();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOODFILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_upDiff;
	int m_lowDiff;
	int m_maskValue;
	int m_colorR;
	int m_colorG;
	int m_colorB;
	int m_fillMode;
	int m_connectivity;
	bool m_bMask;
	CButton m_maskCheck;
	CButton m_fixedRange;
	CButton m_connectivityCheck;
	CEdit m_maskEdit;
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
};
