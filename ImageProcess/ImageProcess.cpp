
// ImageProcess.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ImageProcess.h"
#include "MainFrm.h"

#include "ImageProcessDoc.h"
#include "ImageProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern Mat srcImg,tempImg,dstImg;
extern CString fileName,extension;

// CImageProcessApp

BEGIN_MESSAGE_MAP(CImageProcessApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CImageProcessApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CImageProcessApp::OnFileOpen)
//	ON_COMMAND(ID_FILE_SAVE, &CImageProcessApp::OnFileSave)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CImageProcessApp 构造

CImageProcessApp::CImageProcessApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ImageProcess.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CImageProcessApp 对象

CImageProcessApp theApp;


// CImageProcessApp 初始化

BOOL CImageProcessApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CImageProcessDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CImageProcessView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CImageProcessApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	destroyAllWindows();
	srcImg.release();
	tempImg.release();
	dstImg.release();
	return CWinAppEx::ExitInstance();
}

// CImageProcessApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CImageProcessApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CImageProcessApp 自定义加载/保存方法

void CImageProcessApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CImageProcessApp::LoadCustomState()
{
}

void CImageProcessApp::SaveCustomState()
{
}

// CImageProcessApp 消息处理程序





void CImageProcessApp::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szFilter[] = _T("JPEG文件(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG文件(*.png)|*.png|Windows位图(*.bmp;*.dib)|*.bmp;*.dib|TIFF文件(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters光栅文件(*.sr;*.ras)|*.sr;*.ras|便携文件格式(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
	CFileDialog fileDlg(TRUE,_T("jpg"),NULL,0,szFilter);
	CString filePath;
	if(fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();
		fileName = fileDlg.GetFileTitle();
		extension = fileDlg.GetFileExt();
		OpenDocumentFile(filePath);
	}
}

//void CImageProcessApp::OnFileSave()
//{
//	// TODO: 在此添加命令处理程序代码
//	if(srcImg.data)
//	{
//		TCHAR szFilter[] = _T("JPEG文件(*.jpeg;*.jpg;*,jpe;*jp2)|*.jpeg;*.jpg;*,jpe;*jp2|PNG文件(*.png)|*.png|Windows位图(*.bmp;*.dib)|*.bmp;*.dib|TIFF文件(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters光栅文件(*.sr;*.ras)|*.sr;*.ras|便携文件格式(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
//		CFileDialog fileDlg(FALSE,extension,fileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
//		CString filePath;
//		if(fileDlg.DoModal() == IDOK)
//		{
//			filePath = fileDlg.GetPathName();
//			SaveDocumentFile(filePath);
//		}
//	}
//	else
//	{
//		MessageBox(_T("没有载入图片！"),_T("警告"),MB_ICONWARNING);
//	}
//}
