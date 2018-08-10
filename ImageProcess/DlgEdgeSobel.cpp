// DlgEdgeSobel.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgEdgeSobel.h"
#include "afxdialogex.h"

extern Mat srcImg;
// CDlgEdgeSobel 对话框

IMPLEMENT_DYNAMIC(CDlgEdgeSobel, CDialogEx)

CDlgEdgeSobel::CDlgEdgeSobel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEdgeSobel::IDD, pParent)
	, m_scale(1)
	, m_delta(0)
	, m_depth(CV_8U)
	, m_kSize(3)
	, m_dX(1)
	, m_dY(1)
{

}

CDlgEdgeSobel::~CDlgEdgeSobel()
{
}

void CDlgEdgeSobel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO4, m_ddepth);
	DDX_Control(pDX, IDC_COMBO3, m_apertureSize);
	DDX_Control(pDX, IDC_COMBO2, m_dx);
	DDX_Control(pDX, IDC_COMBO1, m_dy);
	DDX_Text(pDX, IDC_EDIT1, m_scale);
	DDV_MinMaxDouble(pDX, m_scale, 0, 10);
	DDX_Text(pDX, IDC_EDIT2, m_delta);
	DDV_MinMaxDouble(pDX, m_delta, -10, 10);
}


BEGIN_MESSAGE_MAP(CDlgEdgeSobel, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgEdgeSobel::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgEdgeSobel::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgEdgeSobel::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgEdgeSobel::OnCbnSelchangeCombo1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgEdgeSobel::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgEdgeSobel::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgEdgeSobel::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgEdgeSobel::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CDlgEdgeSobel 消息处理程序


void CDlgEdgeSobel::OnCbnSelchangeCombo4()
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


void CDlgEdgeSobel::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_apertureSize.GetCurSel();
	CString string;
	m_apertureSize.GetLBText(sel,string);
	if(string == _T("1"))	m_kSize = 1;
	else if(string == _T("3"))	m_kSize = 3;
	else if(string == _T("5"))	m_kSize = 5;
	else if(string == _T("7"))	m_kSize = 7;
}


void CDlgEdgeSobel::OnCbnSelchangeCombo2()
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


void CDlgEdgeSobel::OnCbnSelchangeCombo1()
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


void CDlgEdgeSobel::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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


void CDlgEdgeSobel::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
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


BOOL CDlgEdgeSobel::OnInitDialog()
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

	m_apertureSize.AddString(_T("1"));
	m_apertureSize.AddString(_T("3"));
	m_apertureSize.AddString(_T("5"));
	m_apertureSize.AddString(_T("7"));
	m_apertureSize.SetCurSel(1);

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


void CDlgEdgeSobel::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_scale < 0)	m_scale = 0;
	else if(m_scale > 10)	m_scale = 10;
	UpdateData(false);
}


void CDlgEdgeSobel::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_delta < -10)	m_delta = -10;
	else if(m_delta > 10)	m_delta = 10;
	UpdateData(false);
}
