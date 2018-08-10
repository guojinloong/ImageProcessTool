// DlgConvexHull.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgConvexHull.h"
#include "afxdialogex.h"


// CDlgConvexHull 对话框

IMPLEMENT_DYNAMIC(CDlgConvexHull, CDialogEx)

CDlgConvexHull::CDlgConvexHull(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConvexHull::IDD, pParent)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_path(_T(""))
	, m_direction(true)
	, m_select(TRUE)
	, m_bRandColor(TRUE)
{

}

CDlgConvexHull::~CDlgConvexHull()
{
}

void CDlgConvexHull::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Text(pDX, IDC_EDIT4, m_path);
	DDX_Control(pDX, IDC_EDIT4, m_edit);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Check(pDX, IDC_CHECK2, m_select);
	DDX_Check(pDX, IDC_CHECK1, m_bRandColor);
}


BEGIN_MESSAGE_MAP(CDlgConvexHull, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgConvexHull::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgConvexHull::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgConvexHull::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgConvexHull::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgConvexHull::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgConvexHull::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgConvexHull::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgConvexHull::OnBnClickedCheck2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgConvexHull::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlgConvexHull 消息处理程序


BOOL CDlgConvexHull::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	m_comboBox.AddString(_T("顺时针"));
	m_comboBox.AddString(_T("逆时针"));
	m_comboBox.SetCurSel(0);
	m_edit1.SetReadOnly(true);
	m_edit2.SetReadOnly(true);
	m_edit3.SetReadOnly(true);
	m_button1.EnableWindow(false);
	m_edit.SetReadOnly(true);
	m_button2.EnableWindow(false);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgConvexHull::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(m_colorR<0)	m_colorR = 0;
	else if(m_colorR>255)	m_colorR = 255;
	UpdateData(false);
}


void CDlgConvexHull::OnEnChangeEdit2()
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


void CDlgConvexHull::OnEnChangeEdit3()
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


void CDlgConvexHull::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_bRandColor)
	{
		m_edit1.SetReadOnly(true);
		m_edit2.SetReadOnly(true);
		m_edit3.SetReadOnly(true);
		m_button1.EnableWindow(false);
	}
	else
	{
		m_edit1.SetReadOnly(false);
		m_edit2.SetReadOnly(false);
		m_edit3.SetReadOnly(false);
		m_button1.EnableWindow(true);
	}
	UpdateData(false);
}


void CDlgConvexHull::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_select)
	{
		m_edit.SetReadOnly(true);
		m_button2.EnableWindow(false);
	}
	else
	{
		m_edit.SetReadOnly(false);
		m_button2.EnableWindow(true);
	}
	UpdateData(false);
}


void CDlgConvexHull::OnBnClickedButton1()
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


void CDlgConvexHull::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
}


void CDlgConvexHull::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	TCHAR szFilter[] = _T("XML文件(*.xml)|*.xml|YAML文件(*.yaml;*.yml)|*.yaml;*.yml|TXT文件(*.txt)|*.txt||");
	CFileDialog fileDlg(TRUE,_T("xml"),_T("contours"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(fileDlg.DoModal() == IDOK)
	{
		CString p = fileDlg.GetPathName();
		CStringA file(p.GetBuffer(0));
		p.ReleaseBuffer();
		m_path=file.GetBuffer(0);
		file.ReleaseBuffer();
	}
	UpdateData(false);
}


void CDlgConvexHull::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string == _T("顺时针"))	m_direction = true;
	else if(string == _T("逆时针"))	m_direction = false;
	UpdateData(false);
}
