// DlgStich.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgStich.h"
#include "afxdialogex.h"


// CDlgStich �Ի���

IMPLEMENT_DYNAMIC(CDlgStich, CDialogEx)

CDlgStich::CDlgStich(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStich::IDD, pParent)
{

}

CDlgStich::~CDlgStich()
{
}

void CDlgStich::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStich, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgStich::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgStich ��Ϣ�������


BOOL CDlgStich::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgStich::OnBnClickedButton2()
{
	//// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG�ļ�(*.png)|*.png|Windowsλͼ(*.bmp;*.dib)|*.bmp;*.dib|TIFF�ļ�(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters��դ�ļ�(*.sr;*.ras)|*.sr;*.ras|��Я�ļ���ʽ(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
	//CFileDialog fileDlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT|OFN_ENABLEHOOK,szFilter);
	//if(fileDlg.DoModal() == IDOK)
	//{
	//	//filePath = fileDlg.GetPathName();
	//	//fileName = fileDlg.GetFileTitle();
	//	//extension = fileDlg.GetFileExt();
	//	CStringA file(filePath.GetBuffer(0));
	//	filePath.ReleaseBuffer();
	//	string path=file.GetBuffer(0);
	//	file.ReleaseBuffer();
	//	if(!dstImg.data)	dstImg = srcImg.clone();
	//	imwrite(path,dstImg);
	//	TRACE(_T("Save As Success!\n"));
	//	saveFlag = false;
	//}
}
