// DlgAffineWarp.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgAffineWarp.h"
#include "afxdialogex.h"

extern Mat srcImg,tempImg;

// CDlgAffineWarp 对话框

IMPLEMENT_DYNAMIC(CDlgAffineWarp, CDialogEx)

CDlgAffineWarp::CDlgAffineWarp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAffineWarp::IDD, pParent)
	, m_colorR(0)
	, m_colorG(0)
	, m_colorB(0)
	, m_interWays(INTER_LINEAR)
	, m_affineWays(0)
	, m_pointSelect(0)
	, m_angle(0)
	, m_scale(1)
	, m_button(0)
	, m_viewScale(1)
	, m_affineDirection(0)
{

}

CDlgAffineWarp::~CDlgAffineWarp()
{
}

void CDlgAffineWarp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Text(pDX, IDC_EDIT1, m_colorR);
	DDV_MinMaxInt(pDX, m_colorR, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_colorG);
	DDV_MinMaxInt(pDX, m_colorG, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_colorB);
	DDV_MinMaxInt(pDX, m_colorB, 0, 255);
	DDX_Radio(pDX, IDC_RADIO1, m_affineWays);
	DDV_MinMaxInt(pDX, m_affineWays, 0, 1);
	DDX_Radio(pDX, IDC_RADIO3, m_pointSelect);
	DDV_MinMaxInt(pDX, m_pointSelect, 0, 2);
	DDX_Text(pDX, IDC_EDIT5, m_angle);
	DDV_MinMaxDouble(pDX, m_angle, 0, 360);
	DDX_Text(pDX, IDC_EDIT4, m_scale);
	DDV_MinMaxDouble(pDX, m_scale, 0.1, 10);
	DDX_Control(pDX, IDC_RADIO3, m_radio);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox1);
	DDX_Text(pDX, IDC_EDIT5, m_angle);
}


BEGIN_MESSAGE_MAP(CDlgAffineWarp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgAffineWarp::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgAffineWarp::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgAffineWarp::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgAffineWarp::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgAffineWarp::OnEnChangeEdit3)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_RADIO1, &CDlgAffineWarp::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgAffineWarp::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgAffineWarp::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgAffineWarp::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &CDlgAffineWarp::OnBnClickedRadio2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgAffineWarp::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgAffineWarp::OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgAffineWarp::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgAffineWarp::OnEnChangeEdit4)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgAffineWarp::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CDlgAffineWarp 消息处理程序



BOOL CDlgAffineWarp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	m_comboBox.AddString(_T("LINEAR"));
	m_comboBox.AddString(_T("NEAREST"));
	m_comboBox.AddString(_T("AREA"));
	m_comboBox.AddString(_T("CUBIC"));
	m_comboBox.AddString(_T("LANCZOS4"));
	m_comboBox.AddString(_T("FILL_OUTLIERS"));
	m_comboBox.AddString(_T("INVERSE_MAP"));
	m_comboBox.SetCurSel(0);
	
	m_comboBox1.AddString(_T("正方向"));
	m_comboBox1.AddString(_T("反方向"));
	m_comboBox1.SetCurSel(0);

	if(m_affineWays == 0)
	{
		GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(_T("目标点坐标"));
		GetDlgItem(IDC_SPIN1)->ShowWindow(true);
		GetDlgItem(IDC_SPIN2)->ShowWindow(true);
		GetDlgItem(IDC_RADIO3)->ShowWindow(true);
		GetDlgItem(IDC_RADIO4)->ShowWindow(true);
		GetDlgItem(IDC_RADIO5)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_LEFTTOP)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_RIGHTTOP)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_LEFTBOTTOM)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_ANCHOR)->ShowWindow(false);
		GetDlgItem(IDC_STATIC1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC3)->ShowWindow(false);
		GetDlgItem(IDC_EDIT4)->ShowWindow(false);
		GetDlgItem(IDC_EDIT5)->ShowWindow(false);
	}
	else if(m_affineWays == 1)
	{
		GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(_T("旋转参数"));
		GetDlgItem(IDC_SPIN1)->ShowWindow(false);
		GetDlgItem(IDC_SPIN2)->ShowWindow(false);
		GetDlgItem(IDC_RADIO3)->ShowWindow(false);
		GetDlgItem(IDC_RADIO4)->ShowWindow(false);
		GetDlgItem(IDC_RADIO5)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_LEFTTOP)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_RIGHTTOP)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_LEFTBOTTOM)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_ANCHOR)->ShowWindow(true);
		GetDlgItem(IDC_STATIC1)->ShowWindow(true);
		GetDlgItem(IDC_STATIC2)->ShowWindow(true);
		GetDlgItem(IDC_STATIC3)->ShowWindow(true);
		GetDlgItem(IDC_EDIT4)->ShowWindow(true);
		GetDlgItem(IDC_EDIT5)->ShowWindow(true);
	}

	IplImage img = tempImg;
	image.CopyOf(&img);
	CRect rt;
	GetDlgItem(IDC_STATIC_SHOW)->GetClientRect(&rt);
	if(image.Width() > image.Height())
	{
		m_viewScale = (float)rt.Width() / image.Width();
	}
	else
	{
		m_viewScale = (float)rt.Height() / image.Height();
	}
	int imageX = (int)(image.Width()*m_viewScale);
	int imageY = (int)(image.Height()*m_viewScale);
	rect = CRect((rt.Width()-imageX)/2,(rt.Height()-imageY)/2,(rt.Width()+imageX)/2,(rt.Height()+imageY)/2);
	CDC* pDC = GetDlgItem( IDC_STATIC_SHOW )->GetDC();
	hDC=pDC->GetSafeHdc();

	m_anchor = CPoint(rect.Width()/2,rect.Height()/2);
	m_point[0] = CPoint(0,0);
	m_point[1] = CPoint(rect.Width(),0);
	m_point[2] = CPoint(0,rect.Height());
	CString corrdinate;
	corrdinate.Format(_T("%dx%d"),m_anchor.x,m_anchor.y);
	GetDlgItem(IDC_STATIC_ANCHOR)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[0].x,m_point[0].y);
	GetDlgItem(IDC_STATIC_LEFTTOP)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[1].x,m_point[1].y);
	GetDlgItem(IDC_STATIC_RIGHTTOP)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[2].x,m_point[2].y);
	GetDlgItem(IDC_STATIC_LEFTBOTTOM)->SetWindowText(corrdinate);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgAffineWarp::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
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
	UpdateData(false);
}


void CDlgAffineWarp::OnBnClickedButton1()
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

void CDlgAffineWarp::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_colorR<0)	m_colorR = 0;
	else if(m_colorR>255)	m_colorR = 255;
	UpdateData(false);
}


void CDlgAffineWarp::OnEnChangeEdit2()
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


void CDlgAffineWarp::OnEnChangeEdit3()
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


BOOL CDlgAffineWarp::ContinueModal()
{
	// TODO: 在此添加专用代码和/或调用基类
	CRect rt = rect;
	rt.left+=1;
	rt.top+=1;
	rt.right-=1;
	rt.bottom-=1;
	image.DrawToHDC(hDC,&rt);
	rt = rect;
	rt.left+=9;
	rt.top+=10;
	rt.right+=9;
	rt.bottom+=10;
	CPoint p[4];
	p[0] = m_point[0];
	p[1] = m_point[1];
	p[2] = m_point[2];
	p[3] = m_anchor;
	//p[0].x+=rt.left;
	//p[0].y+=rt.top;
	//p[1].x+=rt.left;
	//p[1].y+=rt.top;
	//p[2].x+=rt.left;
	//p[2].y+=rt.top;
	//p[3].x+=rt.left;
	//p[3].y+=rt.top;
	CRect rec[4];
	for(int i=0;i<4;i++)
	{
		p[i].x+=rt.left;
		p[i].y+=rt.top;
		rec[i] = CRect(p[i].x-5,p[i].y-5,p[i].x+5,p[i].y+5);
		//if(rec[i].left<rt.left)	rec[i].left = rt.left;
		//if(rec[i].right>rt.right)	rec[i].right = rt.right;
		//if(rec[i].top<rt.top)	rec[i].top=rt.top;
		//if(rec[i].bottom>rt.bottom)	rec[i].bottom=rt.bottom;
	}
	//rt0 = CRect(p[0].x-5,p[0].y-5,p[0].x+5,p[0].y+5);
	//rt1 = CRect(p[1].x-5,p[1].y-5,p[1].x+5,p[1].y+5);
	//rt2 = CRect(p[2].x-5,p[2].y-5,p[2].x+5,p[2].y+5);
	//rt3 = CRect(p[3].x-5,p[3].y-5,p[3].x+5,p[3].y+5);
	CClientDC dc(this);
	CBrush brush,*oldBrush;
	brush.CreateSolidBrush(RGB(255,0,0));
	oldBrush = dc.SelectObject(&brush);
	if(m_affineWays == 0)
	{
		dc.Ellipse(rec[0]);
		dc.Ellipse(rec[1]);
		dc.Ellipse(rec[2]);
		CBrush brush(RGB(0,255,0));
		dc.SelectObject(brush);
		dc.Rectangle(p[m_pointSelect].x-5,p[m_pointSelect].y-5,p[m_pointSelect].x+5,p[m_pointSelect].y+5);
	}
	else if(m_affineWays == 1)
	{
		CBrush brush(RGB(0,0,255));
		dc.SelectObject(brush);
		dc.Ellipse(rec[3]);
	}
	dc.SelectObject(oldBrush);
	return CDialogEx::ContinueModal();
}


void CDlgAffineWarp::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	CRect rt = rect;
	rt.left+=9;
	rt.top+=10;
	rt.right+=9;
	rt.bottom+=10;
	if(rt.PtInRect(point))
	{
		if(m_affineWays == 0)
		{
			point.x-=rt.left;
			point.y-=rt.top;
			CRect rt0,rt1,rt2;
			rt0 = CRect(m_point[0].x-10,m_point[0].y-10,m_point[0].x+10,m_point[0].y+10);
			rt1 = CRect(m_point[1].x-10,m_point[1].y-10,m_point[1].x+10,m_point[1].y+10);
			rt2 = CRect(m_point[2].x-10,m_point[2].y-10,m_point[2].x+10,m_point[2].y+10);
			if(rt0.PtInRect(point))	m_pointSelect = 0;
			else if(rt1.PtInRect(point))	m_pointSelect = 1;
			else if(rt2.PtInRect(point))	m_pointSelect = 2;
			m_radio.SetCheck(m_pointSelect);
		}
		SetCursor(m_hCursor);
		SetCapture();
		rt = rect;
		GetDlgItem(IDC_STATIC_SHOW)->ClientToScreen(&rt);
		ClipCursor(rt);
		m_button = 1;
	}
	UpdateData(false);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgAffineWarp::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	CRect rt = rect;
	rt.left+=9;
	rt.top+=10;
	rt.right+=9;
	rt.bottom+=10;
	if(rt.PtInRect(point))
	{
		point.x-=rt.left;
		point.y-=rt.top;
		CString corrdinate;
		if(m_affineWays == 0)
		{
			m_point[m_pointSelect] = point;
			corrdinate.Format(_T("%dx%d"),m_point[0].x,m_point[0].y);
			GetDlgItem(IDC_STATIC_LEFTTOP)->SetWindowText(corrdinate);
			corrdinate.Format(_T("%dx%d"),m_point[1].x,m_point[1].y);
			GetDlgItem(IDC_STATIC_RIGHTTOP)->SetWindowText(corrdinate);
			corrdinate.Format(_T("%dx%d"),m_point[2].x,m_point[2].y);
			GetDlgItem(IDC_STATIC_LEFTBOTTOM)->SetWindowText(corrdinate);
			m_pointSelect++;
			if(m_pointSelect == 3)	m_pointSelect = 0;
			m_radio.SetCheck(m_pointSelect);
		}
		else if(m_affineWays == 1)
		{
			m_anchor = point;
			corrdinate.Format(_T("%dx%d"),m_anchor.x,m_anchor.y);
			GetDlgItem(IDC_STATIC_ANCHOR)->SetWindowText(corrdinate);
		}
		ReleaseCapture();
		ClipCursor(NULL);
		m_button = 0;
	}
	UpdateData(false);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgAffineWarp::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	CRect rt = rect;
	rt.left+=9;
	rt.top+=10;
	rt.right+=9;
	rt.bottom+=10;
	if(rt.PtInRect(point))
	{
		point.x-=rt.left;
		point.y-=rt.top;
		CString cursor;
		cursor.Format(_T("Cursor:%dx%d"),point.x,point.y);
		GetDlgItem(IDC_STATIC_CURSOR)->SetWindowText(cursor);
		//ClientToScreen(&rect);
		CRect rt0,rt1,rt2,rt3;
		rt0 = CRect(m_point[0].x-10,m_point[0].y-10,m_point[0].x+10,m_point[0].y+10);
		rt1 = CRect(m_point[1].x-10,m_point[1].y-10,m_point[1].x+10,m_point[1].y+10);
		rt2 = CRect(m_point[2].x-10,m_point[2].y-10,m_point[2].x+10,m_point[2].y+10);
		rt3 = CRect(m_anchor.x-10,m_anchor.y-10,m_anchor.x+10,m_anchor.y+10);
		CString corrdinate;
		if(m_affineWays == 0)
		{
			if(rt0.PtInRect(point)||rt1.PtInRect(point)||rt2.PtInRect(point))
			{
				if(m_button == 1)
				{
					m_point[m_pointSelect] = point;
					corrdinate.Format(_T("%dx%d"),m_point[0].x,m_point[0].y);
					GetDlgItem(IDC_STATIC_LEFTTOP)->SetWindowText(corrdinate);
					corrdinate.Format(_T("%dx%d"),m_point[1].x,m_point[1].y);
					GetDlgItem(IDC_STATIC_RIGHTTOP)->SetWindowText(corrdinate);
					corrdinate.Format(_T("%dx%d"),m_point[2].x,m_point[2].y);
					GetDlgItem(IDC_STATIC_LEFTBOTTOM)->SetWindowText(corrdinate);
					//Invalidate();
				}
				m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
			}
			else
			{
				m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
			}
		}
		else if(m_affineWays == 1)
		{
			if(rt3.PtInRect(point))
			{
				if(m_button == 1)
				{
					m_anchor = point;
					corrdinate.Format(_T("%dx%d"),m_anchor.x,m_anchor.y);
					GetDlgItem(IDC_STATIC_ANCHOR)->SetWindowText(corrdinate);
					//Invalidate();
				}
				m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
			}
			else
			{
				m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
			}
		}
		//Invalidate();
	}
	else m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	SetCursor(m_hCursor);
	UpdateData(false);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgAffineWarp::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_affineWays == 0)
	{
		GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(_T("目标点坐标"));
		GetDlgItem(IDC_SPIN1)->ShowWindow(true);
		GetDlgItem(IDC_SPIN2)->ShowWindow(true);
		GetDlgItem(IDC_RADIO3)->ShowWindow(true);
		GetDlgItem(IDC_RADIO4)->ShowWindow(true);
		GetDlgItem(IDC_RADIO5)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_LEFTTOP)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_RIGHTTOP)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_LEFTBOTTOM)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_ANCHOR)->ShowWindow(false);
		GetDlgItem(IDC_STATIC1)->ShowWindow(false);
		GetDlgItem(IDC_STATIC2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC3)->ShowWindow(false);
		GetDlgItem(IDC_EDIT4)->ShowWindow(false);
		GetDlgItem(IDC_EDIT5)->ShowWindow(false);
	}
	else if(m_affineWays == 1)
	{
		GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(_T("旋转参数"));
		GetDlgItem(IDC_SPIN1)->ShowWindow(false);
		GetDlgItem(IDC_SPIN2)->ShowWindow(false);
		GetDlgItem(IDC_RADIO3)->ShowWindow(false);
		GetDlgItem(IDC_RADIO4)->ShowWindow(false);
		GetDlgItem(IDC_RADIO5)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_LEFTTOP)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_RIGHTTOP)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_LEFTBOTTOM)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_ANCHOR)->ShowWindow(true);
		GetDlgItem(IDC_STATIC1)->ShowWindow(true);
		GetDlgItem(IDC_STATIC2)->ShowWindow(true);
		GetDlgItem(IDC_STATIC3)->ShowWindow(true);
		GetDlgItem(IDC_EDIT4)->ShowWindow(true);
		GetDlgItem(IDC_EDIT5)->ShowWindow(true);
	}
	Invalidate();
	UpdateData(false);
}


void CDlgAffineWarp::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_radio.SetCheck(m_pointSelect);
	UpdateData(false);
}

void CDlgAffineWarp::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMUpDown->iDelta == -1)
	{
		if(m_point[m_pointSelect].x<rect.Width())	m_point[m_pointSelect].x++;
	}
	else if(pNMUpDown->iDelta == 1)
	{
		if(m_point[m_pointSelect].x>0)	m_point[m_pointSelect].x--;
	}
	CString corrdinate;
	corrdinate.Format(_T("%dx%d"),m_point[0].x,m_point[0].y);
	GetDlgItem(IDC_STATIC_LEFTTOP)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[1].x,m_point[1].y);
	GetDlgItem(IDC_STATIC_RIGHTTOP)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[2].x,m_point[2].y);
	GetDlgItem(IDC_STATIC_LEFTBOTTOM)->SetWindowText(corrdinate);
	*pResult = 0;
}


void CDlgAffineWarp::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMUpDown->iDelta == -1)
	{
		if(m_point[m_pointSelect].y<rect.Height())	m_point[m_pointSelect].y++;
	}
	else if(pNMUpDown->iDelta == 1)
	{
		if(m_point[m_pointSelect].y>0) m_point[m_pointSelect].y--;
	}
	CString corrdinate;
	corrdinate.Format(_T("%dx%d"),m_point[0].x,m_point[0].y);
	GetDlgItem(IDC_STATIC_LEFTTOP)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[1].x,m_point[1].y);
	GetDlgItem(IDC_STATIC_RIGHTTOP)->SetWindowText(corrdinate);
	corrdinate.Format(_T("%dx%d"),m_point[2].x,m_point[2].y);
	GetDlgItem(IDC_STATIC_LEFTBOTTOM)->SetWindowText(corrdinate);
	*pResult = 0;
}


void CDlgAffineWarp::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_angle<0)	m_angle = 0;
	else if(m_angle>360)	m_angle = 360;
	UpdateData(false);
}


void CDlgAffineWarp::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_scale<0.1)	m_scale = 0.1;
	else if(m_scale>10)	m_scale = 10;
	UpdateData(false);
}


void CDlgAffineWarp::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox1.GetCurSel();
	CString string;
	m_comboBox1.GetLBText(sel,string);
	if(string == _T("正方向"))	m_affineDirection = 0;
	else if(string == _T("反方向"))	m_affineDirection = 1;
	else m_affineDirection = 0;
}
