// DlgHoughLines.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgHoughLinesP.h"
#include "afxdialogex.h"


// CDlgHoughLinesP �Ի���

IMPLEMENT_DYNAMIC(CDlgHoughLinesP, CDialogEx)

CDlgHoughLinesP::CDlgHoughLinesP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHoughLinesP::IDD, pParent)
	, m_threshold(300)
	, m_rho(1)
	, m_theta(1)
	, m_maxLineLength(0)
	, m_maxLineGap(0)
{

}

CDlgHoughLinesP::~CDlgHoughLinesP()
{
}

void CDlgHoughLinesP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_threshold);
	DDV_MinMaxInt(pDX, m_threshold, 300, 1000);
	DDX_Text(pDX, IDC_EDIT2, m_rho);
	DDV_MinMaxDouble(pDX, m_rho, 1, 10);
	DDX_Text(pDX, IDC_EDIT3, m_theta);
	DDV_MinMaxDouble(pDX, m_theta, 1, 360);
	DDX_Text(pDX, IDC_EDIT4, m_maxLineLength);
	DDV_MinMaxDouble(pDX, m_maxLineLength, 0, 10);
	DDX_Text(pDX, IDC_EDIT5, m_maxLineGap);
	DDV_MinMaxDouble(pDX, m_maxLineGap, 0, 10);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CDlgHoughLinesP, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgHoughLinesP::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgHoughLinesP::OnEnChangeEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgHoughLinesP::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgHoughLinesP::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDlgHoughLinesP::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CDlgHoughLinesP::OnDeltaposSpin4)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgHoughLinesP::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgHoughLinesP::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgHoughLinesP::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgHoughLinesP::OnEnChangeEdit5)
END_MESSAGE_MAP()


// CDlgHoughLinesP ��Ϣ�������


BOOL CDlgHoughLinesP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_slider.SetRange(300,1000);
	m_slider.SetTicFreq(1);
	m_slider.SetPos(m_threshold);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgHoughLinesP::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_threshold = m_slider.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgHoughLinesP::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_threshold < 300)	m_threshold = 300;
	else if(m_threshold > 1000)	m_threshold = 1000;
	else m_slider.SetPos(m_threshold);
	UpdateData(false);
}


void CDlgHoughLinesP::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_rho < 1)	m_rho = 1;
	else if(m_rho > 10)	m_rho = 10;
	UpdateData(false);
}


void CDlgHoughLinesP::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_theta < 1)	m_theta = 1;
	else if(m_theta > 360)	m_theta = 360;
	UpdateData(false);
}


void CDlgHoughLinesP::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_maxLineLength < 0)	m_maxLineLength = 0;
	else if(m_maxLineLength > 10)	m_maxLineLength = 10;
	UpdateData(false);
}


void CDlgHoughLinesP::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_maxLineGap < 0)	m_maxLineGap = 0;
	else if(m_maxLineGap > 10)	m_maxLineGap = 10;
	UpdateData(false);
}



void CDlgHoughLinesP::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_rho>1)	m_rho -= 1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_rho<10)	m_rho += 1;
	}
	UpdateData(false);

	*pResult = 0;
}


void CDlgHoughLinesP::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_theta>1)	m_theta -= 1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_theta<360)	m_theta += 1;
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgHoughLinesP::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_maxLineLength>0)	m_maxLineLength -= 0.1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_maxLineLength<10)	m_maxLineLength += 0.1;
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgHoughLinesP::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_maxLineGap>0)	m_maxLineGap -= 0.1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_maxLineGap<10)	m_maxLineGap += 0.1;
	}
	UpdateData(false);
	*pResult = 0;
}