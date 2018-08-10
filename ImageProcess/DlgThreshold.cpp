// DlgThreshold.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgThreshold.h"
#include "afxdialogex.h"


// CDlgThreshold 对话框

IMPLEMENT_DYNAMIC(CDlgThreshold, CDialogEx)

CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgThreshold::IDD, pParent)
	, m_threshold(0)
	, m_maxValue(255)
	, m_type(THRESH_BINARY)
{

}

CDlgThreshold::~CDlgThreshold()
{
}

void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_threshold);
	DDV_MinMaxInt(pDX, m_threshold, 0, 254);
	DDX_Text(pDX, IDC_EDIT2, m_maxValue);
	DDV_MinMaxInt(pDX, m_maxValue, 1, 255);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_COMBO1, m_thresholdType);
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgThreshold::OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgThreshold::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDlgThreshold::OnNMCustomdrawSlider2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgThreshold::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgThreshold::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CDlgThreshold 消息处理程序



BOOL CDlgThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	m_thresholdType.AddString(_T("BINARY"));
	m_thresholdType.AddString(_T("BINARY_INV"));
	m_thresholdType.AddString(_T("TRUNC"));
	m_thresholdType.AddString(_T("TOZERO"));
	m_thresholdType.AddString(_T("TOZERO_INV"));
	m_thresholdType.SetCurSel(0);

	m_slider1.SetRange(0,254);
	m_slider1.SetTicFreq(1);
	m_slider2.SetRange(1,255);
	m_slider2.SetTicFreq(1);
	m_slider1.SetPos(m_threshold);
	m_slider2.SetPos(m_maxValue);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgThreshold::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_thresholdType.GetCurSel();
	CString string;
	m_thresholdType.GetLBText(sel,string);
	if(string = _T("BINARY"))	m_type = THRESH_BINARY;
	else if(string = _T("BINARY_INV"))	m_type = THRESH_BINARY_INV;
	else if(string = _T("TRUNC"))	m_type = THRESH_TRUNC;
	else if(string = _T("TOZERO"))	m_type = THRESH_TOZERO;
	else if(string = _T("TOZERO_INV"))	m_type = THRESH_TOZERO_INV;
	else m_type = MORPH_RECT;
}


void CDlgThreshold::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_threshold<m_maxValue)	m_threshold = m_slider1.GetPos();
	if(m_threshold>m_maxValue)
	{
		m_maxValue = m_threshold+1;
		m_slider2.SetPos(m_maxValue);
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgThreshold::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_maxValue = m_slider2.GetPos();
	if(m_threshold>m_maxValue)
	{
		m_threshold = m_maxValue-1;
		m_slider1.SetPos(m_threshold);
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgThreshold::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_slider1.SetPos(m_threshold);
	if(m_threshold>m_maxValue)
	{
		m_maxValue = m_threshold+1;
		m_slider2.SetPos(m_maxValue);
	}
	UpdateData(false);
}


void CDlgThreshold::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_threshold<m_maxValue)
	{
		m_threshold = m_maxValue-1;
		m_slider1.SetPos(m_threshold);
	}
	m_slider2.SetPos(m_maxValue);
	UpdateData(false);
}