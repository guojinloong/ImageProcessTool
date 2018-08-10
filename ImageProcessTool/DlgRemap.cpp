// DlgRemap.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgRemap.h"
#include "afxdialogex.h"


// CDlgRemap �Ի���

IMPLEMENT_DYNAMIC(CDlgRemap, CDialogEx)

CDlgRemap::CDlgRemap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRemap::IDD, pParent)
	, m_remapWays(0)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_interWays(INTER_LINEAR)
{

}

CDlgRemap::~CDlgRemap()
{
}

void CDlgRemap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO3, m_remapWays);
	DDV_MinMaxInt(pDX, m_remapWays, 0, 3);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
}


BEGIN_MESSAGE_MAP(CDlgRemap, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgRemap::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgRemap::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgRemap::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRemap::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgRemap::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgRemap::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgRemap::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO5, &CDlgRemap::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO6, &CDlgRemap::OnBnClickedRadio)
END_MESSAGE_MAP()


// CDlgRemap ��Ϣ�������


void CDlgRemap::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_colorR<0)	m_colorR = 0;
	else if(m_colorR>255)	m_colorR = 255;
	UpdateData(false);
}


void CDlgRemap::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_colorG<0)	m_colorG = 0;
	else if(m_colorG>255)	m_colorG = 255;
	UpdateData(false);
}


void CDlgRemap::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_colorB<0)	m_colorB = 0;
	else if(m_colorB>255)	m_colorB = 255;
	UpdateData(false);
}


void CDlgRemap::OnBnClickedButton1()
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


void CDlgRemap::OnCbnSelchangeCombo1()
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


void CDlgRemap::OnBnClickedRadio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	UpdateData(false);
}


BOOL CDlgRemap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
