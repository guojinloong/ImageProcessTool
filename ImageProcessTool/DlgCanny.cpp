// DlgEdgeCanny.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgCanny.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern Mat srcImg,tempImg,dstImg;

// CDlgCanny �Ի���

IMPLEMENT_DYNAMIC(CDlgCanny, CDialogEx)

CDlgCanny::CDlgCanny(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCanny::IDD, pParent)
	, m_cannyThreshold1(0)
	, m_cannyThreshold2(255)
	, m_apertureSize(3)
{

}

CDlgCanny::~CDlgCanny()
{
}

void CDlgCanny::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Text(pDX, IDC_EDIT1, m_cannyThreshold1);
	DDV_MinMaxInt(pDX, m_cannyThreshold1, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_cannyThreshold2);
	DDV_MinMaxInt(pDX, m_cannyThreshold2, 0, 255);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
}


BEGIN_MESSAGE_MAP(CDlgCanny, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgCanny::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgCanny::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgCanny::OnEnChangeEdit2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgCanny::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CDlgCanny::OnNMCustomdrawSlider2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgCanny ��Ϣ�������


BOOL CDlgCanny::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//m_apertureSize.AddString(_T("1"));
	m_comboBox.AddString(_T("3"));
	m_comboBox.AddString(_T("5"));
	m_comboBox.AddString(_T("7"));
	m_comboBox.SetCurSel(0);

	m_slider1.SetRange(0,255);
	m_slider2.SetRange(0,255);
	m_slider1.SetTicFreq(1);
	m_slider2.SetTicFreq(1);
	m_slider1.SetPos(m_cannyThreshold1);
	m_slider2.SetPos(m_cannyThreshold2);
	
	SetTimer(1,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgCanny::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_oldCannyThreshold1 == m_cannyThreshold1)
	{
		if(m_bChanged1 == true)	m_bProcessed = false;
		m_bChanged1 = false;
	}
	else m_bChanged1 = true;
	m_oldCannyThreshold1 = m_cannyThreshold1;

	if(m_oldCannyThreshold2 == m_cannyThreshold2)
	{
		if(m_bChanged2 == true)	m_bProcessed = false;
		m_bChanged2 = false;
	}
	else m_bChanged2 = true;
	m_oldCannyThreshold2 = m_cannyThreshold2;
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgCanny::ContinueModal()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_bProcessed == false)
	{
		Mat mask;
		blur(srcImg,tempImg,Size(3,3));
		Canny(tempImg,mask,m_cannyThreshold1,m_cannyThreshold2,m_apertureSize);
		tempImg=Scalar::all(0);
		srcImg.copyTo(tempImg,mask);
		//imshow("��Canny��Ե��⡿",tempImg);
		mask.release();
		CMainFrame *m_pMianFrm=(CMainFrame *)AfxGetMainWnd();
		m_pMianFrm->GetActiveView()->Invalidate(FALSE);
		m_pMianFrm->GetActiveView()->UpdateWindow();
		m_bProcessed = true;
	}

	return CDialogEx::ContinueModal();
}


void CDlgCanny::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string == _T("3"))	m_apertureSize = 3;
	else if(string == _T("5"))	m_apertureSize = 5;
	else if(string == _T("7"))	m_apertureSize = 7;
	m_bProcessed = false;
}


void CDlgCanny::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_cannyThreshold1<0)	m_cannyThreshold1 = 0;
	else if(m_cannyThreshold1>255)	m_cannyThreshold1 = 255;
	m_slider1.SetPos(m_cannyThreshold1);
	UpdateData(false);
}


void CDlgCanny::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_cannyThreshold2<0)	m_cannyThreshold2 = 0;
	else if(m_cannyThreshold2>255)	m_cannyThreshold2 = 255;
	m_slider2.SetPos(m_cannyThreshold2);
	UpdateData(false);
}


void CDlgCanny::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_cannyThreshold1 = m_slider1.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgCanny::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_cannyThreshold2 = m_slider2.GetPos();
	UpdateData(false);
	*pResult = 0;
}