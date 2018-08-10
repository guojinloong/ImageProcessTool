// DlgHoughLines.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgHoughLines.h"
#include "afxdialogex.h"


// CDlgHoughLines 对话框

IMPLEMENT_DYNAMIC(CDlgHoughLines, CDialogEx)

CDlgHoughLines::CDlgHoughLines(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHoughLines::IDD, pParent)
	, m_threshold(300)
	, m_rho(1)
	, m_theta(1)
	, m_srn(0)
	, m_stn(0)
{

}

CDlgHoughLines::~CDlgHoughLines()
{
}

void CDlgHoughLines::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_threshold);
	DDV_MinMaxInt(pDX, m_threshold, 300, 1000);
	DDX_Text(pDX, IDC_EDIT2, m_rho);
	DDV_MinMaxDouble(pDX, m_rho, 1, 10);
	DDX_Text(pDX, IDC_EDIT3, m_theta);
	DDV_MinMaxDouble(pDX, m_theta, 1, 360);
	DDX_Text(pDX, IDC_EDIT4, m_srn);
	DDV_MinMaxDouble(pDX, m_srn, 0, 10);
	DDX_Text(pDX, IDC_EDIT5, m_stn);
	DDV_MinMaxDouble(pDX, m_stn, 0, 10);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CDlgHoughLines, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgHoughLines::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgHoughLines::OnEnChangeEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgHoughLines::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgHoughLines::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDlgHoughLines::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CDlgHoughLines::OnDeltaposSpin4)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgHoughLines::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgHoughLines::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgHoughLines::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgHoughLines::OnEnChangeEdit5)
END_MESSAGE_MAP()


// CDlgHoughLines 消息处理程序


BOOL CDlgHoughLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_slider.SetRange(300,1000);
	m_slider.SetTicFreq(1);
	m_slider.SetPos(m_threshold);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgHoughLines::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_threshold = m_slider.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgHoughLines::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_threshold < 300)	m_threshold = 300;
	else if(m_threshold > 1000)	m_threshold = 1000;
	else m_slider.SetPos(m_threshold);
	UpdateData(false);
}


void CDlgHoughLines::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_rho < 1)	m_rho = 1;
	else if(m_rho > 10)	m_rho = 10;
	UpdateData(false);
}


void CDlgHoughLines::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_theta < 1)	m_theta = 1;
	else if(m_theta > 360)	m_theta = 360;
	UpdateData(false);
}


void CDlgHoughLines::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_srn < 0)	m_srn = 0;
	else if(m_srn > 10)	m_srn = 10;
	UpdateData(false);
}


void CDlgHoughLines::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_stn < 0)	m_stn = 0;
	else if(m_stn > 10)	m_stn = 10;
	UpdateData(false);
}



void CDlgHoughLines::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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


void CDlgHoughLines::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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


void CDlgHoughLines::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_srn>0)	m_srn -= 0.1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_srn<10)	m_srn += 0.1;
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgHoughLines::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_stn>0)	m_stn -= 0.1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_stn<10)	m_stn += 0.1;
	}
	UpdateData(false);
	*pResult = 0;
}