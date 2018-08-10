// DlgThreshold.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgThreshold.h"
#include "afxdialogex.h"

extern Mat srcImg,tempImg,dstImg;

// CDlgThreshold �Ի���

IMPLEMENT_DYNAMIC(CDlgThreshold, CDialogEx)

CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgThreshold::IDD, pParent)
	, m_threshold(127)
	, m_maxValue(255)
	, m_thresholdType(THRESH_BINARY)
{

}

CDlgThreshold::~CDlgThreshold()
{
}

void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_threshold);
	DDV_MinMaxInt(pDX, m_threshold, 1, 254);
	DDX_Text(pDX, IDC_EDIT2, m_maxValue);
	DDV_MinMaxInt(pDX, m_maxValue, 1, 255);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgThreshold::OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgThreshold::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDlgThreshold::OnNMCustomdrawSlider2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgThreshold::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgThreshold::OnEnChangeEdit2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgThreshold ��Ϣ�������



BOOL CDlgThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_comboBox.AddString(_T("BINARY"));
	m_comboBox.AddString(_T("BINARY_INV"));
	m_comboBox.AddString(_T("TRUNC"));
	m_comboBox.AddString(_T("TOZERO"));
	m_comboBox.AddString(_T("TOZERO_INV"));
	m_comboBox.SetCurSel(0);

	m_slider1.SetRange(1,254);
	m_slider1.SetTicFreq(1);
	m_slider2.SetRange(1,255);
	m_slider2.SetTicFreq(1);
	m_slider1.SetPos(m_threshold);
	m_slider2.SetPos(m_maxValue);
	SetTimer(1,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgThreshold::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_oldThreshold == m_threshold)
	{
		if(m_bChanged1 == true)	m_bProcessed = false;
		m_bChanged1 = false;
	}
	else m_bChanged1 = true;
	m_oldThreshold = m_threshold;

	if(m_oldMaxValue == m_maxValue)
	{
		if(m_bChanged2 == true)	m_bProcessed = false;
		m_bChanged2 = false;
	}
	else m_bChanged2 = true;
	m_oldMaxValue = m_maxValue;

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgThreshold::ContinueModal()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_bProcessed == false)
	{
		threshold(srcImg,tempImg,m_threshold,m_maxValue,m_thresholdType);
		AfxGetMainWnd()->Invalidate(FALSE);
		AfxGetMainWnd()->UpdateWindow();
		m_bProcessed = true;
	}
	return CDialogEx::ContinueModal();
}


void CDlgThreshold::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string = _T("BINARY"))	m_thresholdType = THRESH_BINARY;
	else if(string = _T("BINARY_INV"))	m_thresholdType = THRESH_BINARY_INV;
	else if(string = _T("TRUNC"))	m_thresholdType = THRESH_TRUNC;
	else if(string = _T("TOZERO"))	m_thresholdType = THRESH_TOZERO;
	else if(string = _T("TOZERO_INV"))	m_thresholdType = THRESH_TOZERO_INV;
	else m_thresholdType = MORPH_RECT;
	m_bProcessed = false;
}


void CDlgThreshold::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_threshold = m_slider1.GetPos();
	if(m_threshold>=m_maxValue)
	{
		m_maxValue = m_threshold;
		m_slider2.SetPos(m_maxValue);
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgThreshold::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_maxValue = m_slider2.GetPos();
	if(m_threshold>=m_maxValue)
	{
		m_threshold = m_maxValue;
		m_slider1.SetPos(m_threshold);
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgThreshold::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_threshold<1)	m_threshold = 1;
	else if(m_threshold>254)	m_threshold = 254;
	m_slider1.SetPos(m_threshold);
	if(m_threshold>=m_maxValue)
	{
		m_maxValue = m_threshold;
		m_slider2.SetPos(m_maxValue);
	}
	UpdateData(false);
}


void CDlgThreshold::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_maxValue<1)	m_maxValue = 1;
	else if(m_maxValue>255)	m_maxValue = 255;
	m_slider2.SetPos(m_maxValue);
	if(m_threshold>=m_maxValue)
	{
		m_threshold = m_maxValue;
		m_slider1.SetPos(m_threshold);
	}
	UpdateData(false);
}