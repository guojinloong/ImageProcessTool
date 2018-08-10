// DlgPoly.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgPoly.h"
#include "afxdialogex.h"


// CDlgPoly 对话框

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


// CDlgPoly 消息处理程序

void CDlgPoly::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorR<0)	m_colorR = 0;
	else if(m_colorR>255)	m_colorR = 255;
	UpdateData(false);
}


void CDlgPoly::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorG<0)	m_colorG = 0;
	else if(m_colorG>255)	m_colorG = 255;
	UpdateData(false);
}


void CDlgPoly::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorB<0)	m_colorB = 0;
	else if(m_colorB>255)	m_colorB = 255;
	UpdateData(false);
}


void CDlgPoly::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	m_edit1.SetReadOnly(true);
	m_edit2.SetReadOnly(true);
	m_edit3.SetReadOnly(true);
	m_button.EnableWindow(false);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
