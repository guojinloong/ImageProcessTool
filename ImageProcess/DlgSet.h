#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgSet 对话框

class CDlgSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSet)
private:
public:
	CDlgSet(CWnd* pParent = NULL);   // 标准构造函数
	//CDlgSet(int,int,int);
	virtual ~CDlgSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HCURSOR m_hCursor;
	COLORREF m_color;
	CFont m_font;
	int m_colorR;
	int m_colorG;
	int m_colorB;
	int m_thickness;
	int m_lineType;
	int m_shift;
	afx_msg void OnCbnSelchangeCombobox1();
	afx_msg void OnCbnSelchangeCombobox2();
	afx_msg void OnCbnSelchangeCombobox3();
	virtual BOOL OnInitDialog();
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
	CComboBox m_comboBox3;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnStnClickedStaticColorBackground();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
};
