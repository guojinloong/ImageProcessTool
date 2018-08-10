// DlgAffineWarp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgAffineWarp.h"
#include "afxdialogex.h"

extern CvvImage image;

// CDlgAffineWarp �Ի���

IMPLEMENT_DYNAMIC(CDlgAffineWarp, CDialogEx)

CDlgAffineWarp::CDlgAffineWarp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAffineWarp::IDD, pParent)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_interWays(INTER_LINEAR)
{

}

CDlgAffineWarp::~CDlgAffineWarp()
{
}

void CDlgAffineWarp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
}


BEGIN_MESSAGE_MAP(CDlgAffineWarp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgAffineWarp::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgAffineWarp::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgAffineWarp::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgAffineWarp::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgAffineWarp::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CDlgAffineWarp ��Ϣ�������



BOOL CDlgAffineWarp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rt;
	GetDlgItem(IDC_SHOW)->GetClientRect(&rt);
	double ViewScale = 1;
	if((image.Width() > rt.Width())||(image.Height() > rt.Height()))
	{
		float widthCoff = (float)rt.Width() / image.Width();
		float heightCoff = (float)rt.Height() / image.Height();
		ViewScale = widthCoff < heightCoff ? widthCoff:heightCoff;
	}
	int imageX = (int)(image.Width()*ViewScale);
	int imageY = (int)(image.Height()*ViewScale);
	CRect rect((rt.Width()-imageX)/2,(rt.Width()+imageX)/2,(rt.Height()-imageY)/2,(rt.Height()+imageY)/2);
	CDC* pDC = GetDlgItem( IDC_SHOW )->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	image.DrawToHDC(hDC,&rect);

	m_comboBox.AddString(_T("LINEAR"));
	m_comboBox.AddString(_T("NEAREST"));
	m_comboBox.AddString(_T("AREA"));
	m_comboBox.AddString(_T("CUBIC"));
	m_comboBox.AddString(_T("LANCZOS4"));
	m_comboBox.AddString(_T("FILL_OUTLIERS"));
	m_comboBox.AddString(_T("INVERSE_MAP"));
	m_comboBox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgAffineWarp::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string == _T("LINEAR"))	m_interWays = INTER_LINEAR;
	else if(string == _T("NEAREST"))	m_interWays = INTER_NEAREST;
	else if(string == _T("NEAREST"))	m_interWays = INTER_NEAREST;
	else if(string == _T("AREA"))	m_interWays = INTER_AREA;
	else if(string == _T("CUBIC"))	m_interWays = INTER_CUBIC;
	else if(string == _T("FILL_OUTLIERS"))	m_interWays = CV_WARP_FILL_OUTLIERS;
	else if(string == _T("INVERSE_MAP"))	m_interWays = CV_WARP_INVERSE_MAP;
	else m_interWays = INTER_LINEAR;
	UpdateData(false);
}


void CDlgAffineWarp::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CColorDialog colorDlg;
	if(colorDlg.DoModal() == IDOK)
	{
		COLORREF color = colorDlg.GetColor();
		m_colorR = GetRValue(color);
		m_colorG = GetGValue(color);
		m_colorB = GetBValue(color);
	}
	UpdateData(false);
}

void CDlgAffineWarp::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_colorR<0)	m_colorR = 0;
	else if(m_colorR>255)	m_colorR = 255;
	else return;
	UpdateData(false);
}


void CDlgAffineWarp::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_colorG<0)	m_colorG = 0;
	else if(m_colorG>255)	m_colorG = 255;
	else return;
	UpdateData(false);
}


void CDlgAffineWarp::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_colorB<0)	m_colorB = 0;
	else if(m_colorB>255)	m_colorB = 255;
	else return;
	UpdateData(false);
}
