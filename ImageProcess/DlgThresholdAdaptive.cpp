// DlgThresholdAdaptive.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgThresholdAdaptive.h"
#include "afxdialogex.h"


// CDlgThresholdAdaptive �Ի���

IMPLEMENT_DYNAMIC(CDlgThresholdAdaptive, CDialogEx)

CDlgThresholdAdaptive::CDlgThresholdAdaptive(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgThresholdAdaptive::IDD, pParent)
	, m_blockSize(1)
	, m_maxValue(255)
	, m_type1(ADAPTIVE_THRESH_MEAN_C)
	, m_type2(THRESH_BINARY)
{

}

CDlgThresholdAdaptive::~CDlgThresholdAdaptive()
{
}

void CDlgThresholdAdaptive::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_adaptiveMethod);
	DDX_Control(pDX, IDC_COMBO2, m_thresholdType);
	DDX_Text(pDX, IDC_EDIT1, m_blockSize);
	DDV_MinMaxInt(pDX, m_blockSize, 1, 20);
	DDX_Text(pDX, IDC_EDIT2, m_maxValue);
	DDV_MinMaxInt(pDX, m_maxValue, 1, 255);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
}


BEGIN_MESSAGE_MAP(CDlgThresholdAdaptive, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgThresholdAdaptive::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgThresholdAdaptive::OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgThresholdAdaptive::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgThresholdAdaptive::OnEnChangeEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgThresholdAdaptive::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDlgThresholdAdaptive::OnNMCustomdrawSlider2)
END_MESSAGE_MAP()


// CDlgThresholdAdaptive ��Ϣ�������


void CDlgThresholdAdaptive::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_adaptiveMethod.GetCurSel();
	CString string;
	m_adaptiveMethod.GetLBText(sel,string);
	if(string = _T("BINARY"))	m_type2 = ADAPTIVE_THRESH_MEAN_C;
	else if(string = _T("BINARY_INV"))	m_type2 = ADAPTIVE_THRESH_GAUSSIAN_C;
}


void CDlgThresholdAdaptive::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_thresholdType.GetCurSel();
	CString m_string;
	m_thresholdType.GetLBText(sel,m_string);
	if(m_string = _T("BINARY"))	m_type2 = THRESH_BINARY;
	else if(m_string = _T("BINARY_INV"))	m_type2 = THRESH_BINARY_INV;
	//else if(m_string = _T("TRUNC"))	m_type2 = THRESH_TRUNC;
	//else if(m_string = _T("TOZERO"))	m_type2 = THRESH_TOZERO;
	//else if(m_string = _T("TOZERO_INV"))	m_type2 = THRESH_TOZERO_INV;
	//else m_type2 = MORPH_RECT;

}


void CDlgThresholdAdaptive::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_slider2.SetPos(m_blockSize);
	UpdateData(false);
}


void CDlgThresholdAdaptive::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_slider2.SetPos(m_maxValue);
	UpdateData(false);
}


void CDlgThresholdAdaptive::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_blockSize = m_slider1.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgThresholdAdaptive::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_maxValue = m_slider2.GetPos();
	UpdateData(false);
	*pResult = 0;
}


BOOL CDlgThresholdAdaptive::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_adaptiveMethod.AddString(_T("MEAN_C"));
	m_adaptiveMethod.AddString(_T("GAUSSIAN_C"));
	m_adaptiveMethod.SetCurSel(0);
	m_thresholdType.AddString(_T("BINARY"));
	m_thresholdType.AddString(_T("BINARY_INV"));
	//m_thresholdType.AddString(_T("TRUNC"));
	//m_thresholdType.AddString(_T("TOZERO"));
	//m_thresholdType.AddString(_T("TOZERO_INV"));
	m_thresholdType.SetCurSel(0);

	m_slider1.SetRange(1,20);
	m_slider2.SetRange(1,255);
	m_slider1.SetTicFreq(1);
	m_slider2.SetTicFreq(1);
	m_slider1.SetPos(m_blockSize);
	m_slider2.SetPos(m_maxValue);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
