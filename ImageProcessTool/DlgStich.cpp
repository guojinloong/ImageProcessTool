// DlgStich.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcessTool.h"
#include "DlgStich.h"
#include "afxdialogex.h"


// CDlgStich 对话框

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


// CDlgStich 消息处理程序


BOOL CDlgStich::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgStich::OnBnClickedButton2()
{
	//// TODO: 在此添加控件通知处理程序代码
	//TCHAR szFilter[] = _T("JPEG文件(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG文件(*.png)|*.png|Windows位图(*.bmp;*.dib)|*.bmp;*.dib|TIFF文件(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters光栅文件(*.sr;*.ras)|*.sr;*.ras|便携文件格式(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
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
