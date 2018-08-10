
// ImageProcessToolDoc.cpp : CImageProcessToolDoc ���ʵ��
//
#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CImageProcessToolDoc ����/����

CImageProcessToolDoc::CImageProcessToolDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CImageProcessToolDoc::~CImageProcessToolDoc()
{
}

//BOOL CImageProcessToolDoc::OnNewDocument()
//{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
//
//	// TODO: �ڴ�������³�ʼ������
//	// (SDI �ĵ������ø��ĵ�)
//
//	return TRUE;
//}




// CImageProcessToolDoc ���л�

void CImageProcessToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CImageProcessToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CImageProcessToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CImageProcessToolDoc ���

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

	// TODO:  �ڴ������ר�õĴ�������
	
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
	// TODO: �ڴ���������������
	INT_PTR nRes = MessageBox(AfxGetMainWnd()->m_hWnd,_T("   ȷ�����棿"),_T("��ʾ"),MB_OKCANCEL|MB_ICONQUESTION);
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
	// TODO: �ڴ������������û����洦��������
	if((!srcImg.empty())&(saveFlag == true))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolDoc::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG�ļ�(*.png)|*.png|Windowsλͼ(*.bmp;*.dib)|*.bmp;*.dib|TIFF�ļ�(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters��դ�ļ�(*.sr;*.ras)|*.sr;*.ras|��Я�ļ���ʽ(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
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
	// TODO: �ڴ������������û����洦��������
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}
