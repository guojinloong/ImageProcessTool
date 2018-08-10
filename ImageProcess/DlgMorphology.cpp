// DlgMorphology.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgMorphology.h"
#include "afxdialogex.h"


// CDlgMorphology 对话框

IMPLEMENT_DYNAMIC(CDlgMorphology, CDialogEx)

CDlgMorphology::CDlgMorphology(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMorphology::IDD, pParent)
	, shape(MORPH_RECT)
	, m_string(_T(""))
	, m_structElementSize(0)
{

}

CDlgMorphology::~CDlgMorphology()
{
}

void CDlgMorphology::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_elementShape);
	DDX_CBString(pDX, IDC_COMBO1, m_string);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Text(pDX, IDC_EDIT1, m_structElementSize);
}


BEGIN_MESSAGE_MAP(CDlgMorphology, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgMorphology::OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgMorphology::OnNMCustomdrawSlider1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgMorphology::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgMorphology::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CDlgMorphology 消息处理程序


BOOL CDlgMorphology::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_elementShape.AddString(_T("MORPH_RECT"));
	m_elementShape.AddString(_T("MORPH_CROSS"));
	m_elementShape.AddString(_T("MORPH_ELLIPSE"));

	m_elementShape.SetCurSel(0);
	m_slider.SetRange(0,20);
	m_slider.SetTicFreq(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgMorphology::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_elementShape.GetCurSel();
	CString m_string;
	m_elementShape.GetLBText(sel,m_string);
	if(m_string = _T("MORPH_RECT"))	shape = MORPH_RECT;
	else if(m_string = _T("MORPH_CROSS"))	shape = MORPH_CROSS;
	else if(m_string = _T("MORPH_ELLIPSE"))	shape = MORPH_ELLIPSE;
	else shape = MORPH_RECT;
}


void CDlgMorphology::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_structElementSize = m_slider.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgMorphology::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_slider.SetPos(m_structElementSize);
	UpdateData(false);
}



void CDlgMorphology::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_structElementSize>1)	m_structElementSize--;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_structElementSize<20)	m_structElementSize++;
	}
	m_slider.SetPos(m_structElementSize);
	CString ss;
	ss.Format(_T("%d"),m_structElementSize);
	GetDlgItem(IDC_EDIT1)->SetWindowText(ss);
	UpdateData(false);
	*pResult = 0;
}