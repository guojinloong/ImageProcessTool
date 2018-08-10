
// ImageProcessTool.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ImageProcessTool.h"
#include "MainFrm.h"

#include "ImageProcessToolDoc.h"
#include "ImageProcessToolView.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern Mat srcImg,tempImg,dstImg,temp[10];
extern CString filePath,fileName,extension;
//extern CvvImage image;
//extern int t,tMax;
extern bool ok,fitWindow,saveFlag;

// CImageProcessToolApp

BEGIN_MESSAGE_MAP(CImageProcessToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CImageProcessToolApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CImageProcessToolApp::OnFileOpen)
//	ON_COMMAND(ID_FILE_SAVE, &CImageProcessToolApp::OnFileSave)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CImageProcessToolApp ����

CImageProcessToolApp::CImageProcessToolApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ImageProcessTool.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CImageProcessToolApp ����

CImageProcessToolApp theApp;


// CImageProcessToolApp ��ʼ��

BOOL CImageProcessToolApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	
	//SetDialogBkColor(RGB(127,127,127),RGB(255,255,255));
	Gdiplus::GdiplusStartup(&m_gdiplusToken,&m_gdiplusStartupInput,NULL);//���ô˺�������GDI+����

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CImageProcessToolDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CImageProcessToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CImageProcessToolApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	Gdiplus::GdiplusShutdown(m_gdiplusToken);//ʹ����ɵ��ô˺����ر�GDI+��
	destroyAllWindows();
	srcImg.release();
	dstImg.release();
	tempImg.release();
	for(int i=0;i<10;i++)	temp[i].release();
	return CWinAppEx::ExitInstance();
}

// CImageProcessToolApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_string;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
	, m_string(_T("������ܡ��л����񹲺͹�����Ȩ�����������������Դ������������ʣ�����ϵ����QQ��2360787940���䣺2360787940@qq.com"))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//m_string = _T("������ܡ��л����񹲺͹�����Ȩ�����������������Դ������������ʣ�����ϵ����QQ��2360787940���䣺2360787940@qq.com");
	DDX_Text(pDX, IDC_EDIT1, m_string);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CImageProcessToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CImageProcessToolApp �Զ������/���淽��

void CImageProcessToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CImageProcessToolApp::LoadCustomState()
{
}

void CImageProcessToolApp::SaveCustomState()
{
}

// CImageProcessToolApp ��Ϣ�������

int CImageProcessToolApp::Run()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CWinAppEx::Run();
}


void CImageProcessToolApp::OnFileOpen()
{
	// TODO: �ڴ���������������
	TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG�ļ�(*.png)|*.png|ICONͼ��(*.icon;*.ico)|*.icon;*.ico|Windowsλͼ(*.bmp;*.dib)|*.bmp;*.dib|Windows��Ƶ(*.avi)|*.avi|TIFF�ļ�(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters��դ�ļ�(*.sr;*.ras)|*.sr;*.ras|��Я�ļ���ʽ(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm|XML�ļ�(*.xml)|*.xml|YAML�ļ�(*.yaml;*.yml)|*.yaml;*.yml|TXT�ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||"); 
	CFileDialog fileDlg(TRUE,extension,fileName,0,szFilter);
	if(fileDlg.DoModal() == IDOK)
	{
		INT_PTR nRes = NULL;
		if(saveFlag == true)	nRes = MessageBox(AfxGetMainWnd()->m_hWnd,_T("�Ƿ񱣴��޸Ĺ����ļ���"),fileName,MB_YESNOCANCEL|MB_ICONQUESTION);//AfxGetMainWnd()->m_hWnd,
		if(nRes == IDYES)
		{
			CStringA file(filePath.GetBuffer(0));
			filePath.ReleaseBuffer();
			string path=file.GetBuffer(0);
			file.ReleaseBuffer();
			imwrite(path,dstImg);
			TRACE(_T("Save Success!\n"));
		}
		else if(nRes == IDCANCEL)	return ;
		filePath = fileDlg.GetPathName();
		fileName = fileDlg.GetFileTitle();
		extension = fileDlg.GetFileExt();
		OpenDocumentFile(filePath);
		TRACE(_T("Open Success!\n"));
	}
}