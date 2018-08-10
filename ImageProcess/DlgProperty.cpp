// DlgProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgProperty.h"
#include "afxdialogex.h"

extern Mat srcImg;

// CDlgProperty 对话框

IMPLEMENT_DYNAMIC(CDlgProperty, CDialogEx)

CDlgProperty::CDlgProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProperty::IDD, pParent)
	, m_colorMode(0)
	, m_grayMode(0)
	, m_depth(srcImg.depth())
	, m_channels(srcImg.channels())
	, m_ndims(srcImg.dims)
{

}

CDlgProperty::~CDlgProperty()
{
}

void CDlgProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox1);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox2);
	DDX_Control(pDX, IDC_COMBO3, m_comboBox3);
	DDX_Control(pDX, IDC_COMBO4, m_comboBox4);
	DDX_Control(pDX, IDC_COMBO5, m_comboBox5);
}


BEGIN_MESSAGE_MAP(CDlgProperty, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgProperty::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgProperty::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgProperty::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgProperty::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CDlgProperty::OnCbnSelchangeCombo5)
END_MESSAGE_MAP()


// CDlgProperty 消息处理程序


BOOL CDlgProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	CString size;
	size.Format(_T("%d x %d"),srcImg.cols,srcImg.rows);
	GetDlgItem(IDC_EDIT_SIZE)->SetWindowText(size);

	m_comboBox1.AddString(_T("RGB"));
	m_comboBox1.AddString(_T("RGBA"));
	m_comboBox1.AddString(_T("HSV"));
	m_comboBox1.AddString(_T("HLS"));
	m_comboBox1.AddString(_T("YCrCb"));
	m_comboBox1.AddString(_T("XYZ"));
	m_comboBox1.SetCurSel(0);
	srcImg.type();

	m_comboBox2.AddString(_T("真彩色"));
	m_comboBox2.AddString(_T("灰度"));
	m_comboBox2.AddString(_T("二值"));
	m_comboBox2.SetCurSel(0);
	srcImg.channels();

	m_comboBox3.AddString(_T("CV_8U"));
	m_comboBox3.AddString(_T("CV_8S"));
	m_comboBox3.AddString(_T("CV_16U"));
	m_comboBox3.AddString(_T("CV_16S"));
	m_comboBox3.AddString(_T("CV_32S"));
	m_comboBox3.AddString(_T("CV_32F"));
	m_comboBox3.AddString(_T("CV_64F"));
	m_comboBox3.SetCurSel(m_depth);
	
	m_comboBox4.AddString(_T("1"));
	m_comboBox4.AddString(_T("2"));
	m_comboBox4.AddString(_T("3"));
	m_comboBox4.AddString(_T("4"));
	m_comboBox4.SetCurSel(m_channels-1);

	m_comboBox5.AddString(_T("二维"));
	m_comboBox5.AddString(_T("三维"));
	m_comboBox5.SetCurSel(m_ndims-2);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgProperty::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox1.GetCurSel();
	CString string;
	m_comboBox1.GetLBText(sel,string);
	if(string == _T("RGB"))	m_colorMode = 1;
	else if(string == _T("RGBA"))	m_colorMode = 2;
	else if(string == _T("HSV"))	m_colorMode = 3;
	else if(string == _T("HLS"))	m_colorMode = 4;
	else if(string == _T("Luv"))	m_colorMode = 5;
	else if(string == _T("YCrCb"))	m_colorMode = 6;
	else if(string == _T("XYZ"))	m_colorMode = 7;
}


void CDlgProperty::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox2.GetCurSel();
	CString string;
	m_comboBox2.GetLBText(sel,string);
	if(string == _T("真彩色"))	m_grayMode = 1;
	else if(string == _T("灰度"))	m_grayMode = 2;
	else if(string == _T("二值"))	m_grayMode = 3;
}


void CDlgProperty::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox3.GetCurSel();
	CString string;
	m_comboBox3.GetLBText(sel,string);
	if(string == _T("CV_8U"))	m_depth = CV_8U;
	else if(string == _T("CV_8S"))	m_depth = CV_8S;
	else if(string == _T("CV_16U"))	m_depth = CV_16U;
	else if(string == _T("CV_16S"))	m_depth = CV_16S;
	else if(string == _T("CV_32S"))	m_depth = CV_32S;
	else if(string == _T("CV_32F"))	m_depth = CV_32F;
	else if(string == _T("CV_64F"))	m_depth = CV_64F;
}


void CDlgProperty::OnCbnSelchangeCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox4.GetCurSel();
	CString string;
	m_comboBox4.GetLBText(sel,string);
	if(string == _T("1"))	m_channels = 1;
	else if(string == _T("2"))	m_channels = 2;
	else if(string == _T("3"))	m_channels = 3;
	else if(string == _T("4"))	m_channels = 4;
}


void CDlgProperty::OnCbnSelchangeCombo5()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox5.GetCurSel();
	CString string;
	m_comboBox5.GetLBText(sel,string);
	if(string == _T("二维"))	m_ndims = 2;
	else if(string == _T("三维"))	m_ndims = 3;
}
