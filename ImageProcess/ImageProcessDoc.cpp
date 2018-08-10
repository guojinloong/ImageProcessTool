
// ImageProcessDoc.cpp : CImageProcessDoc ���ʵ��
//
#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ImageProcess.h"
#endif

#include "ImageProcessDoc.h"
#include <propkey.h>
extern Mat srcImg,tempImg,dstImg,temp[];
extern CString fileName,extension;
extern CvvImage image;
extern bool fitWindow;;
//extern int t;	//��ǰ�ĳ�������

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageProcessDoc

IMPLEMENT_DYNCREATE(CImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessDoc, CDocument)

//	ON_COMMAND(ID_FILE_SAVE, &CImageProcessDoc::OnFileSave)
END_MESSAGE_MAP()


// CImageProcessDoc ����/����

CImageProcessDoc::CImageProcessDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CImageProcessDoc::~CImageProcessDoc()
{
}

//BOOL CImageProcessDoc::OnNewDocument()
//{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
//
//	// TODO: �ڴ�������³�ʼ������
//	// (SDI �ĵ������ø��ĵ�)
//
//	return TRUE;
//}




// CImageProcessDoc ���л�

void CImageProcessDoc::Serialize(CArchive& ar)
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
void CImageProcessDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CImageProcessDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CImageProcessDoc::SetSearchContent(const CString& value)
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

// CImageProcessDoc ���

#ifdef _DEBUG
void CImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL CImageProcessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	
	int num = WideCharToMultiByte(CP_OEMCP,NULL,lpszPathName,-1,NULL,0,NULL,FALSE);
	char *pchar = new char[num];
	WideCharToMultiByte (CP_OEMCP,NULL,lpszPathName,-1,pchar,num,NULL,FALSE);
	image.Load(pchar);
	srcImg = image.GetImage();
	//srcImg = imread(pchar);
	srcImg.copyTo(dstImg);
	srcImg.copyTo(tempImg);
	//t = 0;
	srcImg.copyTo(temp[0]);
	TRACE(_T("Back:0\n"));
	fitWindow = true;
	delete []pchar;
	return TRUE;
}


//BOOL CImageProcessDoc::OnSaveDocument(LPCTSTR lpszPathName)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	
//	int num = WideCharToMultiByte(CP_OEMCP,NULL,lpszPathName,-1,NULL,0,NULL,FALSE);
//	char *pchar = new char[num];
//	WideCharToMultiByte (CP_OEMCP,NULL,lpszPathName,-1,pchar,num,NULL,FALSE);
//	if(!dstImg.data)	dstImg = srcImg.clone();
//	IplImage img = dstImg;
//	image.CopyOf(&img);
//	image.Save(pchar);
//	return CDocument::OnSaveDocument(lpszPathName);
//}