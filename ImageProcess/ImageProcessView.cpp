
// ImageProcessView.cpp : CImageProcessView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ImageProcess.h"
#endif

#include "ImageProcessDoc.h"
#include "ImageProcessView.h"
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

// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PROCESS_FILTER_BOX, &CImageProcessView::OnProcessFilterBox)
	ON_COMMAND(ID_PROCESS_FILTER_MEAN, &CImageProcessView::OnProcessFilterMean)
	ON_COMMAND(ID_PROCESS_FILTER_GAUSSIAN, &CImageProcessView::OnProcessFilterGaussian)
	ON_COMMAND(ID_PROCESS_FILTER_MEDIAN, &CImageProcessView::OnProcessFilterMedian)
	ON_COMMAND(ID_PROCESS_FILTER_BILATERAL, &CImageProcessView::OnProcessFilterBilateral)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_DILATE, &CImageProcessView::OnProcessMorphologyDilate)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_ERODE, &CImageProcessView::OnProcessMorphologyErode)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_OPEN, &CImageProcessView::OnProcessMorphologyOpen)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_CLOSE, &CImageProcessView::OnProcessMorphologyClose)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_GRADIENT, &CImageProcessView::OnProcessMorphologyGradient)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_TOPHAT, &CImageProcessView::OnProcessMorphologyTophat)
	ON_COMMAND(ID_PROCESS_MORPHOLOGY_BLACKHAT, &CImageProcessView::OnProcessMorphologyBlackhat)
	ON_COMMAND(ID_PROCESS_FLOODFILL, &CImageProcessView::OnProcessFloodfill)
	ON_COMMAND(ID_PROCESS_PYRAMID_UP, &CImageProcessView::OnProcessPyramidUp)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_PYRAMID_UP, &CImageProcessView::OnUpdateProcessPyramidUp)
	ON_COMMAND(ID_PROCESS_PYRAMID_DOWN, &CImageProcessView::OnProcessPyramidDown)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_PYRAMID_DOWN, &CImageProcessView::OnUpdateProcessPyramidDown)
	ON_COMMAND(ID_PROCESS_THRESHOLD, &CImageProcessView::OnProcessThreshold)
	ON_COMMAND(ID_PROCESS_THRESHOLD_ADAPTIVE, &CImageProcessView::OnProcessThresholdAdaptive)
	ON_COMMAND(ID_TRANSFORM_EDGE_CANNY, &CImageProcessView::OnTransformEdgeCanny)
	ON_COMMAND(ID_TRANSFORM_EDGE_SOBEL, &CImageProcessView::OnTransformEdgeSobel)
	ON_COMMAND(ID_TRANSFORM_EDGE_LAPLACIAN, &CImageProcessView::OnTransformEdgeLaplacian)
	ON_COMMAND(ID_TRANSFORM_EDGE_SCHARR, &CImageProcessView::OnTransformEdgeScharr)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_BOX, &CImageProcessView::OnUpdateProcessFilterBox)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_MEAN, &CImageProcessView::OnUpdateProcessFilterMean)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_GAUSSIAN, &CImageProcessView::OnUpdateProcessFilterGaussian)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_MEDIAN, &CImageProcessView::OnUpdateProcessFilterMedian)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FILTER_BILATERAL, &CImageProcessView::OnUpdateProcessFilterBilateral)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_DILATE, &CImageProcessView::OnUpdateProcessMorphologyDilate)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_ERODE, &CImageProcessView::OnUpdateProcessMorphologyErode)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_OPEN, &CImageProcessView::OnUpdateProcessMorphologyOpen)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_CLOSE, &CImageProcessView::OnUpdateProcessMorphologyClose)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_GRADIENT, &CImageProcessView::OnUpdateProcessMorphologyGradient)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_TOPHAT, &CImageProcessView::OnUpdateProcessMorphologyTophat)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_MORPHOLOGY_BLACKHAT, &CImageProcessView::OnUpdateProcessMorphologyBlackhat)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_THRESHOLD, &CImageProcessView::OnUpdateProcessThreshold)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_THRESHOLD_ADAPTIVE, &CImageProcessView::OnUpdateProcessThresholdAdaptive)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_CANNY, &CImageProcessView::OnUpdateTransformEdgeCanny)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_SOBEL, &CImageProcessView::OnUpdateTransformEdgeSobel)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_LAPLACIAN, &CImageProcessView::OnUpdateTransformEdgeLaplacian)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EDGE_SCHARR, &CImageProcessView::OnUpdateTransformEdgeScharr)
	ON_COMMAND(ID_TRANSFORM_HOUGH_HOUGHLINES, &CImageProcessView::OnTransformHoughHoughlines)
	ON_COMMAND(ID_TRANSFORM_HOUGH_HOUGHLINESP, &CImageProcessView::OnTransformHoughHoughlinesp)
	ON_COMMAND(ID_TRANSFORM_HOUGH_HOUGHCIRCLES, &CImageProcessView::OnTransformHoughHoughcircles)
	ON_COMMAND(ID_TRANSFORM_AFFINE_WARP, &CImageProcessView::OnTransformAffineWarp)
	//ON_COMMAND(ID_TRANSFORM_AFFINE_ROTATION, &CImageProcessView::OnTransformAffineRotation)
	ON_COMMAND(ID_TRANSFORM_REMAP, &CImageProcessView::OnTransformRemap)
	ON_COMMAND(ID_TRANSFORM_EQUALIZEHIST, &CImageProcessView::OnTransformEqualizehist)
	ON_COMMAND(ID_EDIT_UNDO, &CImageProcessView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CImageProcessView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHLINES, &CImageProcessView::OnUpdateTransformHoughHoughlines)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHLINESP, &CImageProcessView::OnUpdateTransformHoughHoughlinesp)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHCIRCLES, &CImageProcessView::OnUpdateTransformHoughHoughcircles)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_AFFINE_WARP, &CImageProcessView::OnUpdateTransformAffineWarp)
	//ON_UPDATE_COMMAND_UI(ID_TRANSFORM_AFFINE_ROTATION, &CImageProcessView::OnUpdateTransformAffineRotation)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_REMAP, &CImageProcessView::OnUpdateTransformRemap)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EQUALIZEHIST, &CImageProcessView::OnUpdateTransformEqualizehist)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CImageProcessView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CImageProcessView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FLOODFILL, &CImageProcessView::OnUpdateProcessFloodfill)
	ON_COMMAND(ID_PROCESS_RESIZE, &CImageProcessView::OnProcessResize)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_RESIZE, &CImageProcessView::OnUpdateProcessResize)
	ON_COMMAND(ID_ADVANCED_DEFOG, &CImageProcessView::OnAdvancedDefog)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_DEFOG, &CImageProcessView::OnUpdateAdvancedDefog)
	ON_COMMAND(ID_CONTOURS_FINDCONTOURS, &CImageProcessView::OnContoursFindcontours)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_FINDCONTOURS, &CImageProcessView::OnUpdateContoursFindcontours)
	ON_COMMAND(ID_CONTOURS_CONVEXHULL, &CImageProcessView::OnContoursConvexhull)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_CONVEXHULL, &CImageProcessView::OnUpdateContoursConvexhull)
	ON_COMMAND(ID_CONTOURS_POLY, &CImageProcessView::OnContoursPoly)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_POLY, &CImageProcessView::OnUpdateContoursPoly)
	ON_COMMAND(ID_EDIT_SET, &CImageProcessView::OnEditSet)
	ON_COMMAND(ID_EDIT_CLEAR, &CImageProcessView::OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CImageProcessView::OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_PROPERTY, &CImageProcessView::OnEditProperty)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTY, &CImageProcessView::OnUpdateEditProperty)
	ON_COMMAND(ID_CONTOURS_MOMENTS, &CImageProcessView::OnContoursMoments)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_MOMENTS, &CImageProcessView::OnUpdateContoursMoments)
	ON_COMMAND(ID_CONTOURS_WATERSHED, &CImageProcessView::OnContoursWatershed)
	ON_UPDATE_COMMAND_UI(ID_CONTOURS_WATERSHED, &CImageProcessView::OnUpdateContoursWatershed)
	ON_COMMAND(ID_ADVANCED_INPAINT, &CImageProcessView::OnAdvancedInpaint)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_INPAINT, &CImageProcessView::OnUpdateAdvancedInpaint)
END_MESSAGE_MAP()

// CImageProcessView ����/����

CImageProcessView::CImageProcessView()
{
	// TODO: �ڴ˴���ӹ������
	m_pDlgSet = NULL;
	m_pDlgFilter = NULL;
	m_pDlgFloodFill = NULL;
	
	color = RGB(120,120,120);
	thickness = 1;
	lineType = 8;
	shift = 0;
	m_bDraw = false;
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
}


CImageProcessView::~CImageProcessView()
{
	if(!m_pDlgSet)	delete m_pDlgSet;
	if(!m_pDlgFilter)	delete m_pDlgFilter;
	if(!m_pDlgFloodFill)	delete m_pDlgFloodFill;
}


BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	//CBrush backBrush;
	//backBrush.CreateSolidBrush(RGB(255,0,255));
	//cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,0,(HBRUSH)::GetStockObject(255),0);
	return CScrollView::PreCreateWindow(cs);
}


BOOL CImageProcessView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	hAccel = LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(ID_EDIT_REDO));
	if(TranslateAccelerator(GetSafeHwnd(),hAccel,pMsg))
		return true;
	return CScrollView::PreTranslateMessage(pMsg);
}


BOOL CImageProcessView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rt;
	//GetClientRect(&rt);
	//Gdiplus::Bitmap bitmap(srcImg.cols, srcImg.rows, srcImg.cols * srcImg.channels(),  PixelFormat24bppRGB, (BYTE*)tempImg.data);//����Mat���󴴽�һ��GDI+�е�Bitmapλͼ
	//CDC memDC;
	//CBitmap memBitmap;
	//CBrush backBrush;
	//memDC.CreateCompatibleDC(pDC);
	//memBitmap.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
	//backBrush.CreateSolidBrush(RGBCImageProcessView::OnEraseBkgnd(255,0,255));

	//memDC.SelectObject(&memBitmap);
	//memDC.FillSolidRect(&rt,RGB(255,0,0));
	//
	//memDC.MoveTo(rt.left,rt.top);
	//memDC.LineTo(rt.right,rt.bottom);
 //   //��ͼ
 //   Gdiplus::Graphics g(memDC.m_hDC);
 //   Gdiplus::Image *img = &bitmap; 
 //   g.DrawImage(img, Gdiplus::Rect(0,0,rt.Width(),rt.Height()));
 //   g.ReleaseHDC(memDC.m_hDC);

	//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&memDC,0,0,SRCCOPY);
	//memDC.DeleteDC();
	//memBitmap.DeleteObject();
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}


//BOOL CImageProcessView::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	 CBrush backBrush;
//	 backBrush.CreateSolidBrush(RGB(255,0,255));
//
//	//����ɵ�brush
//	 CBrush* pOldBrush = pDC->SelectObject(&backBrush);
//	 CRect rect;
//	 pDC->GetClipBox(&rect);
//
//	//����Ҫ������
//	 pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
//	 pDC->SelectObject(pOldBrush);
//
//
//	return CScrollView::OnEraseBkgnd(pDC);
//}


// CImageProcessView ����

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
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
		if((tempImg.cols > rt.Width())||(tempImg.rows > rt.Height()))
		{
			if(!srcImg.empty() && fitWindow == true)
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
		
		CDC memDC;
		CBitmap memBitmap;
		CBrush backBrush;

		memDC.CreateCompatibleDC(pDC);
		memBitmap.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
		backBrush.CreateSolidBrush(RGB(255,0,255));

		memDC.SelectObject(&memBitmap);
		memDC.FillSolidRect(&rt,color);

		//memDC.MoveTo(rt.left,rt.top);
		//memDC.LineTo(rt.right,rt.bottom);
		//��ͼ
		Gdiplus::Graphics g(memDC.m_hDC);
		Gdiplus::Bitmap bitmap(tempImg.cols,tempImg.rows,tempImg.cols*tempImg.channels(),PixelFormat24bppRGB,(BYTE*)tempImg.data);//����Mat���󴴽�һ��GDI+�е�Bitmapλͼ
		Gdiplus::Image *img = &bitmap;
		//Gdiplus::Image *img = Gdiplus::Image::FromFile(filePath);
		g.DrawImage(img, Gdiplus::Rect(rect.left,rect.top,rect.Width(),rect.Height()));
		g.ReleaseHDC(memDC.m_hDC);

		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
		memBitmap.DeleteObject();

		CSize sizeTotal(imageX,imageY);
		SetScrollSizes(MM_TEXT,sizeTotal);
		//HDC hDC=pDC->GetSafeHdc();
		//IplImage img1 = tempImg;
		//pDoc->image.CopyOf(&img1);
		//pDoc->image.DrawToHDC(hDC,&rect);
		//��������ͼ��
		if(ok == true)
		{
			saveFlag = true;
			t++;
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
	}
}

void CImageProcessView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BOOL CImageProcessView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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


// CImageProcessView ��ӡ


void CImageProcessView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CImageProcessView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessView ���

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessView ��Ϣ�������

void CImageProcessView::OnLButtonDown(UINT nFlags, CPoint point)
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


void CImageProcessView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bDraw = false;
	ReleaseCapture();
	ClipCursor(NULL);
	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageProcessView::OnMouseMove(UINT nFlags, CPoint point)
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


void CImageProcessView::OnRButtonDown(UINT nFlags, CPoint point)
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


void CImageProcessView::OnEditUndo()
{
	// TODO: �ڴ���������������
	saveFlag = true;
	t--;
	temp[t].copyTo(srcImg);
	temp[t].copyTo(dstImg);
	temp[t].copyTo(tempImg);
	if(t<0)	t=0;
	TRACE(_T("Back:%d\n"),t);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if((!srcImg.empty())&&(t>0))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditRedo()
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
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if((!srcImg.empty())&&(t<tMax))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditClear()
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


void CImageProcessView::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditProperty()
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
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnUpdateEditProperty(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditSet()
{
	// TODO: �ڴ���������������
	CDlgSet m_dlgSet;//(thickness,lineType,shift);
	INT_PTR nRes = m_dlgSet.DoModal();
	if(nRes == IDOK)
	{
		thickness =m_dlgSet.m_thickness;
		lineType = m_dlgSet.m_lineType;
		shift = m_dlgSet.m_shift;
		color = RGB(m_dlgSet.m_colorR,m_dlgSet.m_colorG,m_dlgSet.m_colorB);
		//LOGFONT lf;
		//m_dlgSet.m_font.GetLogFont(&lf);
		//font.CreateFontIndirectW(&lf);
		Invalidate();
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
	//	Invalidate();
	//	UpdateWindow();
	//}
}


void CImageProcessView::OnProcessFilterBox()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterBox(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterMean()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterMean(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterGaussian()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterMedian()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterMedian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterBilateral()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterBilateral(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyDilate()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyDilate(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyErode()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyErode(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyOpen()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyOpen(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyClose()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyClose(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyGradient()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyGradient(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyTophat()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyTophat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyBlackhat()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyBlackhat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CImageProcessView::OnProcessPyramidUp()
{
	// TODO: �ڴ���������������
	ok = true;
	pyrUp(tempImg,tempImg,Size(tempImg.cols*2,tempImg.rows*2));
	//imshow("����������",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessPyramidUp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessPyramidDown()
{
	// TODO: �ڴ���������������
	ok = true;
	pyrDown(tempImg,tempImg,Size(tempImg.cols/2,tempImg.rows/2));
	//imshow("����������",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessPyramidDown(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessResize()
{
	// TODO: �ڴ���������������
	CDlgResize dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//resize(srcImg,tempImg,Size(dlg.m_width,dlg.m_height),0,0,dlg.m_interWays);
	//imshow("�ߴ����",tempImg);
	Invalidate();
	UpdateWindow();
}



void CImageProcessView::OnUpdateProcessResize(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFloodfill()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFloodfill(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessThreshold()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessThreshold(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessThresholdAdaptive()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessThresholdAdaptive(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeCanny()
{
	// TODO: �ڴ���������������
	CDlgEdgeCanny dlg;
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeCanny(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeSobel()
{
	// TODO: �ڴ���������������
	CDlgEdgeSobel dlg;
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeSobel(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeLaplacian()
{
	// TODO: �ڴ���������������
	CDlgEdgeLaplacian dlg;
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeLaplacian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeScharr()
{
	// TODO: �ڴ���������������
	CDlgEdgeScharr dlg;
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeScharr(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformHoughHoughlines()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformHoughHoughlines(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformHoughHoughlinesp()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformHoughHoughlinesp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformHoughHoughcircles()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformHoughHoughcircles(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(false);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformAffineWarp()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformAffineWarp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


//void CImageProcessView::OnTransformAffineRotation()
//{
//	// TODO: �ڴ���������������
//	if(!srcImg.empty())
//	{
//		CDlgAffineWarpRotation dlg;
//		INT_PTR nRes = dlg.DoModal();
//		if(nRes == IDOK)	ok = true;
//		else return;
//		angle = dlg.m_angle;
//		scale = dlg.m_scale;
//		interWays = dlg.m_interWays;
//		boarderValue = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
//		Mat rotationMat(2,3,CV_32FC1);
//		Point2f anchor((float)srcImg.cols/2,(float)srcImg.rows/2);
//		rotationMat=getRotationMatrix2D(anchor,angle,scale);
//		warpAffine(srcImg,tempImg,rotationMat,srcImg.size(),interWays,0,boarderValue);
//		//imshow("��WarpRotation��",tempImg);
//		Invalidate();
//		UpdateWindow();
//		rotationMat.release();
//	}
//}
//
//
//void CImageProcessView::OnUpdateTransformAffineRotation(CCmdUI *pCmdUI)
//{
//	// TODO: �ڴ������������û����洦��������
//	if(!srcImg.empty())	pCmdUI->Enable(true);
//	else pCmdUI->Enable(false);
//}


void CImageProcessView::OnTransformRemap()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformRemap(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEqualizehist()
{
	// TODO: �ڴ���������������
	Mat grayImg;
	cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
	equalizeHist(grayImg,tempImg);
	ok = true;
	//imshow("��ֱ��ͼ���⻯��",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEqualizehist(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursFindcontours()
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
	Invalidate();
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


void CImageProcessView::OnUpdateContoursFindcontours(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursConvexhull()
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
	Invalidate();
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


void CImageProcessView::OnUpdateContoursConvexhull(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursPoly()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateContoursPoly(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursMoments()
{
	// TODO: �ڴ���������������
}


void CImageProcessView::OnUpdateContoursMoments(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursWatershed()
{
	// TODO: �ڴ���������������
}


void CImageProcessView::OnUpdateContoursWatershed(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnAdvancedDefog()
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
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateAdvancedDefog(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnAdvancedInpaint()
{
	// TODO: �ڴ���������������
}


void CImageProcessView::OnUpdateAdvancedInpaint(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}