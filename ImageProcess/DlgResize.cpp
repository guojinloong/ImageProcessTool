// DlgResize.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgResize.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern Mat srcImg,tempImg,dstImg,temp[];

// CDlgResize 对话框

IMPLEMENT_DYNAMIC(CDlgResize, CDialogEx)

CDlgResize::CDlgResize(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResize::IDD, pParent)
	, m_width(srcImg.cols)
	, m_height(srcImg.rows)
	, m_interWays(INTER_LINEAR)
	, m_proportion (false)
	, m_scaleX(1)
	, m_scaleY(1)
{

}

CDlgResize::~CDlgResize()
{
}

void CDlgResize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_width);
	//DDV_MinMaxDouble(pDX, m_width, 0, 3840);
	DDX_Text(pDX, IDC_EDIT2, m_height);
	//DDV_MinMaxDouble(pDX, m_height, 0, 2160);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Control(pDX, IDC_CHECK1, m_proportionCheck);
	DDX_Control(pDX, IDC_EDIT2, m_heightEdit);
}


BEGIN_MESSAGE_MAP(CDlgResize, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgResize::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgResize::OnEnChangeEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgResize::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDlgResize::OnNMCustomdrawSlider2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgResize::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgResize::OnBnClickedCheck1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgResize 消息处理程序


BOOL CDlgResize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_slider1.SetRange(0,100);
	m_slider2.SetRange(0,100);
	m_slider1.SetTicFreq(1);
	m_slider2.SetTicFreq(1);
	m_slider1.SetPos(50);
	m_slider2.SetPos(50);
	
	m_comboBox.AddString(_T("LINEAR"));
	m_comboBox.AddString(_T("NEAREST"));
	m_comboBox.AddString(_T("AREA"));
	m_comboBox.AddString(_T("CUBIC"));
	m_comboBox.AddString(_T("LANCZOS4"));
	m_comboBox.AddString(_T("FILL_OUTLIERS"));
	m_comboBox.AddString(_T("INVERSE_MAP"));
	m_comboBox.SetCurSel(0);

	m_proportionCheck.SetCheck(FALSE);

	SetTimer(1,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgResize::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_oldWidth == m_width)
	{
		if(m_bChanged1 == true)	m_bProcessed = false;
		m_bChanged1 = false;
	}
	else m_bChanged1 = true;
	m_oldWidth = m_width;

	if(m_oldHeight == m_height)
	{
		if(m_bChanged2 == true)	m_bProcessed = false;
		m_bChanged2 = false;
	}
	else m_bChanged2 = true;
	m_oldHeight = m_height;

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgResize::ContinueModal()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_bProcessed == false)
	{
		resize(srcImg,tempImg,Size(m_width,m_height),0,0,m_interWays);
		//CMainFrame *m_pMianFrm=(CMainFrame *)AfxGetMainWnd();
		//m_pMianFrm->GetActiveView()->Invalidate(FALSE);
		//m_pMianFrm->GetActiveView()->UpdateWindow();

		AfxGetMainWnd()->Invalidate(FALSE);
		AfxGetMainWnd()->UpdateWindow();
		m_bProcessed = true;
	}

	return CDialogEx::ContinueModal();
}


void CDlgResize::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	//m_slider1.SetPos((int)(m_width/m_scaleX));
	//if(m_width<0)	m_width = 0;
	//else if(m_width>3840)	m_width = 3840;
	UpdateData(false);
}


void CDlgResize::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	int pos;
	m_scaleY = (double)m_height/srcImg.rows;
	if((m_scaleY>0.1)&&(m_scaleY<10))
	{
		if(m_scaleY>1)	pos = (int)((m_scaleY-1)*10+50);
		else	pos = (int)(50-10*((1/m_scaleY)-1));
		m_slider2.SetPos(pos);
	}
	//if(m_height<0)	m_height = 0;
	//else if(m_height>2160)	m_height = 2160;
	UpdateData(false);
}


void CDlgResize::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int pos = m_slider1.GetPos();
	if(pos>50)	m_scaleX = (double)(pos-50)/10+1;
	else if(pos<50)
	{
		m_scaleX = (double)(50-pos)/10+1;
		m_scaleX = 1/m_scaleX;
	}
	m_width = (int)(srcImg.cols*m_scaleX);
	if(m_proportion == true)
	{
		m_height = (int)(srcImg.rows*m_scaleX);
		m_slider2.SetPos(pos);
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgResize::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int pos = m_slider2.GetPos();
	if(pos>50)	m_scaleY = (double)(pos-50)/10+1;
	else if(pos<50)
	{
		m_scaleY = (double)(50-pos)/10+1;
		m_scaleY = 1/m_scaleY;
	}
	m_height = (int)(srcImg.rows*m_scaleY);
	if(m_proportion == true)
	{
		m_width = (int)(srcImg.cols*m_scaleY);
		m_slider1.SetPos(pos);
	}
	UpdateData(false);
	*pResult = 0;
}


void CDlgResize::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string == _T("NEAREST"))	m_interWays = INTER_NEAREST;
	else if(string == _T("LINEAR"))	m_interWays = INTER_LINEAR;
	else if(string == _T("CUBIC"))	m_interWays = INTER_CUBIC;
	else if(string == _T("AREA"))	m_interWays = INTER_AREA;
	else if(string == _T("LANCZOS4"))	m_interWays = INTER_LANCZOS4;
	else if(string == _T("FILL_OUTLIERS"))	m_interWays = CV_WARP_FILL_OUTLIERS;
	else if(string == _T("INVERSE_MAP"))	m_interWays = CV_WARP_INVERSE_MAP;
	else m_interWays = INTER_LINEAR;
	m_bProcessed = false;
}


void CDlgResize::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_proportionCheck.GetCheck())
	{
		//m_heightEdit.SetReadOnly(true);
		m_slider2.SetPos(m_slider1.GetPos());
		m_proportion = true;
	}
	else
	{
		//m_heightEdit.SetReadOnly(false);
		m_proportion = false;
	}
}
