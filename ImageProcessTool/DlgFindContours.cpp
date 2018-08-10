// DlgFindContours.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgFindContours.h"
#include "afxdialogex.h"


// CDlgFindContours 对话框

IMPLEMENT_DYNAMIC(CDlgFindContours, CDialogEx)

CDlgFindContours::CDlgFindContours(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFindContours::IDD, pParent)
	, m_mode(CV_RETR_EXTERNAL)
	, m_method(CV_CHAIN_APPROX_NONE)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_bRandColor(true)
{

}

CDlgFindContours::~CDlgFindContours()
{
}

void CDlgFindContours::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_RADIO5, m_radio2);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Control(pDX, IDC_CHECK1, m_check);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_BUTTON1, m_button);
}


BEGIN_MESSAGE_MAP(CDlgFindContours, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgFindContours::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgFindContours::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgFindContours::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgFindContours::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO5, &CDlgFindContours::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO6, &CDlgFindContours::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO7, &CDlgFindContours::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO8, &CDlgFindContours::OnBnClickedRadio2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFindContours::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgFindContours::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgFindContours::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFindContours::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgFindContours::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CDlgFindContours 消息处理程序


BOOL CDlgFindContours::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	m_radio1.SetCheck(1);
	m_radio2.SetCheck(1);
	m_check.SetCheck(1);
	m_edit1.SetReadOnly(true);
	m_edit2.SetReadOnly(true);
	m_edit3.SetReadOnly(true);
	m_button.EnableWindow(false);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgFindContours::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int sel = m_radio1.GetCheck();
	if(sel == 1)	m_mode = CV_RETR_EXTERNAL;
	else if(sel == 2)	m_mode = CV_RETR_LIST;
	else if(sel == 3)	m_mode = CV_RETR_CCOMP;
	else if(sel == 4)	m_mode = CV_RETR_TREE;
	UpdateData(false);
}


void CDlgFindContours::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int sel = m_radio2.GetCheck();
	if(sel == 1)	m_method = CV_CHAIN_APPROX_NONE;
	else if(sel == 2)	m_method = CV_CHAIN_APPROX_SIMPLE;
	else if(sel == 3)	m_method = CV_CHAIN_APPROX_TC89_L1;
	else if(sel == 4)	m_method = CV_CHAIN_APPROX_TC89_KCOS;
	UpdateData(false);
}

void CDlgFindContours::OnEnChangeEdit1()
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


void CDlgFindContours::OnEnChangeEdit2()
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


void CDlgFindContours::OnEnChangeEdit3()
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


void CDlgFindContours::OnBnClickedButton1()
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


void CDlgFindContours::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_check.GetCheck())
	{
		m_edit1.SetReadOnly(true);
		m_edit2.SetReadOnly(true);
		m_edit3.SetReadOnly(true);
		m_button.EnableWindow(false);
		m_bRandColor = true;
	}
	else
	{
		m_edit1.SetReadOnly(false);
		m_edit2.SetReadOnly(false);
		m_edit3.SetReadOnly(false);
		m_button.EnableWindow(true);
		m_bRandColor = false;
	}
	UpdateData(false);
}
