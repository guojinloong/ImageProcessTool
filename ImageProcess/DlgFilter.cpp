// DlgFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgFilter.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern Mat srcImg,tempImg,dstImg;
//extern bool ok;	//适合窗口标志,保存标志,确定标志

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
	DDV_MinMaxInt(pDX, m_filterValue, 0, 100);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CDlgFilter, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgFilter::OnDeltaposSpin1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgFilter::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFilter::OnEnChangeEdit1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgFilter 消息处理程序


BOOL CDlgFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	m_slider.SetRange(0,100);
	m_slider.SetTicFreq(1);

	SetTimer(1,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgFilter::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_oldFilterValue == m_filterValue)
	{
		if(m_bChanged == true)	m_bProcessed = false;
		m_bChanged = false;
	}
	else m_bChanged = true;
	m_oldFilterValue = m_filterValue;
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgFilter::ContinueModal()
{
		// TODO: 在此添加专用代码和/或调用基类
	if(m_bProcessed == false)
	{
		switch(filterWays)
		{
		case 0:
			boxFilter(srcImg,tempImg,-1,Size(m_filterValue+1,m_filterValue+1));
			break;
		case 1:
			blur(srcImg,tempImg,Size(m_filterValue+1,m_filterValue+1));
			break;
		case 2:
			GaussianBlur(srcImg,tempImg,Size(m_filterValue*2+1,m_filterValue*2+1),0,0);
			break;
		case 3:
			medianBlur(srcImg,tempImg,m_filterValue*2+1);
			break;
		case 4:
			bilateralFilter(srcImg,tempImg,m_filterValue,m_filterValue*2,m_filterValue/2);
			break;
		default:
			boxFilter(srcImg,tempImg,-1,Size(m_filterValue+1,m_filterValue+1));
			break;
		}
		CMainFrame *m_pMianFrm=(CMainFrame *)AfxGetMainWnd();
		m_pMianFrm->GetActiveView()->Invalidate(FALSE);
		m_pMianFrm->GetActiveView()->UpdateWindow();

		//AfxGetMainWnd()->Invalidate(FALSE);
		//AfxGetMainWnd()->UpdateWindow();
		m_bProcessed = true;
	}
	return CDialogEx::ContinueModal();
}


void CDlgFilter::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_filterValue>0)	m_filterValue--;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_filterValue<100)	m_filterValue++;
	}
	m_slider.SetPos(m_filterValue);
	//CString ss;
	//ss.Format(_T("%d"),m_filterValue);
	//GetDlgItem(IDC_EDIT1)->SetWindowText(ss);
	UpdateData(false);
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


//void CDlgFilter::OnOK()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	ok = true;
//	AfxGetMainWnd()->Invalidate();
//	AfxGetMainWnd()->UpdateWindow();
//	//((CDlgFilter*)m_pMain)->Invalidate(FALSE);
//	//((CDlgFilter*)m_pMain)->UpdateWindow();
//	CDialogEx::OnOK();
//}


//void CDlgFilter::OnCancel()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	srcImg.copyTo(tempImg);
//	AfxGetMainWnd()->Invalidate();
//	AfxGetMainWnd()->UpdateWindow();
//	CDialogEx::OnCancel();
//}
