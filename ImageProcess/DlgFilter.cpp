// DlgFilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgFilter.h"
#include "afxdialogex.h"

extern Mat srcImg,tempImg,dstImg;

// CDlgFilter �Ի���

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


// CDlgFilter ��Ϣ�������


void CDlgFilter::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_filterValue = m_slider.GetPos();
	UpdateData(false);
	*pResult = 0;
}


BOOL CDlgFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_slider.SetRange(0,40);
	m_slider.SetTicFreq(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgFilter::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_slider.SetPos(m_filterValue);
	UpdateData(false);
}