// DlgFloodFill.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgFloodFill.h"
#include "afxdialogex.h"


// CDlgFloodFill 对话框

IMPLEMENT_DYNAMIC(CDlgFloodFill, CDialogEx)

CDlgFloodFill::CDlgFloodFill(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFloodFill::IDD, pParent)
	, m_upDiff(10)
	, m_lowDiff(10)
	, m_maskValue(255)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_fillMode(FLOODFILL_FIXED_RANGE)
	, m_connectivity(4)
	, m_bMask(false)
{

}

CDlgFloodFill::~CDlgFloodFill()
{
}

void CDlgFloodFill::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT6, m_upDiff);
	DDV_MinMaxInt(pDX, m_upDiff, 0, 20);
	DDX_Text(pDX, IDC_EDIT5, m_lowDiff);
	DDV_MinMaxInt(pDX, m_lowDiff, 0, 20);
	DDX_Text(pDX, IDC_EDIT4, m_maskValue);
	DDV_MinMaxInt(pDX, m_maskValue, 0, 255);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Control(pDX, IDC_CHECK1, m_maskCheck);
	DDX_Control(pDX, IDC_CHECK2, m_fixedRange);
	DDX_Control(pDX, IDC_EDIT4, m_maskEdit);
	DDX_Control(pDX, IDC_CHECK3, m_connectivityCheck);
}


BEGIN_MESSAGE_MAP(CDlgFloodFill, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT6, &CDlgFloodFill::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgFloodFill::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgFloodFill::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgFloodFill::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgFloodFill::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFloodFill::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgFloodFill::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgFloodFill::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFloodFill::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK3, &CDlgFloodFill::OnBnClickedCheck3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgFloodFill::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgFloodFill::OnDeltaposSpin2)
END_MESSAGE_MAP()


// CDlgFloodFill 消息处理程序


void CDlgFloodFill::OnEnChangeEdit6()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_upDiff<0)	m_upDiff = 0;
	else if(m_upDiff>20)	m_upDiff = 20;
	else return;
	UpdateData(false);
}


void CDlgFloodFill::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_lowDiff<0)	m_lowDiff = 0;
	else if(m_lowDiff>20)	m_lowDiff = 20;
	else return;
	UpdateData(false);
}


void CDlgFloodFill::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_maskValue<0)	m_maskValue = 0;
	else if(m_maskValue>255)	m_maskValue = 255;
	else return;
	UpdateData(false);
}


void CDlgFloodFill::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_maskCheck.GetCheck())
	{
		m_maskEdit.SetReadOnly(false);
		m_bMask = true;
	}
	else
	{
		m_maskEdit.SetReadOnly(true);
		m_bMask = false;
	}
	UpdateData(false);
}


void CDlgFloodFill::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_fixedRange.GetCheck())	m_fillMode = FLOODFILL_FIXED_RANGE;
	else m_fillMode = 0;
	UpdateData(false);
}


void CDlgFloodFill::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_connectivityCheck.GetCheck())	m_connectivity = 8;
	else m_connectivity = 4;
	UpdateData(false);
}



void CDlgFloodFill::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorR<0)	m_colorR = 0;
	else if(m_colorR>255)	m_colorR = 255;
	else return;
	UpdateData(false);
}


void CDlgFloodFill::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorG<0)	m_colorG = 0;
	else if(m_colorG>255)	m_colorG = 255;
	else return;
	UpdateData(false);
}


void CDlgFloodFill::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorB<0)	m_colorB = 0;
	else if(m_colorB>255)	m_colorB = 255;
	else return;
	UpdateData(false);
}


void CDlgFloodFill::OnBnClickedButton1()
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


BOOL CDlgFloodFill::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	
	m_maskCheck.SetCheck(FALSE);
	m_fixedRange.SetCheck(TRUE);
	m_connectivityCheck.SetCheck(FALSE);
	m_maskEdit.SetReadOnly(true);

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgFloodFill::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if((pNMUpDown->iDelta == 1)&&(m_upDiff>0))	m_upDiff--;
	else if((pNMUpDown->iDelta == -1)&&(m_upDiff<20))	m_upDiff++;
	*pResult = 0;
	UpdateData(false);
}


void CDlgFloodFill::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if((pNMUpDown->iDelta == 1)&&(m_lowDiff>0))	m_lowDiff--;
	else if((pNMUpDown->iDelta == -1)&&(m_lowDiff<20))	m_lowDiff++;
	*pResult = 0;
	UpdateData(false);
}
