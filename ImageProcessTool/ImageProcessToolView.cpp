
// ImageProcessToolView.cpp : CImageProcessToolView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ImageProcessTool.h"
#endif

#include "ImageProcessToolDoc.h"
#include "ImageProcessToolView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int boxFilterValue = 3;
int meanFilterValue = 2;
int gaussianFilterValue = 3;
int medianFilterValue = 4;
int bilateralFilterValue = 5;
int elementShape = MORPH_RECT;
int structElementSize = 3;
float pyr = 1.0;
double threshValue = 100;	//��ֵ
double maxValue = 255;	//�������ص����ֵ
int adaptiveMethod = ADAPTIVE_THRESH_MEAN_C;	//ָ��Ҫʹ�õ�����Ӧ��ֵ�㷨ADAPTIVE_THRESH_MEAN_C��ADAPTIVE_THRESH_GAUSSIAN_C
int thresholdType = THRESH_BINARY;	//��ֵ���ͣ�ȡֵΪTHRESH_BINARY��THRESH_BINARY_INV��THRESH_TRUNC��THRESH_TOZERO��THRESH_TOZERO_INV������Ӧ��ֵֻ��ȡǰ������
int blockSize = 3;	//���ڼ�������Ӧ��ֵ��С��һ�����ص�����ߴ磬ȡֵΪ3��5��7��
double C = 0;	//����Ӧ��ֵ����ȥƽ�����Ȩƽ��ֵ��ĳ���ֵͨ��Ϊ�����������������Ҳ����Ϊ�����
int cannyThreshold1 = 3;	//��һ���ͺ�����ֵ,��һ���ͺ�����ֵ�͵ڶ����ͺ�����ֵ��ֵͨ��ȡ2��1-3��1
int cannyThreshold2 = 1;	//�ڶ����ͺ�����ֵ
int apertureSize = 3;	////Sobel���ӵĿ׾���С������ȡ1��3��5��7
int ddepth = CV_16S;	//���ͼ����ȣ�ȡ-1/CV_16S/CV_32F/CV_64F
int kSize = 0;	//Sobel�˵Ĵ�С������ȡ1��3��5��7
int laplacianKernelSize = 3;	//���ڼ�����׵������˲����Ŀ׾��ߴ磬����Ϊ��������Laplacian�͵Ĵ�С��
double scale = 1;	//�����㵼��ʱ��ѡ�ģ����ű�������scale
double delta = 0;	//�ڽ������Ŀ��ͼ֮ǰ��ѡ��deltaֵ
double angle = 30.0;	//��ת�Ƕȣ�Ϊ��ֵ��ʾ����ʱ����ת������ԭ�������Ͻǣ�
int remapWays = 1;	//��ӳ�䷽ʽ��ˮƽ���񡢴�ֱ����
int interWays = INTER_LINEAR;	//��ֵ��ʽ����ȡINTER_NEAREST(����ڲ�ֵ)��INTER_LINEAR(˫���Բ�ֵ)��INTER_CUBIC(˫����������ֵ4x4)��INTER_LANCZOS4(Lanczos��ֵ8x8)
Scalar boarderValue(0,0,0);
//double scale = 0.8;	//����ϵ��
//int maxIterationNum = 10;
//int openCloseNum = 9;
//int erodeDilateNum = 9;
//int topBlackHatNum = 2;
double ViewScale = 1;
int t = 0;	//��ǰ�ĳ�������
int tMax = t;	//��ǰ�������������ֵ
//bool ok = false;
bool fitWindow = true,saveFlag = false,ok = false;	//�ʺϴ��ڱ�־,�����־,ȷ����־
//CRect lastRect;
//CSize sizeTotal;
CPoint centerPoint;
Mat srcImg,tempImg,dstImg;	//ԭʼͼ�񡢻���/��ʾͼ��Ŀ��ͼ��
Mat temp[10];	//����ͼ�񻺴�
CString filePath,fileName,extension;//�ļ�·������
vector<vector<Point>> contours;
vector<vector<Point>> hull;
vector<Vec4i> hierarchy;

// CImageProcessToolView

IMPLEMENT_DYNCREATE(CImageProcessToolView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessToolView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PROCESS_FILTER_BOX, &CImageProcessToolView::OnProcessFilterBox)
	ON_COMMAND(ID_PROCESS_FILTER_MEAN, &CImageProcessToolView::OnProcessFilterMean)
	ON_COMMAND(ID_PROCESS_FILTER_GAUSSIAN, &CImageProcessToolView::OnProcessFilterGaussian)
	ON_COMMAND(ID_PROCESS_FILTER_MEDIAN, &CImageProcessToolView::OnProcessFilterMedian)
	ON_COMMAND(ID_PROCESS_FILTER_BILATERAL, &CImageProcessToolView::OnProcessFilterBilateral)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_DILATE, &CImageProcessToolView::OnProcessMorphologyDilate)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_ERODE, &CImageProcessToolView::OnProcessMorphologyErode)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_OPEN, &CImageProcessToolView::OnProcessMorphologyOpen)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_CLOSE, &CImageProcessToolView::OnProcessMorphologyClose)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_GRADIENT, &CImageProcessToolView::OnProcessMorphologyGradient)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_TOPHAT, &CImageProcessToolView::OnProcessMorphologyTophat)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_BLACKHAT, &CImageProcessToolView::OnProcessMorphologyBlackhat)
	ON_COMMAND(ID_PROCESS_FLOODFILL, &CImageProcessToolView::OnProcessFloodfill)
	ON_COMMAND(ID_PROCESS_PYRAMID_UP, &CImageProcessToolView::OnProcessPyramidUp)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_PYRAMID_UP, &CImageProcessToolView::OnUpdateProcessPyramidUp)
	ON_COMMAND(ID_PROCESS_PYRAMID_DOWN, &CImageProcessToolView::OnProcessPyramidDown)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_PYRAMID_DOWN, &CImageProcessToolView::OnUpdateProcessPyramidDown)
	ON_COMMAND(ID_PROCESS_THRESHOLD, &CImageProcessToolView::OnProcessThreshold)
	ON_COMMAND(ID_PROCESS_THRESHOLD_ADAPTIVE, &CImageProcessToolView::OnProcessThresholdAdaptive)
	ON_COMMAND(ID_TRANSFORM_EDGE_CANNY, &CImageProcessToolView::OnTransformEdgeCanny)
	ON_COMMAND(ID_TRANSFORM_EDGE_SOBEL, &CImageProcessToolView::OnTransformEdgeSobel)
	ON_COMMAND(ID_TRANSFORM_EDGE_LAPLACIAN, &CImageProcessToolView::OnTransformEdgeLaplacian)
	ON_COMMAND(ID_TRANSFORM_EDGE_SCHARR, &CImageProcessToolView::OnTransformEdgeScharr)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_BOX, &CImageProcessToolView::OnUpdateProcessFilterBox)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_MEAN, &CImageProcessToolView::OnUpdateProcessFilterMean)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_GAUSSIAN, &CImageProcessToolView::OnUpdateProcessFilterGaussian)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_MEDIAN, &CImageProcessToolView::OnUpdateProcessFilterMedian)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_BILATERAL, &CImageProcessToolView::OnUpdateProcessFilterBilateral)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_DILATE, &CImageProcessToolView::OnUpdateProcessMorphologyDilate)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_ERODE, &CImageProcessToolView::OnUpdateProcessMorphologyErode)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_OPEN, &CImageProcessToolView::OnUpdateProcessMorphologyOpen)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_CLOSE, &CImageProcessToolView::OnUpdateProcessMorphologyClose)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_GRADIENT, &CImageProcessToolView::OnUpdateProcessMorphologyGradient)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_TOPHAT, &CImageProcessToolView::OnUpdateProcessMorphologyTophat)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_BLACKHAT, &CImageProcessToolView::OnUpdateProcessMorphologyBlackhat)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_THRESHOLD, &CImageProcessToolView::OnUpdateProcessThreshold)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_THRESHOLD_ADAPTIVE, &CImageProcessToolView::OnUpdateProcessThresholdAdaptive)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_CANNY, &CImageProcessToolView::OnUpdateTransformEdgeCanny)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_SOBEL, &CImageProcessToolView::OnUpdateTransformEdgeSobel)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_LAPLACIAN, &CImageProcessToolView::OnUpdateTransformEdgeLaplacian)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_SCHARR, &CImageProcessToolView::OnUpdateTransformEdgeScharr)
	ON_COMMAND(ID_TRANSFORM_HOUGH_HOUGHLINES, &CImageProcessToolView::OnTransformHoughHoughlines)
	ON_COMMAND(ID_TRANSFORM_HOUGH_HOUGHLINESP, &CImageProcessToolView::OnTransformHoughHoughlinesp)
	ON_COMMAND(ID_TRANSFORM_HOUGH_HOUGHCIRCLES, &CImageProcessToolView::OnTransformHoughHoughcircles)
	ON_COMMAND(ID_TRANSFORM_AFFINE_WARP, &CImageProcessToolView::OnTransformAffineWarp)
	//ON_COMMAND(ID_TRANSFORM_AFFINE_ROTATION, &CImageProcessToolView::OnTransformAffineRotation)
	ON_COMMAND(ID_TRANSFORM_REMAP, &CImageProcessToolView::OnTransformRemap)
	ON_COMMAND(ID_TRANSFORM_EQUALIZEHIST, &CImageProcessToolView::OnTransformEqualizehist)
	ON_COMMAND(ID_EDIT_UNDO, &CImageProcessToolView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CImageProcessToolView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHLINES, &CImageProcessToolView::OnUpdateTransformHoughHoughlines)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHLINESP, &CImageProcessToolView::OnUpdateTransformHoughHoughlinesp)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHCIRCLES, &CImageProcessToolView::OnUpdateTransformHoughHoughcircles)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_AFFINE_WARP, &CImageProcessToolView::OnUpdateTransformAffineWarp)
	//ON_UPDATE_COMMAND_UI(ID_TRANSFORM_AFFINE_ROTATION, &CImageProcessToolView::OnUpdateTransformAffineRotation)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_REMAP, &CImageProcessToolView::OnUpdateTransformRemap)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EQUALIZEHIST, &CImageProcessToolView::OnUpdateTransformEqualizehist)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CImageProcessToolView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CImageProcessToolView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FLOODFILL, &CImageProcessToolView::OnUpdateProcessFloodfill)
	ON_COMMAND(ID_PROCESS_RESIZE, &CImageProcessToolView::OnProcessResize)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_RESIZE, &CImageProcessToolView::OnUpdateProcessResize)
	ON_COMMAND(ID_ADVANCED_DEFOG, &CImageProcessToolView::OnAdvancedDefog)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_DEFOG, &CImageProcessToolView::OnUpdateAdvancedDefog)
	ON_COMMAND(ID_CONTOURS_FINDCONTOURS, &CImageProcessToolView::OnContoursFindcontours)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_FINDCONTOURS, &CImageProcessToolView::OnUpdateContoursFindcontours)
	ON_COMMAND(ID_CONTOURS_CONVEXHULL, &CImageProcessToolView::OnContoursConvexhull)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_CONVEXHULL, &CImageProcessToolView::OnUpdateContoursConvexhull)
	ON_COMMAND(ID_CONTOURS_POLY, &CImageProcessToolView::OnContoursPoly)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_POLY, &CImageProcessToolView::OnUpdateContoursPoly)
	ON_COMMAND(ID_EDIT_SET, &CImageProcessToolView::OnEditSet)
	ON_COMMAND(ID_EDIT_CLEAR, &CImageProcessToolView::OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CImageProcessToolView::OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_PROPERTY, &CImageProcessToolView::OnEditProperty)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTY, &CImageProcessToolView::OnUpdateEditProperty)
	ON_COMMAND(ID_CONTOURS_MOMENTS, &CImageProcessToolView::OnContoursMoments)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_MOMENTS, &CImageProcessToolView::OnUpdateContoursMoments)
	ON_COMMAND(ID_CONTOURS_WATERSHED, &CImageProcessToolView::OnContoursWatershed)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_WATERSHED, &CImageProcessToolView::OnUpdateContoursWatershed)
	ON_COMMAND(ID_ADVANCED_INPAINT, &CImageProcessToolView::OnAdvancedInpaint)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_INPAINT, &CImageProcessToolView::OnUpdateAdvancedInpaint)
	ON_COMMAND(ID_ADVANCED_STITCH, &CImageProcessToolView::OnAdvancedStitch)
	ON_COMMAND(ID_EDIT_PASTE, &CImageProcessToolView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CImageProcessToolView::OnUpdateEditPaste)
END_MESSAGE_MAP()

// CImageProcessToolView ����/����

CImageProcessToolView::CImageProcessToolView()
{
	// TODO: �ڴ˴���ӹ������
	m_pDlgSet = NULL;
	m_pDlgFilter = NULL;
	m_pDlgFloodFill = NULL;
	
	color = RGB(128,128,128);
	thickness = 1;
	lineType = 8;
	shift = 0;
	m_bDraw = false;
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
}


CImageProcessToolView::~CImageProcessToolView()
{
	if(!m_pDlgSet)	delete m_pDlgSet;
	if(!m_pDlgFilter)	delete m_pDlgFilter;
	if(!m_pDlgFloodFill)	delete m_pDlgFloodFill;
}


BOOL CImageProcessToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	//CBrush backBrush;
	//backBrush.CreateSolidBrush(RGB(255,0,255));
	//cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,0,(HBRUSH)::GetStockObject(255),0);
	return CScrollView::PreCreateWindow(cs);
}


BOOL CImageProcessToolView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	hAccel = LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(ID_EDIT_REDO));
	if(TranslateAccelerator(GetSafeHwnd(),hAccel,pMsg))
		return true;
	return CScrollView::PreTranslateMessage(pMsg);
}


BOOL CImageProcessToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect,RGB(128,128,128));
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}


//BOOL CImageProcessToolView::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CBrush backBrush;
//	backBrush.CreateSolidBrush(RGB(255,0,255));
//
//	//����ɵ�brush
//	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
//	CRect rect;
//	pDC->GetClipBox(&rect);
//
//	//����Ҫ������
//	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
//	pDC->SelectObject(pOldBrush);
//	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
//	return CScrollView::OnEraseBkgnd(pDC);
//}


// CImageProcessToolView ����

void CImageProcessToolView::OnDraw(CDC* pDC)
{
	CImageProcessToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//CClientDC myDC(this);
	//OnPrepareDC (& myDC);
	//myDC.Ellipse(60,60,200,200);

	CRect rt;
	GetClientRect(&rt);
	if((extension == _T("xml")||(extension == _T("yaml"))||(extension == _T("yml"))||(extension == _T("txt"))))
	{
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0x66,0xcc,0xff));
		pDC->TextOut(0,0,pDoc->str);
		
		//pDC->DrawText(pDoc->strings,rt,DT_CALCRECT);
		//
		//HDC hDC=pDC->GetSafeHdc();
		//DrawText(hDC,pDoc->strings,pDoc->strings.GetAllocLength(),rt,DT_CALCRECT| DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		//��������ͼ��
		if(ok == true)
		{
			if(srcImg.empty())	t = 0;
			else
			{
				saveFlag = true;
				t++;
			}
			if(t>9)
			{
				for(int i=0;i<9;i++)	temp[i]=temp[i+1];
				t=9;
			}
			tMax = t;
			tempImg.copyTo(srcImg);
			tempImg.copyTo(dstImg);
			tempImg.copyTo(temp[t]);
			ok = false;
			TRACE(_T("Back:%d\n"),t);
		}
		if((tempImg.cols > rt.Width())||(tempImg.rows > rt.Height()))
		{
			if(fitWindow == true)
			{
				float widthCoff = (float)rt.Width() / tempImg.cols;
				float heightCoff = (float)rt.Height() / tempImg.rows;
				ViewScale = widthCoff < heightCoff ? widthCoff:heightCoff;
				fitWindow = false;
			}
		}
		centerPoint = CPoint((int)(centerPoint.x/ViewScale),(int)(centerPoint.y/ViewScale));
		int imageX = (int)(tempImg.cols*ViewScale);
		int imageY = (int)(tempImg.rows*ViewScale);

		int rectL,rectT,rectR,rectB;
		//ˮƽ������ʾ����
		if(rt.Width()>imageX)
		{
			rectL = (rt.Width()-imageX)/2;
			rectR = (rt.Width()+imageX)/2;
		}
		else
		{
			rectL = 0;
			rectR = imageX;
			rt.right = imageX;
		}
		//��ֱ������ʾ����
		if(rt.Height()>imageY)
		{
			rectT = (rt.Height()-imageY)/2;
			rectB = (rt.Height()+imageY)/2;
		}
		else
		{
			rectT = 0;
			rectB = imageY;
			rt.bottom = imageY;
		}
		CRect rect(rectL,rectT,rectR,rectB);
		
		//CDC memDC;
		//CBitmap memBitmap;

		//memDC.CreateCompatibleDC(pDC);
		//memBitmap.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());

		//memDC.SelectObject(&memBitmap);
		//memDC.FillSolidRect(&rt,color);
		//if(tempImg.empty())
		//{
		//	memDC.MoveTo(rt.left,rt.top);
		//	memDC.LineTo(rt.right,rt.bottom);
		//	memDC.MoveTo(rt.right,rt.top);
		//	memDC.LineTo(rt.left,rt.bottom);
		//}
		//��ͼ
		//Gdiplus::Graphics g(memDC.m_hDC);
		//Gdiplus::Bitmap bitmap(tempImg.cols,tempImg.rows,tempImg.cols*tempImg.channels(),PixelFormat24bppRGB,(BYTE*)tempImg.data);//����Mat���󴴽�һ��GDI+�е�Bitmapλͼ
		//Gdiplus::Image *img = &bitmap;
		////Gdiplus::Image *img = Gdiplus::Image::FromFile(filePath);
		//g.DrawImage(img, Gdiplus::Rect(rect.left,rect.top,rect.Width(),rect.Height()));
		//g.ReleaseHDC(memDC.m_hDC);

		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&memDC,0,0,SRCCOPY);
		//memDC.DeleteDC();
		//memBitmap.DeleteObject();

		CSize sizeTotal(imageX,imageY);
		SetScrollSizes(MM_TEXT,sizeTotal);
		HDC hDC=pDC->GetSafeHdc();
		pDoc->image.CopyOf(&(IplImage)tempImg);
		pDoc->image.DrawToHDC(hDC,&rect);
	}
}


void CImageProcessToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BOOL CImageProcessToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!srcImg.empty())
	{
		if(zDelta > 0)
		{
			if(ViewScale+0.05 < 5)	ViewScale = ViewScale + 0.05;
		}
		else if(zDelta < 0)
		{
			if(ViewScale-0.02 > 0 )		ViewScale = ViewScale - 0.02;
		}
		else  return CView::OnMouseWheel(nFlags, zDelta, pt);
		//centerPoint = pt;
		Invalidate(TRUE);
		UpdateWindow();
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


// CImageProcessToolView ��ӡ


void CImageProcessToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageProcessToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageProcessToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CImageProcessToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessToolView ���

#ifdef _DEBUG
void CImageProcessToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessToolDoc* CImageProcessToolView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessToolDoc)));
	return (CImageProcessToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessToolView ��Ϣ�������

void CImageProcessToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!srcImg.empty())
	{
		centerPoint = point;
		m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
		m_bDraw = true;
	}
	else
	{
		m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_NO);
		m_bDraw = false;
	}
	m_pOld = point;
	SetCursor(m_hCursor);
	SetCapture();
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	ClipCursor(rect);
	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageProcessToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bDraw = false;
	ReleaseCapture();
	ClipCursor(NULL);
	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageProcessToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bDraw)
	{
		CClientDC dc(this);
		dc.MoveTo(m_pOld);
		dc.LineTo(point);
		m_pOld = point;
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CImageProcessToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CMenu menu;
	//CMenu *pSubMenu;
	//menu.LoadMenu(IDR_POPUP_EDIT);
	//pSubMenu=menu.GetSubMenu(0);
	//ClientToScreen(&point);
	//pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	CScrollView::OnRButtonDown(nFlags, point);
}


void CImageProcessToolView::OnEditUndo()
{
	// TODO: �ڴ���������������
	saveFlag = true;
	t--;
	temp[t].copyTo(srcImg);
	temp[t].copyTo(dstImg);
	temp[t].copyTo(tempImg);
	if(t<0)	t=0;
	TRACE(_T("Back:%d\n"),t);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if((!srcImg.empty())&&(t>0))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnEditRedo()
{
	// TODO: �ڴ���������������
	if(t<tMax)
	{
		saveFlag = true;
		t++;
		temp[t].copyTo(srcImg);
		temp[t].copyTo(dstImg);
		temp[t].copyTo(tempImg);
		//if(t<0)	t=0;
		TRACE(_T("Back:%d\n"),t);
		Invalidate(TRUE);
		UpdateWindow();
	}
}


void CImageProcessToolView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if((!srcImg.empty())&&(t<tMax))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnEditPaste()
{
	// TODO: �ڴ���������������
	system("start explorer http://www.csdn.net"); 
}


void CImageProcessToolView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(false);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnEditClear()
{
	// TODO: �ڴ���������������
	if(contours.size() == 0)
	{
		MessageBox(_T("û�п�����Ļ��棡"),_T("��ʾ"),MB_OK);
		return;
	}
	//vector<vector<Point>>::iterator it;
	//vector<Point> point;
	//vector<Point>::iterator it;
	//for(unsigned int i=0;i<contours.size();i++)
	//{
	//	point = contours[i];
	//	for(it = point.begin();it!=point.end();it++)
	//		it = point.erase(it);
	//	point.clear();
	//	vector<Point>(point).swap(point);
	//}
	//for(it = contours.begin();it != contours.end();it++)
	//{
	//	it = contours.erase(it);
	//}
	//contours.clear();
	//vector<vector<Point>>(contours).swap(contours);
	//hull.clear();
	//vector<vector<Point>>(hull).swap(hull);
	//hierarchy.resize(0);
	//hierarchy.clear();
	//vector<Vec4i>(hierarchy).swap(hierarchy);
	//while(contours.capacity()!=0);
	if(contours.size() == 0)
	{
		MessageBox(_T("������ϣ�"),_T("��ʾ"),MB_OK);
		return;
	}
}


void CImageProcessToolView::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnEditProperty()
{
	// TODO: �ڴ���������������
	CDlgProperty dlg;
	if(dlg.DoModal() == IDOK)
	{
		ok = true;
		Mat img(srcImg.size(),CV_MAKETYPE(dlg.m_depth,dlg.m_channels));
		tempImg.copyTo(img);
		tempImg.release();
		tempImg = img;
		//tempImg = Mat(srcImg.size(),CV_MAKETYPE(dlg.m_depth,dlg.m_channels));
		//tempImg = srcImg.clone();
		//tempImg.copyTo(img);
		//img.copyTo(tempImg);
		//img.release();
		//srcImg.reshape(,);
		//cvtColor(srcImg,srcImg,CV_MAKETYPE(m_depth,m_channels));
		Invalidate(TRUE);
		UpdateWindow();
	}
}


void CImageProcessToolView::OnUpdateEditProperty(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnEditSet()
{
	// TODO: �ڴ���������������
	//CDlgSet m_dlgSet;//(thickness,lineType,shift);
	//LOGFONT lf;
	//font.GetLogFont(&lf);
	//m_dlgSet.m_font.CreateFontIndirectW(&lf);
	INT_PTR nRes = m_dlgSet.DoModal();
	if(nRes == IDOK)
	{
		thickness =m_dlgSet.m_thickness;
		lineType = m_dlgSet.m_lineType;
		shift = m_dlgSet.m_shift;
		color = m_dlgSet.m_color;//RGB(m_dlgSet.m_colorR,m_dlgSet.m_colorG,m_dlgSet.m_colorB);
		//LOGFONT lf;
		//m_dlgSet.m_font.GetLogFont(&lf);
		//font.CreateFontIndirectW(&lf);
		Invalidate(TRUE);
		UpdateWindow();
	}
	//dlg.m_thickness = thickness;
	//dlg.m_lineType = lineType;
	//dlg.m_shift = shift;
	//if(m_pDlgSet == NULL)
	//{
	//	m_pDlgSet = new CDlgSet();
	//	m_pDlgSet->Create(IDD_DIALOG_SET,this);
	//}
	//INT_PTR nRes = m_pDlgSet->ShowWindow(SW_SHOW);
	//if(nRes == IDOK)
	//{
	//	thickness = m_pDlgSet->m_thickness;
	//	lineType = m_pDlgSet->m_lineType;
	//	shift = m_pDlgSet->m_shift;
	//	color = m_pDlgSet->m_color;
	//	LOGFONT lf;
	//	m_pDlgSet->m_font.GetLogFont(&lf);
	//	font.CreateFontIndirectW(&lf);
	//	Invalidate(TRUE);
	//	UpdateWindow();
	//}
}


void CImageProcessToolView::OnProcessFilterBox()
{
	// TODO: �ڴ���������������
	//if(m_pDlgFilter == NULL)
	//{
	//	m_pDlgFilter = new CDlgFilter();
	//	m_pDlgFilter->Create(IDD_DIALOG_FILTER_BOX,this);
	//	m_pDlgFilter->m_pMain = (void *)this;
	//}
	//m_pDlgFilter->ShowWindow(SW_SHOW);
	//boxFilterValue = m_pDlgFilter->m_filterValue;
	CDlgFilter dlg;
	dlg.filterWays = 0;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//boxFilterValue = dlg.m_filterValue;
	//boxFilter(srcImg,tempImg,-1,Size(boxFilterValue+1,boxFilterValue+1));
	//imshow("�������˲���",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessFilterBox(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessFilterMean()
{
	// TODO: �ڴ���������������
	CDlgFilter dlg;
	dlg.filterWays = 1;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//meanFilterValue = dlg.m_filterValue;
	//blur(srcImg,tempImg,Size(meanFilterValue+1,meanFilterValue+1));
	//imshow("����ֵ�˲���",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessFilterMean(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessFilterGaussian()
{
	// TODO: �ڴ���������������
	CDlgFilter dlg;
	dlg.filterWays = 2;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//gaussianFilterValue = dlg.m_filterValue;
	//GaussianBlur(srcImg,tempImg,Size(gaussianFilterValue*2+1,gaussianFilterValue*2+1),0,0);
	//imshow("����˹�˲���",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessFilterMedian()
{
	// TODO: �ڴ���������������
	CDlgFilter dlg;
	dlg.filterWays = 3;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//medianFilterValue = dlg.m_filterValue;
	//medianBlur(srcImg,tempImg,medianFilterValue*2+1);
	//imshow("����ֵ�˲���",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessFilterMedian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessFilterBilateral()
{
	// TODO: �ڴ���������������
	CDlgFilter dlg;
	dlg.filterWays = 4;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//bilateralFilterValue = dlg.m_filterValue;
	//bilateralFilter(srcImg,tempImg,bilateralFilterValue,bilateralFilterValue*2,bilateralFilterValue/2);
	//imshow("��˫���˲���",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessFilterBilateral(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyDilate()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 0;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//dilate(srcImg,tempImg,element);
	////imshow("�����͡�",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyDilate(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyErode()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 1;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//erode(srcImg,tempImg,element);
	////imshow("����ʴ��",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyErode(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyOpen()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 2;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_OPEN,element);
	////imshow("�������㡿",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyOpen(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyClose()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 3;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_CLOSE,element);
	////imshow("�������㡿",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyClose(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyGradient()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 4;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_GRADIENT,element);
	////imshow("���ݶ����㡿",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyGradient(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyTophat()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 5;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_TOPHAT,element);
	////imshow("����ñ���㡿",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyTophat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessMorphologyBlackhat()
{
	// TODO: �ڴ���������������
	CDlgMorphology dlg;
	dlg.m_morphWays = 6;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_BLACKHAT,element);
	////imshow("����ñ���㡿",tempImg);
	//element.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessMorphologyBlackhat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CImageProcessToolView::OnProcessPyramidUp()
{
	// TODO: �ڴ���������������
	ok = true;
	pyrUp(tempImg,tempImg,Size(tempImg.cols*2,tempImg.rows*2));
	//imshow("����������",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessPyramidUp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessPyramidDown()
{
	// TODO: �ڴ���������������
	ok = true;
	pyrDown(tempImg,tempImg,Size(tempImg.cols/2,tempImg.rows/2));
	//imshow("����������",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessPyramidDown(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessResize()
{
	// TODO: �ڴ���������������
	CDlgResize dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//resize(srcImg,tempImg,Size(dlg.m_width,dlg.m_height),0,0,dlg.m_interWays);
	//imshow("�ߴ����",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}



void CImageProcessToolView::OnUpdateProcessResize(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessFloodfill()
{
	// TODO: �ڴ���������������
	if(m_pDlgFloodFill == NULL)
	{
		m_pDlgFloodFill = new CDlgFloodFill();
		m_pDlgFloodFill->Create(IDD_DIALOG_FLOODFILL,this);
	}
	m_pDlgFloodFill->ShowWindow(SW_SHOW);

	//CDlgFloodFill dlg;
	//INT_PTR nRes = dlg.DoModal();
	//if(nRes == IDOK)	ok = true;
	//else return;
	//int flags = dlg.m_connectivity|dlg.m_fillMode;
	//
	//Mat mask;
	//mask.create(srcImg.rows+2,srcImg.cols+2,CV_8UC1);
	//Point seed(centerPoint.x,centerPoint.y);
	//Rect ccomp;
	//int area;
	//if(dlg.m_bMask)
	//{
	//	if(dlg.m_bMask == true)	flags = flags|FLOODFILL_MASK_ONLY|(dlg.m_maskValue<<8);
	//	threshold(mask,mask,1,128,THRESH_BINARY);
	//	area = floodFill(srcImg,mask,seed,Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR),&ccomp,Scalar(dlg.m_lowDiff,dlg.m_lowDiff,dlg.m_lowDiff),Scalar(dlg.m_upDiff,dlg.m_upDiff,dlg.m_upDiff),flags);
	//	tempImg = mask.clone();
	//	//imshow("Mask",maskImg);
	//}
	//else floodFill(tempImg,seed,Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR),&ccomp,Scalar(dlg.m_lowDiff,dlg.m_lowDiff,dlg.m_lowDiff),Scalar(dlg.m_upDiff,dlg.m_upDiff,dlg.m_upDiff),flags);
	////imshow("��Ч��ͼ��",dstImg);
	//mask.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessFloodfill(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessThreshold()
{
	// TODO: �ڴ���������������
	CDlgThreshold dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//threshValue = dlg.m_threshold;
	//maxValue = dlg.m_maxValue;
	//thresholdType = dlg.m_type;
	//threshold(srcImg,tempImg,threshValue,maxValue,thresholdType);
	//imshow("����ֵ����",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessThreshold(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnProcessThresholdAdaptive()
{
	// TODO: �ڴ���������������
	CDlgThresholdAdaptive dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//blockSize = dlg.m_blockSize;
	//maxValue = dlg.m_maxValue;
	//adaptiveMethod = dlg.m_type1;
	//thresholdType = dlg.m_type2;
	//Mat grayImg;
	//cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
	////imshow("���Ҷ�ͼ��",grayImg);
	//adaptiveThreshold(grayImg,tempImg,maxValue,adaptiveMethod,thresholdType,blockSize*2+1,C);
	////imshow("������Ӧ��ֵ����",tempImg);
	//grayImg.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateProcessThresholdAdaptive(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformEdgeCanny()
{
	// TODO: �ڴ���������������
	CDlgCanny dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//cannyThreshold1 = dlg.m_cannyThreshold1;
	//cannyThreshold2 = dlg.m_cannyThreshold2;
	//apertureSize = dlg.m_type;
	//Mat mask;
	//blur(srcImg,tempImg,Size(3,3));
	//Canny(tempImg,mask,cannyThreshold1,cannyThreshold2,apertureSize);
	//tempImg=Scalar::all(0);
	//srcImg.copyTo(tempImg,mask);
	////imshow("��Canny��Ե��⡿",tempImg);
	//mask.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformEdgeCanny(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformEdgeSobel()
{
	// TODO: �ڴ���������������
	CDlgSobel dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	//Sobel(srcImg,tempImg,dlg.m_depth,dlg.m_dX,dlg.m_dY,dlg.m_kSize,dlg.m_scale,dlg.m_delta);

	ddepth = dlg.m_depth;
	kSize = dlg.m_kSize;
	scale = dlg.m_scale;
	delta = dlg.m_delta;
	Mat sobelX,sobelY,sobelXAbs,sobelYAbs;
	Sobel(srcImg,sobelX,ddepth,1,0,kSize*2+1,scale,delta);
	convertScaleAbs(sobelX,sobelXAbs);

	Sobel(srcImg,sobelY,ddepth,0,1,kSize*2+1,scale,delta);
	convertScaleAbs(sobelY,sobelYAbs);

	addWeighted(sobelXAbs,0.5,sobelYAbs,0.5,0,tempImg);
	//imshow("��Sobel��Ե��⡿",tempImg);
	sobelX.release();
	sobelY.release();
	sobelXAbs.release();
	sobelYAbs.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformEdgeSobel(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformEdgeLaplacian()
{
	// TODO: �ڴ���������������
	CDlgLaplacian dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	Mat grayImg;
	cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
	//imshow("���Ҷ�ͼ��",grayImg);
	GaussianBlur(grayImg,grayImg,Size(3,3),0,0);
	Laplacian(grayImg,tempImg,dlg.m_depth,dlg.m_kSize,dlg.m_scale,dlg.m_delta);
	//imshow("��Laplacian��Ե��⡿",tempImg);
	grayImg.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformEdgeLaplacian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformEdgeScharr()
{
	// TODO: �ڴ���������������
	CDlgScharr dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	//Scharr(srcImg,tempImg,dlg.m_depth,dlg.m_dX,dlg.m_dY,dlg.m_scale,dlg.m_delta);

	ddepth = dlg.m_depth;
	scale = dlg.m_scale;
	delta = dlg.m_delta;
	Mat scharrX,scharrY,scharrXAbs,scharrYAbs;
	Scharr(srcImg,scharrX,ddepth,1,0,scale,delta);
	convertScaleAbs(scharrX,scharrXAbs);

	Scharr(srcImg,scharrY,ddepth,0,1,scale,delta);
	convertScaleAbs(scharrY,scharrYAbs);

	addWeighted(scharrXAbs,0.5,scharrYAbs,0.5,0,tempImg);
	//imshow("��Scharr��Ե��⡿",tempImg);
	scharrX.release();
	scharrY.release();
	scharrXAbs.release();
	scharrYAbs.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformEdgeScharr(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformHoughHoughlines()
{
	// TODO: �ڴ���������������
	CDlgHoughLines dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	Mat midImg;
	Canny(srcImg,midImg,50,200,3);
	cvtColor(midImg,tempImg,CV_GRAY2BGR);
	vector<Vec2f> lines;
	HoughLines(midImg,lines,dlg.m_rho,CV_PI/180*dlg.m_theta,dlg.m_threshold,dlg.m_srn,dlg.m_stn);
	for(unsigned int i=0;i<lines.size();i++)
	{
		float rho=lines[i][0],theta=lines[i][1];
		Point pt1,pt2;
		float a=cos(theta),b=sin(theta);
		float x0=a*rho,y0=b*rho;
		pt1.x=cvRound(x0+1000*(-b));
		pt1.y=cvRound(y0+1000*(a));
		pt2.x=cvRound(x0-1000*(-b));
		pt2.y=cvRound(y0-1000*(a));
		line(tempImg,pt1,pt2,Scalar(0,0,255),thickness,lineType,shift);
	}
	//imshow("����׼����任��",dst);
	midImg.release();
	//vector<Vec2f>(lines).swap(lines);
	//lines.clear();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformHoughHoughlines(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformHoughHoughlinesp()
{
	// TODO: �ڴ���������������
	CDlgHoughLinesP dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	Mat midImg;
	Canny(srcImg,midImg,50,200,3);
	cvtColor(midImg,tempImg,CV_GRAY2BGR);
	vector<Vec4i> lines;
	HoughLinesP(midImg,lines,dlg.m_rho,CV_PI/180*dlg.m_theta,dlg.m_threshold,dlg.m_maxLineLength,dlg.m_maxLineGap);
	for(unsigned int i=0;i<lines.size();i++)
	{
		Vec4i l=lines[i];
		line(tempImg,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,255,20),thickness,lineType,shift);
	}
	//imshow("���ۼƻ���任��",dst);
	midImg.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformHoughHoughlinesp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformHoughHoughcircles()
{
	// TODO: �ڴ���������������
	Mat grayImg,blurImg,dst;
	cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
	GaussianBlur(grayImg,blurImg,Size(3,3),2,2);
	vector<Vec3f> circles;
	HoughCircles(blurImg,circles,CV_HOUGH_GRADIENT,1.5,10,200,100,0,0);
	for(unsigned int i=0;i<circles.size();i++)
	{
		Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
		int radius=cvRound(circles[i][2]);
		circle(dst,center,3,Scalar(0,255,255),thickness,8,0);
		circle(dst,center,radius,Scalar(255,0,0),thickness,lineType,shift);
	}
	//imshow("������Բ�任��",dst);
	grayImg.release();
	blurImg.release();
	dst.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformHoughHoughcircles(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformAffineWarp()
{
	// TODO: �ڴ���������������
	CDlgAffineWarp dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	//interWays = dlg.m_interWays;
	//boarderValue = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
	//tempImg=Mat::zeros(srcImg.rows,srcImg.cols,srcImg.type());
	Mat warpMat(2,3,CV_32FC1);
	if(dlg.m_affineWays == 0)
	{
		Point2f srcTriangle[3];
		Point2f dstTriangle[3];
		srcTriangle[0]=Point2f(0,0);
		srcTriangle[1]=Point2f((float)srcImg.cols-1,0);
		srcTriangle[2]=Point2f(0,(float)srcImg.rows-1);
		dstTriangle[0]=Point2f(dlg.m_point[0].x/dlg.m_viewScale,dlg.m_point[0].y/dlg.m_viewScale);
		dstTriangle[1]=Point2f(dlg.m_point[1].x/dlg.m_viewScale,dlg.m_point[1].y/dlg.m_viewScale);
		dstTriangle[2]=Point2f(dlg.m_point[2].x/dlg.m_viewScale,dlg.m_point[2].y/dlg.m_viewScale);
		if(dlg.m_affineDirection == 0)	warpMat=getAffineTransform(dstTriangle,srcTriangle);
		else if(dlg.m_affineDirection == 1)	warpMat=getAffineTransform(srcTriangle,dstTriangle);
		//int minX,maxX,minY,maxY;
		//if(dlg.m_point[0].x<dlg.m_point[1].x)
		//{
		//	if(dlg.m_point[0].x<dlg.m_point[2].x)	minX = dlg.m_point[0].x;
		//	else minX = dlg.m_point[2].x;
		//}
		//else
		//{
		//	if(dlg.m_point[0].x>dlg.m_point[2].x)	maxX = dlg.m_point[0].x;
		//	else maxX = dlg.m_point[2].x;
		//}
		//if(dlg.m_point[0].y<dlg.m_point[1].y)
		//{
		//	if(dlg.m_point[0].y<dlg.m_point[2].y)	minY = dlg.m_point[0].y;
		//	else minY = dlg.m_point[2].y;
		//}
		//else
		//{
		//	if(dlg.m_point[0].y>dlg.m_point[2].y)	maxY = dlg.m_point[0].y;
		//	else maxY = dlg.m_point[2].y;
		//}
	}
	else if(dlg.m_affineWays == 1)
	{
		double angle;
		if(dlg.m_affineDirection == 0)	angle = -dlg.m_angle;
		else if(dlg.m_affineDirection == 1)	angle = dlg.m_angle;
		Point2f anchor(dlg.m_anchor.x/dlg.m_viewScale,dlg.m_anchor.y/dlg.m_viewScale);
		warpMat=getRotationMatrix2D(anchor,angle,dlg.m_scale);
	}
	warpAffine(srcImg,tempImg,warpMat,Size(),dlg.m_interWays,0,Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR));
	//imshow("��Warp��",tempImg);
	warpMat.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformAffineWarp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformRemap()
{
	// TODO: �ڴ���������������
	CDlgRemap dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	remapWays = dlg.m_remapWays;
	interWays = dlg.m_interWays;
	boarderValue = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
	Mat mapx;
	Mat mapy;
	mapx.create(srcImg.size(),CV_32FC1);
	mapy.create(srcImg.size(),CV_32FC1);
	tempImg.create(srcImg.size(),CV_32FC1);
	for(int j=0;j<srcImg.rows;j++)
	{
		for(int i=0;i<srcImg.cols;i++)
		{
			switch(remapWays)
			{
			case 3:if(i>srcImg.cols*0.25&&i<srcImg.cols*0.75&&j>srcImg.rows*0.25&&j<srcImg.rows*0.75)
						{
							mapx.at<float>(j,i)=static_cast<float>(2*(i-srcImg.cols*0.25)+0.5);
							mapy.at<float>(j,i)=static_cast<float>(2*(j-srcImg.rows*0.25)+0.5);
						}
						else
						{
							mapx.at<float>(j,i)=0;
							mapy.at<float>(j,i)=0;
						}
						break;
			case 1:
				mapx.at<float>(j,i)=static_cast<float>(i);
				mapy.at<float>(j,i)=static_cast<float>(srcImg.rows-j-1);
				break;
			case 0:
				mapx.at<float>(j,i)=static_cast<float>(srcImg.cols-i-1);
				mapy.at<float>(j,i)=static_cast<float>(j);
				break;
			case 2:
				mapx.at<float>(j,i)=static_cast<float>(srcImg.cols-i-1);
				mapy.at<float>(j,i)=static_cast<float>(srcImg.rows-j-1);
				break;
			}
		}
	}
	remap(srcImg,tempImg,mapx,mapy,interWays,BORDER_CONSTANT,boarderValue);
	//imshow("����ӳ�䡿",tempImg);
	mapx.release();
	mapy.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformRemap(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnTransformEqualizehist()
{
	// TODO: �ڴ���������������
	Mat gray,dst;
	cvtColor(srcImg,gray,COLOR_BGR2GRAY);
	//dst.create(srcImg.size(),CV_8UC1);
	//tempImg.release();
	equalizeHist(gray,dst);
	tempImg = dst;
	ok = true;
	//imshow("��ֱ��ͼ���⻯��",tempImg);
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateTransformEqualizehist(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnContoursFindcontours()
{
	// TODO: �ڴ���������������
	CDlgFindContours dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	Mat img,dst;
	FileStorage fs;
	cvtColor(srcImg,img,COLOR_BGR2GRAY);
	//imshow("Gray",img);
	//threshold(img,img,127,255,THRESH_BINARY);
	//imshow("Threshold",img);
	//Canny(img,img,119,255,3);
	//imshow("Canny",img);
	//contours.clear();
	//vector<vector<Point>>(contours).swap(contours);
	//hierarchy.clear();
	//vector<Vec4i>(hierarchy).swap(hierarchy);
	//_ASSERTE(_CrtCheckMemory());
	findContours(img,contours,dlg.m_mode,dlg.m_method);
	dst = Mat::zeros(srcImg.size(),CV_8UC3);
	for(unsigned int index=0;index<contours.size();index++)
	{
		Scalar color;
		if(dlg.m_bRandColor == true)	color = Scalar(rand()&255,rand()&255,rand()&255);
		else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
		drawContours(dst,contours,index,color,thickness,lineType,hierarchy);
	}
	//imshow("Draw",dst);waitKey(0);
	tempImg = dst;
	img.release();
	dst.release();
	Invalidate(TRUE);
	UpdateWindow();

	TCHAR szFilter[] = _T("XML�ļ�(*.xml)|*.xml|YAML�ļ�(*.yaml;*.yml)|*.yaml;*.yml|TXT�ļ�(*.txt)|*.txt||");
	CFileDialog fileDlg(FALSE,_T("xml"),_T("contours"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	nRes = fileDlg.DoModal();
	if(nRes == IDOK)
	{
		CString p = fileDlg.GetPathName();
		CStringA file(p.GetBuffer(0));
		p.ReleaseBuffer();
		string path=file.GetBuffer(0);
		file.ReleaseBuffer();
		fs.open(path,FileStorage::WRITE);
		//time_t rawtime;
		//time(&rawtime);
		//fs<<"makeDate"<<asctime(localtime(&rawtime));
		fs<<"contours"<<"[";
		for(unsigned int i = 0;i<contours.size();i++)
		{
			//CString id;
			//id.Format(_T("[:��%d������"),i);
			fs<<"points"<<"[:";
			for(unsigned int j=0;j<contours[i].size();j++)
				fs<<contours[i][j].x<<contours[i][j].y;
			fs<<"]";
		}
		fs<<"]";
		fs.release();
		CString f;
		f.Format(_T("�ļ��ѱ��浽%s\n\t�Ƿ�򿪣�"),p);
		nRes = MessageBox(f,_T("��ʾ"),MB_YESNO);
		if(nRes == IDYES)
		{
			
		}
	}
}


void CImageProcessToolView::OnUpdateContoursFindcontours(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnContoursConvexhull()
{
	// TODO: �ڴ���������������
	CDlgConvexHull dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	if(dlg.m_select)
	{
		if(contours.size() == 0)
		{
			MessageBox(_T("���Ƚ��С�����������������"),_T("��ʾ"),MB_OK);
			return;
		}
	}
	//Mat img,dst;
	//cvtColor(srcImg,img,COLOR_BGR2GRAY);
	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;
	FileStorage fs;
	//fs.open(path,FileStorage::READ);
	//FileNode fn = fs["contours"];
	//FileNodeIterator it = fn.begin(),it_end = fn.end();
	//for(int i=0;it!=it_end;it++,i++)
	//{
	//	vector<int> point;
	//	(*it)["points"]>>point;
	//	for(int j=0;j<point.size()/2;j++)
	//	{
	//		contours[i][j].x = point[j*2];
	//		contours[i][j].y = point[j*2+1];
	//	}
	//}
	//imshow("Gray",img);
	//threshold(img,img,127,255,THRESH_BINARY);
	//imshow("Threshold",img);
	//Canny(img,img,119,255,3);
	//imshow("Canny",img);
	//findContours(img,contours,dlg.m_mode,dlg.m_method);
	vector<vector<Point>>(hull).swap(hull);
	hull.clear();
	hull.resize(contours.size());
	for(unsigned int i = 0;i<contours.size();i++)	convexHull(Mat(contours[i]),hull[i],dlg.m_direction);
	Mat dst = Mat::zeros(srcImg.size(),CV_8UC3);
	for(unsigned int index=0;index<contours.size();index++)
	{
		Scalar color;
		if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
		else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
		//drawContours(dst,contours,index,color,CV_FILLED,8,hierarchy);
		drawContours(dst,hull,index,color,thickness,lineType,hierarchy);
	}
	//imshow("Draw",dst);waitKey(0);
	tempImg = dst;
	//img.release();
	dst.release();
	Invalidate(TRUE);
	UpdateWindow();

	TCHAR szFilter[] = _T("XML�ļ�(*.xml)|*.xml|YAML�ļ�(*.yaml;*.yml)|*.yaml;*.yml|TXT�ļ�(*.txt)|*.txt||");
	CFileDialog fileDlg(FALSE,_T("xml"),_T("hull"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	nRes = fileDlg.DoModal();
	if(nRes == IDOK)
	{
		CString p = fileDlg.GetPathName();
		CStringA file(p.GetBuffer(0));
		p.ReleaseBuffer();
		string path=file.GetBuffer(0);
		file.ReleaseBuffer();
		fs.open(path,FileStorage::WRITE);
		//time_t rawtime;
		//time(&rawtime);
		//fs<<"makeDate"<<asctime(localtime(&rawtime));
		fs<<"hull"<<"[";
		for(unsigned int i = 0;i<contours.size();i++)
		{
			//CString id;
			//id.Format(_T("[:��%d������"),i);
			fs<<"points"<<"[:";
			for(unsigned int j=0;j<hull[i].size();j++)
				fs<<"x"<<hull[i][j].x<<"y"<<hull[i][j].y;
			fs<<"]";
		}
		fs<<"]";
		fs.release();
		CString f;
		f.Format(_T("�ļ��ѱ��浽%s\n\t�Ƿ�򿪣�"),p);
		nRes = MessageBox(f,_T("��ʾ"),MB_YESNO);
		if(nRes == IDYES)
		{
			
		}
	}
	//vector<vector<Point>>(hull).swap(hull);
	//hull.clear();
}


void CImageProcessToolView::OnUpdateContoursConvexhull(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnContoursPoly()
{
	// TODO: �ڴ���������������
	if((contours.size() == 0)&(hull.size() == 0))
	{
		MessageBox(_T("���Ȳ���������͹����"),_T("��ʾ"),MB_OK);
		return;
	}
	CDlgPoly dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	vector<Rect> rect;
	vector<RotatedRect> rRect;
	vector<RotatedRect> rEll;
	Point2f vertex[4];
	vector<Point2f> center;
	vector<float> radius;
	Mat dst = Mat::zeros(srcImg.size(),CV_8UC3);
	if(contours.size()>0)
	{
		rect.resize(contours.size());
		rRect.resize(contours.size());
		rEll.resize(contours.size());
		center.resize(contours.size());
		radius.resize(contours.size());
		Scalar color;
		if(dlg.m_select1)
		{
			for(unsigned int index=0;index<contours.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				rect[index] = boundingRect(contours[index]);
				rectangle(dst,rect[index].tl(),rect[index].br(),color,thickness,lineType,shift);
			}
		}
		else if(dlg.m_select2)
		{
			for(unsigned int index=0;index<contours.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				rRect[index] = minAreaRect(contours[index]);
				rRect[index].points(vertex);
				for(int i=0;i<4;i++)
				{
					line(dst,vertex[i],vertex[(i+1)%4],color,thickness,lineType,shift);
				}
				//rect[index] = rRect[index].boundingRect();
				//rectangle(dst,rect[index].tl(),rect[index].br(),color,2,8,0);
			}
		}
		else if(dlg.m_select3)
		{
			for(unsigned int index=0;index<contours.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				minEnclosingCircle(contours[index],center[index],radius[index]);
				circle(dst,center[index],(int)radius[index],color,thickness,lineType,shift);
			}
		}
		else if(dlg.m_select4)
		{
			for(unsigned int index=0;index<contours.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				rEll[index] = fitEllipse(contours[index]);
				ellipse(dst,rEll[index].center,rEll[index].size,rEll[index].angle,0,360,color,thickness,lineType,shift);
			}
		}
		else return;
	}

	if(hull.size()>0)
	{
		rect.resize(hull.size());
		rRect.resize(hull.size());
		rEll.resize(hull.size());
		center.resize(hull.size());
		radius.resize(hull.size());
		Scalar color;
		if(dlg.m_select1)
		{
			for(unsigned int index=0;index<hull.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				rect[index] = boundingRect(hull[index]);
				rectangle(dst,rect[index].tl(),rect[index].br(),color,thickness,lineType,shift);
			}
		}
		else if(dlg.m_select2)
		{
			for(unsigned int index=0;index<hull.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				rRect[index] = minAreaRect(hull[index]);
				rRect[index].points(vertex);
				for(int i=0;i<4;i++)
				{
					line(dst,vertex[i],vertex[(i+1)%4],color,thickness,lineType,shift);
				}
				//rect[index] = rRect[index].boundingRect();
				//rectangle(dst,rect[index].tl(),rect[index].br(),color,2,8,0);
			}
		}
		else if(dlg.m_select3)
		{
			for(unsigned int index=0;index<hull.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				minEnclosingCircle(hull[index],center[index],radius[index]);
				circle(dst,center[index],(int)radius[index],color,thickness,lineType,shift);
			}
		}
		else if(dlg.m_select4)
		{
			for(unsigned int index=0;index<hull.size();index++)
			{
				if(dlg.m_bRandColor)	color = Scalar(rand()&255,rand()&255,rand()&255);
				else color = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
				rEll[index] = fitEllipse(hull[index]);
				ellipse(dst,rEll[index].center,rEll[index].size,rEll[index].angle,0,360,color,thickness,lineType,shift);
			}
		}
		else return;
	}
	//imshow("Draw",dst);waitKey(0);
	tempImg = dst;
	dst.release();
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateContoursPoly(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnContoursMoments()
{
	// TODO: �ڴ���������������
}


void CImageProcessToolView::OnUpdateContoursMoments(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnContoursWatershed()
{
	// TODO: �ڴ���������������
}


void CImageProcessToolView::OnUpdateContoursWatershed(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnAdvancedDefog()
{
	// TODO: �ڴ���������������
	CDlgDefog dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else
	{
		tempImg = srcImg.clone();
		return;
	}
	Invalidate(TRUE);
	UpdateWindow();
}


void CImageProcessToolView::OnUpdateAdvancedDefog(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnAdvancedInpaint()
{
	// TODO: �ڴ���������������
}


void CImageProcessToolView::OnUpdateAdvancedInpaint(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessToolView::OnAdvancedStitch()
{
	// TODO: �ڴ���������������
	TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG�ļ�(*.png)|*.png|Windowsλͼ(*.bmp;*.dib)|*.bmp;*.dib|TIFF�ļ�(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters��դ�ļ�(*.sr;*.ras)|*.sr;*.ras|��Я�ļ���ʽ(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
	CFileDialog fileDlg(TRUE,extension,fileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT|OFN_ENABLEHOOK,szFilter);
	TCHAR *pBuffer = new TCHAR[100*20];
	//for(int i=0;i<100*20;i++)	pBuffer[i] = '\0';
	//TCHAR fileBuffer[100*20] = {0};
	fileDlg.m_ofn.lpstrFile = pBuffer;
	fileDlg.m_ofn.nMaxFile = 100*20;
	fileDlg.m_ofn.lpstrFile[0] = '\0';
	INT_PTR nRes = fileDlg.DoModal();
	if(nRes == IDOK)
	{
		CString str;
		POSITION pos = fileDlg.GetStartPosition();
		Mat img;
		vector<Mat> images;
		while(pos!=NULL)
		{
			str = fileDlg.GetNextPathName(pos);
			CStringA file(str.GetBuffer(0));
			filePath.ReleaseBuffer();
			string path=file.GetBuffer(0);
			file.ReleaseBuffer();
			img = imread(path);
			images.push_back(img);
		}
		//imshow("1",imgs[0]);
		//imshow("2",imgs[1]);
		//imshow("3",imgs[2]);
		//waitKey(0);
		//destroyAllWindows();
		Mat pano;
		Stitcher stitcher = Stitcher::createDefault(true);
		Stitcher::Status status = stitcher.stitch(images,pano);
		if(status == Stitcher::OK)
		{
			//imshow("ƴ��",pano);
			pano.copyTo(tempImg);
			ok = true;
			fitWindow = true;
			saveFlag = false;
			Invalidate(TRUE);
			UpdateWindow();
		}
		else if(status == Stitcher::ERR_NEED_MORE_IMGS)
		{
			MessageBox(_T("��Ҫ����ͼƬ��"),_T("����"),MB_OK);
		}
		pano.release();
	}
}