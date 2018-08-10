
// ImageProcessToolDoc.cpp : CImageProcessToolDoc 类的实现
//
#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageProcessTool.h"
#endif

#include "ImageProcessToolDoc.h"
#include <propkey.h>
extern Mat srcImg,tempImg,dstImg,temp[];
extern CString filePath,fileName,extension;
//extern CString strings;
extern int t,tMax;
extern bool fitWindow,saveFlag;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageProcessToolDoc

IMPLEMENT_DYNCREATE(CImageProcessToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessToolDoc, CDocument)

	ON_COMMAND(ID_FILE_SAVE, &CImageProcessToolDoc::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CImageProcessToolDoc::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImageProcessToolDoc::OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CImageProcessToolDoc::OnUpdateFileSaveAs)
END_MESSAGE_MAP()


// CImageProcessToolDoc 构造/析构

CImageProcessToolDoc::CImageProcessToolDoc()
{
	// TODO: 在此添加一次性构造代码

}

CImageProcessToolDoc::~CImageProcessToolDoc()
{
}

//BOOL CImageProcessToolDoc::OnNewDocument()
//{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
//
//	// TODO: 在此添加重新初始化代码
//	// (SDI 文档将重用该文档)
//
//	return TRUE;
//}




// CImageProcessToolDoc 序列化

void CImageProcessToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CImageProcessToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CImageProcessToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CImageProcessToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProcessToolDoc 诊断

#ifdef _DEBUG
void CImageProcessToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL CImageProcessToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	
	if((extension == _T("xml")||(extension == _T("yaml"))||(extension == _T("yml"))||(extension == _T("txt"))))
	{
		CFile file;
		int hResult = file.Open(lpszPathName,CFile::modeReadWrite);
		if(hResult == 0)	AfxMessageBox(_T("FALSE!"));
		file.Read(str.GetBuffer((UINT)(file.GetLength())),(UINT)(file.GetLength()));
		//str.Format(_T("%d"),str.GetLength());
		file.Close();
		srcImg.release();
	}
	else
	{
		int num = WideCharToMultiByte(CP_OEMCP,NULL,lpszPathName,-1,NULL,0,NULL,FALSE);
		char *pchar = new char[num];
		WideCharToMultiByte(CP_OEMCP,NULL,lpszPathName,-1,pchar,num,NULL,FALSE);
		image.Load(pchar);
		Mat img = image.GetImage();
		img.copyTo(srcImg);
		img.copyTo(dstImg);
		img.copyTo(tempImg);
		img.copyTo(temp[0]);
		img.release();
		TRACE(_T("Back:0\n"));
		delete []pchar;
	}
	t = 0;
	tMax = 0;
	fitWindow = true;
	saveFlag = false;
	return TRUE;
}


void CImageProcessToolDoc::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	INT_PTR nRes = MessageBox(AfxGetMainWnd()->m_hWnd,_T("   确定保存？"),_T("提示"),MB_OKCANCEL|MB_ICONQUESTION);
	if(nRes == IDOK)
	{
		CStringA file(filePath.GetBuffer(0));
		filePath.ReleaseBuffer();
		string path=file.GetBuffer(0);
		file.ReleaseBuffer();
		if(!dstImg.data)	dstImg = srcImg.clone();
		imwrite(path,dstImg);
		TRACE(_T("Save Success!\n"));
		saveFlag = false;
	}
}


void CImageProcessToolDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if((!srcImg.empty())&(saveFlag == true))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolDoc::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szFilter[] = _T("JPEG文件(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG文件(*.png)|*.png|Windows位图(*.bmp;*.dib)|*.bmp;*.dib|TIFF文件(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters光栅文件(*.sr;*.ras)|*.sr;*.ras|便携文件格式(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
	CFileDialog fileDlg(FALSE,extension,fileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();
		fileName = fileDlg.GetFileTitle();
		extension = fileDlg.GetFileExt();
		CStringA file(filePath.GetBuffer(0));
		filePath.ReleaseBuffer();
		string path=file.GetBuffer(0);
		file.ReleaseBuffer();
		if(!dstImg.data)	dstImg = srcImg.clone();
		imwrite(path,dstImg);
		TRACE(_T("Save As Success!\n"));
		saveFlag = false;
	}
}


void CImageProcessToolDoc::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}
