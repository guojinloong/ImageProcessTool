// DlgEdgeCanny.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgEdgeCanny.h"
#include "afxdialogex.h"


// CDlgEdgeCanny 对话框

IMPLEMENT_DYNAMIC(CDlgEdgeCanny, CDialogEx)

CDlgEdgeCanny::CDlgEdgeCanny(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEdgeCanny::IDD, pParent)
	, m_cannyThreshold1(0)
	, m_cannyThreshold2(255)
	, m_type(3)
{

}

CDlgEdgeCanny::~CDlgEdgeCanny()
{
}

void CDlgEdgeCanny::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_apertureSize);
	DDX_Text(pDX, IDC_EDIT1, m_cannyThreshold1);
	DDV_MinMaxInt(pDX, m_cannyThreshold1, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_cannyThreshold2);
	DDV_MinMaxInt(pDX, m_cannyThreshold2, 0, 255);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
}


BEGIN_MESSAGE_MAP(CDlgEdgeCanny, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgEdgeCanny::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgEdgeCanny::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgEdgeCanny::OnEnChangeEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgEdgeCanny::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDlgEdgeCanny::OnNMCustomdrawSlider2)
END_MESSAGE_MAP()


// CDlgEdgeCanny 消息处理程序


void CDlgEdgeCanny::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_apertureSize.GetCurSel();
	CString string;
	m_apertureSize.GetLBText(sel,string);
	//if(string == _T("1"))	m_type = 1;
	//else 
	if(string == _T("3"))	m_type = 3;
	else if(string == _T("5"))	m_type = 5;
	else if(string == _T("7"))	m_type = 7;
}


void CDlgEdgeCanny::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_cannyThreshold1<0)	m_cannyThreshold1 = 0;
	else if(m_cannyThreshold1>255)	m_cannyThreshold1 = 255;
	m_slider1.SetPos(m_cannyThreshold1);
	UpdateData(false);
}


void CDlgEdgeCanny::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_cannyThreshold2<0)	m_cannyThreshold2 = 0;
	else if(m_cannyThreshold2>255)	m_cannyThreshold2 = 255;
	m_slider2.SetPos(m_cannyThreshold2);
	UpdateData(false);
}


void CDlgEdgeCanny::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_cannyThreshold1 = m_slider1.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgEdgeCanny::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_cannyThreshold2 = m_slider2.GetPos();
	UpdateData(false);
	*pResult = 0;
}


BOOL CDlgEdgeCanny::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_apertureSize.AddString(_T("1"));
	m_apertureSize.AddString(_T("3"));
	m_apertureSize.AddString(_T("5"));
	m_apertureSize.AddString(_T("7"));
	m_apertureSize.SetCurSel(0);

	m_slider1.SetRange(0,255);
	m_slider2.SetRange(0,255);
	m_slider1.SetTicFreq(1);
	m_slider2.SetTicFreq(1);
	m_slider1.SetPos(m_cannyThreshold1);
	m_slider2.SetPos(m_cannyThreshold2);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
