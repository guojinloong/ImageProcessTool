// DlgPoly.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgPoly.h"
#include "afxdialogex.h"


// CDlgPoly �Ի���

IMPLEMENT_DYNAMIC(CDlgPoly, CDialogEx)

CDlgPoly::CDlgPoly(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPoly::IDD, pParent)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_bRandColor(TRUE)
	, m_select1(FALSE)
	, m_select2(FALSE)
	, m_select3(FALSE)
	, m_select4(FALSE)
	, m_select5(TRUE)
{

}

CDlgPoly::~CDlgPoly()
{
}

void CDlgPoly::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Check(pDX, IDC_CHECK1, m_bRandColor);
	DDX_Check(pDX, IDC_CHECK2, m_select1);
	DDX_Check(pDX, IDC_CHECK3, m_select2);
	DDX_Check(pDX, IDC_CHECK4, m_select3);
	DDX_Check(pDX, IDC_CHECK5, m_select4);
	DDX_Check(pDX, IDC_CHECK6, m_select5);
}


BEGIN_MESSAGE_MAP(CDlgPoly, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgPoly::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgPoly::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgPoly::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgPoly::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgPoly::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgPoly ��Ϣ�������

void CDlgPoly::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_bRandColor)
	{
		m_edit1.SetReadOnly(true);
		m_edit2.SetReadOnly(true);
		m_edit3.SetReadOnly(true);
		m_button.EnableWindow(false);
	}
	else
	{
		m_edit1.SetReadOnly(false);
		m_edit2.SetReadOnly(false);
		m_edit3.SetReadOnly(false);
		m_button.EnableWindow(true);
	}
	UpdateData(false);
}


void CDlgPoly::OnEnChangeEdit1()
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


void CDlgPoly::OnEnChangeEdit2()
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


void CDlgPoly::OnEnChangeEdit3()
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


void CDlgPoly::OnBnClickedButton1()
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


BOOL CDlgPoly::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(true);
	m_edit1.SetReadOnly(true);
	m_edit2.SetReadOnly(true);
	m_edit3.SetReadOnly(true);
	m_button.EnableWindow(false);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
