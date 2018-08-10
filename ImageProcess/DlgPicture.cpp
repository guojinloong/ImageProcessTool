// DlgPicture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgPicture.h"
#include "afxdialogex.h"

extern CvvImage image;

// CDlgPicture �Ի���

IMPLEMENT_DYNAMIC(CDlgPicture, CDialogEx)

CDlgPicture::CDlgPicture(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPicture::IDD, pParent)
{

}

CDlgPicture::~CDlgPicture()
{
}

void CDlgPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPicture, CDialogEx)
END_MESSAGE_MAP()


// CDlgPicture ��Ϣ�������


BOOL CDlgPicture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	CRect rt;
	CRect rt;
	GetDlgItem(IDC_STATIC_PICTURE)->GetClientRect(&rt);
	double ViewScale = 1;
	if((image.Width() > rt.Width())||(image.Height() > rt.Height()))
	{
		float widthCoff = (float)rt.Width() / image.Width();
		float heightCoff = (float)rt.Height() / image.Height();
		ViewScale = widthCoff < heightCoff ? widthCoff:heightCoff;
	}
	int imageX = (int)(image.Width()*ViewScale);
	int imageY = (int)(image.Height()*ViewScale);
	CRect rect((rt.Width()-imageX)/2,(rt.Width()+imageX)/2,(rt.Height()-imageY)/2,(rt.Height()+imageY)/2);
	CDC* pDC = GetDlgItem( IDC_STATIC_PICTURE )->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	image.DrawToHDC(hDC,&rect);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
