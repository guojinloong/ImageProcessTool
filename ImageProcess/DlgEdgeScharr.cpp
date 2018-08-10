// DlgEdgeScharr.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgEdgeScharr.h"
#include "afxdialogex.h"

extern Mat srcImg;

// CDlgEdgeScharr 对话框

IMPLEMENT_DYNAMIC(CDlgEdgeScharr, CDialogEx)

CDlgEdgeScharr::CDlgEdgeScharr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEdgeScharr::IDD, pParent)
	, m_scale(1)
	, m_delta(0)
	, m_depth(CV_8U)
	, m_dX(1)
	, m_dY(1)
{

}

CDlgEdgeScharr::~CDlgEdgeScharr()
{
}

void CDlgEdgeScharr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ddepth);
	DDX_Control(pDX, IDC_COMBO2, m_dx);
	DDX_Control(pDX, IDC_COMBO3, m_dy);
	DDX_Text(pDX, IDC_EDIT1, m_scale);
	DDV_MinMaxDouble(pDX, m_scale, 0, 10);
	DDX_Text(pDX, IDC_EDIT2, m_delta);
	DDV_MinMaxDouble(pDX, m_delta, -10, 10);
}


BEGIN_MESSAGE_MAP(CDlgEdgeScharr, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgEdgeScharr::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgEdgeScharr::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgEdgeScharr::OnCbnSelchangeCombo3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgEdgeScharr::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgEdgeScharr::OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgEdgeScharr::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgEdgeScharr::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CDlgEdgeScharr 消息处理程序


void CDlgEdgeScharr::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_ddepth.GetCurSel();
	CString string;
	m_ddepth.GetLBText(sel,string);
	if(string == _T("CV_8U"))	m_depth = CV_8U;
	else if(string == _T("CV_16U"))	m_depth = CV_16U;
	else if(string == _T("CV_16S"))	m_depth = CV_16S;
	else if(string == _T("CV_32F"))	m_depth = CV_32F;
	else if(string == _T("CV_64F"))	m_depth = CV_64F;
}


void CDlgEdgeScharr::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_dx.GetCurSel();
	CString string;
	m_dx.GetLBText(sel,string);
	if(string == _T("零阶"))	m_dX = 0;
	else if(string == _T("一阶"))	m_dX = 1;
	else if(string == _T("二阶"))	m_dX = 2;
	if((m_dX == 0)&&(m_dY == 0))
	{
		MessageBox(_T("X和Y方向差分阶数不能同时为零！"),_T("警告"),MB_ICONWARNING);
		m_dY = 1;
		m_dy.SetCurSel(1);
	}
}


void CDlgEdgeScharr::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_dy.GetCurSel();
	CString string;
	m_dy.GetLBText(sel,string);
	if(string == _T("零阶"))	m_dY = 0;
	else if(string == _T("一阶"))	m_dY = 1;
	else if(string == _T("二阶"))	m_dY = 2;
	if((m_dX == 0)&&(m_dY == 0))
	{
		MessageBox(_T("X和Y方向差分阶数不能同时为零！"),_T("警告"),MB_ICONWARNING);
		m_dX = 1;
		m_dx.SetCurSel(1);
	}
}


void CDlgEdgeScharr::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_scale>0)	m_scale -= 0.1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_scale<10)	m_scale += 0.1;
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgEdgeScharr::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_delta>-10)	m_delta -= 0.1;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_delta<10)	m_delta += 0.1;
	}
	UpdateData(false);
	*pResult = 0;
}


BOOL CDlgEdgeScharr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(srcImg.depth() == CV_8U)
	{
		m_ddepth.AddString(_T("CV_8U"));
		m_ddepth.AddString(_T("CV_16S"));
		m_ddepth.AddString(_T("CV_32F"));
		m_ddepth.AddString(_T("CV_64F"));
	}
	else if((srcImg.depth() == CV_16U)||(srcImg.depth() == CV_16S))
	{
		m_ddepth.AddString(_T("CV_16U"));
		m_ddepth.AddString(_T("CV_16S"));
		m_ddepth.AddString(_T("CV_32F"));
		m_ddepth.AddString(_T("CV_64F"));
	}
	else if(srcImg.depth() == CV_32F)
	{
		m_ddepth.AddString(_T("CV_32F"));
		m_ddepth.AddString(_T("CV_64F"));
	}
	else if(srcImg.depth() == CV_64F)
	{
		m_ddepth.AddString(_T("CV_64F"));
	}
	m_ddepth.SetCurSel(0);

	m_dx.AddString(_T("零阶"));
	m_dx.AddString(_T("一阶"));
	m_dx.AddString(_T("二阶"));
	m_dx.SetCurSel(1);

	
	m_dy.AddString(_T("零阶"));
	m_dy.AddString(_T("一阶"));
	m_dy.AddString(_T("二阶"));
	m_dy.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgEdgeScharr::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
}


void CDlgEdgeScharr::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
}
