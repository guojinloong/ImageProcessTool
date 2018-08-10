#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgDefog 对话框

class CDlgDefog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDefog)

public:
	CDlgDefog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDefog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEFOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboBox1;
	CComboBox m_comboBox2;
	CComboBox m_comboBox3;
	//CScrollBar m_scrollBar1;
	//CScrollBar m_scrollBar2;
	//CScrollBar m_scrollBar3;
	int m_algorithm;
	int m_guide;
	int m_depth;
	int m_A;
	int m_minWindowSize;
	int m_radius;
	double m_epsilon;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	virtual BOOL OnInitDialog();
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	CSliderCtrl m_slider3;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	Mat guidedFilter( Mat I, Mat p, int r, double eps );
	CButton m_caculateCheck;
	afx_msg void OnBnClickedCheck1();
	int m_maxValue;
	CEdit m_edit1;
	virtual BOOL ContinueModal();
	CProgressCtrl *m_progress;
	CRect rect;
	CvvImage image;
};
