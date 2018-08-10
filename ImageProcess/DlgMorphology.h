#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgMorphology �Ի���

class CDlgMorphology : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMorphology)

public:
	CDlgMorphology(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMorphology();

// �Ի�������
	enum { IDD = IDD_DIALOG_MORPHOLOGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool m_bChanged;
	bool m_bProcessed;
	int m_morphWays;
	int shape;
	int m_structElementSize;
	int m_oldStructElementSize;
	CComboBox m_elementShape;
	CSliderCtrl m_slider;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL ContinueModal();
};
