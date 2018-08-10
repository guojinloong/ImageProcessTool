// DlgFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgFilter.h"
#include "afxdialogex.h"

extern Mat srcImg,tempImg,dstImg;

// CDlgFilter 对话框

IMPLEMENT_DYNAMIC(CDlgFilter, CDialogEx)

CDlgFilter::CDlgFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilter::IDD, pParent)
	, m_filterValue(0)
{

}

CDlgFilter::~CDlgFilter()
{
}

void CDlgFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_filterValue);
	DDV_MinMaxInt(pDX, m_filterValue, 0, 40);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CDlgFilter, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgFilter::OnDeltaposSpin1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgFilter::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFilter::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CDlgFilter 消息处理程序


void CDlgFilter::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_filterValue>1)	m_filterValue--;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_filterValue<40)	m_filterValue++;
	}
	m_slider.SetPos(m_filterValue);
	//CString ss;
	//ss.Format(_T("%d"),m_filterValue);
	//GetDlgItem(IDC_EDIT1)->SetWindowText(ss);
	UpdateData(false);
	//boxFilter(srcImg,tempImg,-1,Size(m_filterValue+1,m_filterValue+1));
	*pResult = 0;
}


void CDlgFilter::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_filterValue = m_slider.GetPos();
	UpdateData(false);
	*pResult = 0;
}


BOOL CDlgFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_slider.SetRange(0,40);
	m_slider.SetTicFreq(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgFilter::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_slider.SetPos(m_filterValue);
	UpdateData(false);
}