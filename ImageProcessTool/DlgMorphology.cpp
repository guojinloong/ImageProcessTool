// DlgMorphology.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgMorphology.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern Mat srcImg,tempImg,dstImg;

// CDlgMorphology �Ի���

IMPLEMENT_DYNAMIC(CDlgMorphology, CDialogEx)

CDlgMorphology::CDlgMorphology(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMorphology::IDD, pParent)
	, shape(MORPH_RECT)
	, m_structElementSize(10)
{

}

CDlgMorphology::~CDlgMorphology()
{
	
}

void CDlgMorphology::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Text(pDX, IDC_EDIT1, m_structElementSize);
}


BEGIN_MESSAGE_MAP(CDlgMorphology, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgMorphology::OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgMorphology::OnNMCustomdrawSlider1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgMorphology::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgMorphology::OnEnChangeEdit1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMorphology ��Ϣ�������


BOOL CDlgMorphology::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comboBox.AddString(_T("MORPH_RECT"));
	m_comboBox.AddString(_T("MORPH_CROSS"));
	m_comboBox.AddString(_T("MORPH_ELLIPSE"));
	m_comboBox.SetCurSel(0);

	m_slider.SetRange(0,50);
	m_slider.SetTicFreq(1);
	m_slider.SetPos(m_structElementSize);

	SetTimer(1,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgMorphology::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_oldStructElementSize == m_structElementSize)
	{
		if(m_bChanged == true)	m_bProcessed = false;
		m_bChanged = false;
	}
	else m_bChanged = true;
	m_oldStructElementSize = m_structElementSize;

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgMorphology::ContinueModal()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_bProcessed == false)
	{
		Mat element = getStructuringElement(shape,Size(m_structElementSize*2+1,m_structElementSize*2+1),Point(m_structElementSize,m_structElementSize));
		switch(m_morphWays)
		{
		case 0:
			dilate(srcImg,tempImg,element);
			break;
		case 1:
			erode(srcImg,tempImg,element);
			break;
		case 2:
			morphologyEx(srcImg,tempImg,MORPH_OPEN,element);
			break;
		case 3:
			morphologyEx(srcImg,tempImg,MORPH_CLOSE,element);
			break;
		case 4:
			morphologyEx(srcImg,tempImg,MORPH_GRADIENT,element);
			break;
		case 5:
			morphologyEx(srcImg,tempImg,MORPH_TOPHAT,element);
			break;
		case 6:
			morphologyEx(srcImg,tempImg,MORPH_BLACKHAT,element);
			break;
		default:
			dilate(srcImg,tempImg,element);
			break;
		}
		CMainFrame *m_pMianFrm=(CMainFrame *)AfxGetMainWnd();
		m_pMianFrm->GetActiveView()->Invalidate(FALSE);
		m_pMianFrm->GetActiveView()->UpdateWindow();
		element.release();
		m_bProcessed = true;
	}

	return CDialogEx::ContinueModal();
}


void CDlgMorphology::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_comboBox.GetCurSel();
	CString string;
	m_comboBox.GetLBText(sel,string);
	if(string = _T("MORPH_RECT"))	shape = MORPH_RECT;
	else if(string = _T("MORPH_CROSS"))	shape = MORPH_CROSS;
	else if(string = _T("MORPH_ELLIPSE"))	shape = MORPH_ELLIPSE;
	else shape = MORPH_RECT;
	m_bProcessed = false;
}


void CDlgMorphology::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_structElementSize = m_slider.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgMorphology::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_slider.SetPos(m_structElementSize);
	UpdateData(false);
}



void CDlgMorphology::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(pNMUpDown->iDelta == 1)
	{
		if(m_structElementSize>0)	m_structElementSize--;
	}
	else if(pNMUpDown->iDelta == -1)
	{
		if(m_structElementSize<50)	m_structElementSize++;
	}
	m_slider.SetPos(m_structElementSize);
	CString ss;
	ss.Format(_T("%d"),m_structElementSize);
	GetDlgItem(IDC_EDIT1)->SetWindowText(ss);
	UpdateData(false);
	*pResult = 0;
}