#pragma once
#include "afxwin.h"


// CDlgAffineWarpRotation �Ի���

class CDlgAffineWarpRotation : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAffineWarpRotation)

public:
	CDlgAffineWarpRotation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAffineWarpRotation();

// �Ի�������
	enum { IDD = IDD_DIALOG_AFFINE_WARP_ROTATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	int m_colorR;
	int m_colorG;
	int m_colorB;
	int m_angle;
	double m_scale;
	CComboBox m_comboBox;
	virtual BOOL OnInitDialog();
	int m_interWays;
};
