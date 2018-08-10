// DlgSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgSet.h"
#include "afxdialogex.h"


// CDlgSet 对话框

IMPLEMENT_DYNAMIC(CDlgSet, CDialogEx)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSet::IDD, pParent)
	, m_thickness(1)
	, m_lineType(8)
	, m_shift(0)
{

}
//CDlgSet::CDlgSet(int thickness,int lineType,int shift)
//{
//	m_thickness = thickness;
//	m_lineType = lineType;
//	m_shift = shift;
//}

CDlgSet::~CDlgSet()
{
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox1);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox2);
	DDX_Control(pDX, IDC_COMBO3, m_comboBox3);
	//DDX_CBIndex(pDX, IDC_COMBO1, m_thickness);
	//DDV_MinMaxInt(pDX, m_thickness, -1, 20);
	//DDX_CBIndex(pDX, IDC_COMBO3, m_shift);
	//DDV_MinMaxInt(pDX, m_shift, 0, 5);
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgSet::OnCbnSelchangeCombobox1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgSet::OnCbnSelchangeCombobox2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgSet::OnCbnSelchangeCombobox3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgSet::OnBnClickedButton2)
	ON_STN_CLICKED(IDC_STATIC_COLOR_BACKGROUND, &CDlgSet::OnStnClickedStaticColorBackground)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSet::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgSet 消息处理程序


BOOL CDlgSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	//COMBOBOXEXITEM item;
	//item.mask = CBEIF_TEXT;
	//item.pszText = _T("填充");
	//item.iItem = 0;
	//m_comboBox1.InsertItem(&item);
	//item.pszText = _T(" 1");
	//item.iItem = 1;
	//m_comboBox1.InsertItem(&item);
	//item.pszText = _T(" 2");
	//item.iItem = 2;
	//m_comboBox1.InsertItem(&item);
	//item.pszText = _T(" 3");
	//item.iItem = 3;
	//m_comboBox1.InsertItem(&item);
	//item.pszText = _T(" 4");
	//item.iItem = 4;
	//m_comboBox1.InsertItem(&item);
	//item.pszText = _T(" 5");
	//item.iItem = 5;
	//m_comboBox1.InsertItem(&item);
	m_comboBox1.AddString(_T("填充"));
	m_comboBox1.AddString(_T("1"));
	m_comboBox1.AddString(_T("2"));
	m_comboBox1.AddString(_T("3"));
	m_comboBox1.AddString(_T("4"));
	m_comboBox1.AddString(_T("5"));
	m_comboBox1.AddString(_T("6"));
	m_comboBox1.AddString(_T("7"));
	m_comboBox1.AddString(_T("8"));
	m_comboBox1.AddString(_T("9"));
	m_comboBox1.AddString(_T("10"));
	m_comboBox1.AddString(_T("11"));
	m_comboBox1.AddString(_T("12"));
	m_comboBox1.AddString(_T("13"));
	m_comboBox1.AddString(_T("14"));
	m_comboBox1.AddString(_T("15"));
	m_comboBox1.AddString(_T("16"));
	m_comboBox1.AddString(_T("17"));
	m_comboBox1.AddString(_T("18"));
	m_comboBox1.AddString(_T("19"));
	m_comboBox1.AddString(_T("20"));
	m_comboBox1.SetCurSel(1);

	m_comboBox2.AddString(_T("8-连接"));
	m_comboBox2.AddString(_T("4-连接"));
	m_comboBox2.AddString(_T("反走样线条"));
	m_comboBox2.SetCurSel(0);

	m_comboBox3.AddString(_T("0"));
	m_comboBox3.AddString(_T("1"));
	m_comboBox3.AddString(_T("2"));
	m_comboBox3.AddString(_T("3"));
	m_comboBox3.AddString(_T("4"));
	m_comboBox3.AddString(_T("5"));
	m_comboBox3.SetCurSel(0);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSet::OnCbnSelchangeCombobox1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int sel = m_comboBox1.GetCurSel();
	CString string;
	m_comboBox1.GetLBText(sel,string);
	if(string == _T("填充"))	m_thickness = CV_FILLED;
	else if(string == _T("1"))	m_thickness = 1;
	else if(string == _T("2"))	m_thickness = 2;
	else if(string == _T("3"))	m_thickness = 3;
	else if(string == _T("4"))	m_thickness = 4;
	else if(string == _T("5"))	m_thickness = 5;
	else if(string == _T("6"))	m_thickness = 6;
	else if(string == _T("7"))	m_thickness = 7;
	else if(string == _T("8"))	m_thickness = 8;
	else if(string == _T("9"))	m_thickness = 9;
	else if(string == _T("10"))	m_thickness = 10;
	else if(string == _T("11"))	m_thickness = 11;
	else if(string == _T("12"))	m_thickness = 12;
	else if(string == _T("13"))	m_thickness = 13;
	else if(string == _T("14"))	m_thickness = 14;
	else if(string == _T("15"))	m_thickness = 15;
	else if(string == _T("16"))	m_thickness = 16;
	else if(string == _T("17"))	m_thickness = 17;
	else if(string == _T("18"))	m_thickness = 18;
	else if(string == _T("18"))	m_thickness = 19;
	else if(string == _T("20"))	m_thickness = 20;
	//if(m_thickness<1)	m_thickness = -1;
	//else if(m_thickness>20)	m_thickness = 20;
	UpdateData(false);
}


void CDlgSet::OnCbnSelchangeCombobox2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int sel = m_comboBox2.GetCurSel();
	CString string;
	m_comboBox2.GetLBText(sel,string);
	if(string == _T("8-连接"))	m_lineType = 8;
	else if(string == _T("4-连接"))	m_lineType = 4;
	else if(string == _T("反走样线条"))	m_lineType = CV_AA;
	UpdateData(false);
}


void CDlgSet::OnCbnSelchangeCombobox3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int sel = m_comboBox3.GetCurSel();
	CString string;
	m_comboBox3.GetLBText(sel,string);
	if(string == _T("0"))	m_shift = 0;
	else if(string == _T("1"))	m_shift = 1;
	else if(string == _T("2"))	m_shift = 2;
	else if(string == _T("3"))	m_shift = 3;
	else if(string == _T("4"))	m_shift = 4;
	else if(string == _T("5"))	m_shift = 5;
	//if(m_shift<0)	m_shift = 0;
	//else if(m_shift>5)	m_shift = 5;
	UpdateData(false);
}


void CDlgSet::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFontName;
	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	_tcscpy_s(lf.lfFaceName,LF_FACESIZE,_T("宋体"));
	CFontDialog fontDlg(&lf);
	if(fontDlg.DoModal() == IDOK)
	{
		if(m_font.m_hObject)	m_font.DeleteObject();
		m_font.CreateFontIndirectW(fontDlg.m_cf.lpLogFont);
		GetDlgItem(IDC_EDIT_FONT)->SetFont(&m_font);
		strFontName = fontDlg.m_cf.lpLogFont->lfFaceName;
		GetDlgItem(IDC_EDIT_FONT)->SetWindowText(strFontName);
		//SetDlgItemText(IDC_EDIT_FONT,strFontName);
	}
}


void CDlgSet::OnStnClickedStaticColorBackground()
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


void CDlgSet::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetDlgItem(IDC_STATIC_COLOR_BACKGROUND)->GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
		m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	}
	else m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgSet::OnBnClickedButton1()
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
	//GetDlgItem(IDC_STATIC_COLOR_BACKGROUND)
}
