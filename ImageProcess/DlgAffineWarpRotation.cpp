// DlgAffineWarpRotation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgAffineWarpRotation.h"
#include "afxdialogex.h"


// CDlgAffineWarpRotation �Ի���

IMPLEMENT_DYNAMIC(CDlgAffineWarpRotation, CDialogEx)

CDlgAffineWarpRotation::CDlgAffineWarpRotation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAffineWarpRotation::IDD, pParent)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_angle(0)
	, m_scale(1)
	, m_interWays(INTER_LINEAR)
{

}

CDlgAffineWarpRotation::~CDlgAffineWarpRotation()
{
}

void CDlgAffineWarpRotation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Text(pDX, IDC_EDIT5, m_angle);
	DDV_MinMaxInt(pDX, m_angle, -360, 360);
	DDX_Text(pDX, IDC_EDIT4, m_scale);
	DDV_MinMaxDouble(pDX, m_scale, 0, 10);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
}


BEGIN_MESSAGE_MAP(CDlgAffineWarpRotation, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgAffineWarpRotation::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgAffineWarpRotation::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgAffineWarpRotation::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT6, &CDlgAffineWarpRotation::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgAffineWarpRotation::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgAffineWarpRotation::OnEnChangeEdit4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgAffineWarpRotation::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgAffineWarpRotation::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgAffineWarpRotation ��Ϣ�������


void CDlgAffineWarpRotation::OnEnChangeEdit1()
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


void CDlgAffineWarpRotation::OnEnChangeEdit2()
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


void CDlgAffineWarpRotation::OnEnChangeEdit3()
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


void CDlgAffineWarpRotation::OnEnChangeEdit6()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgAffineWarpRotation::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_angle<-360)	m_angle = -360;
	else if(m_angle>360)	m_angle = 360;
	else return;
	UpdateData(false);
}


void CDlgAffineWarpRotation::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_scale<0)	m_scale = 0;
	else if(m_scale>10)	m_scale = 10;
	else return;
	UpdateData(false);
}


void CDlgAffineWarpRotation::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string == _T("NEAREST"))	m_interWays = INTER_NEAREST;
	else if(string == _T("LINEAR"))	m_interWays = INTER_LINEAR;
	else if(string == _T("CUBIC"))	m_interWays = INTER_CUBIC;
	else if(string == _T("AREA"))	m_interWays = INTER_AREA;
	else if(string == _T("LANCZOS4"))	m_interWays = INTER_LANCZOS4;
	else if(string == _T("FILL_OUTLIERS"))	m_interWays = CV_WARP_FILL_OUTLIERS;
	else if(string == _T("INVERSE_MAP"))	m_interWays = CV_WARP_INVERSE_MAP;
	else m_interWays = INTER_LINEAR;
	UpdateData(false);
}


void CDlgAffineWarpRotation::OnBnClickedButton1()
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


BOOL CDlgAffineWarpRotation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��m_comboBox.AddString(_T("LINEAR"));
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
