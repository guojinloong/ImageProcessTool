
// ImageProcessView.cpp : CImageProcessView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
double threshValue = 100;	//阈值
double maxValue = 255;	//赋给像素的最大值
int adaptiveMethod = ADAPTIVE_THRESH_MEAN_C;	//指定要使用的自适应阈值算法ADAPTIVE_THRESH_MEAN_C、ADAPTIVE_THRESH_GAUSSIAN_C
int thresholdType = THRESH_BINARY;	//阈值类型，取值为THRESH_BINARY、THRESH_BINARY_INV、THRESH_TRUNC、THRESH_TOZERO、THRESH_TOZERO_INV（自适应阈值只能取前两个）
int blockSize = 3;	//用于计算自适应阈值大小的一个像素的邻域尺寸，取值为3、5、7等
double C = 0;	//自适应阈值化减去平均或加权平均值后的常数值通常为正数，但少数情况下也可以为零或负数
int cannyThreshold1 = 3;	//第一个滞后性阈值,第一个滞后性阈值和第二个滞后性阈值比值通常取2：1-3：1
int cannyThreshold2 = 1;	//第二个滞后性阈值
int apertureSize = 3;	////Sobel算子的孔径大小，必须取1、3、5或7
int ddepth = CV_16S;	//输出图像深度，取-1/CV_16S/CV_32F/CV_64F
int kSize = 0;	//Sobel核的大小，必须取1、3、5或7
int laplacianKernelSize = 3;	//用于计算二阶导数的滤波器的孔径尺寸，必须为正奇数（Laplacian和的大小）
double scale = 1;	//（计算导数时可选的）缩放比例因子scale
double delta = 0;	//在结果存入目标图之前可选的delta值
double angle = 30.0;	//旋转角度，为正值表示向逆时针旋转（坐标原点是左上角）
int remapWays = 1;	//重映射方式，水平镜像、垂直镜像
int interWays = INTER_LINEAR;	//插值方式，可取INTER_NEAREST(最近邻插值)、INTER_LINEAR(双线性插值)、INTER_CUBIC(双三次样条插值4x4)、INTER_LANCZOS4(Lanczos插值8x8)
Scalar boarderValue(0,0,0);
//double scale = 0.8;	//缩放系数
//int maxIterationNum = 10;
//int openCloseNum = 9;
//int erodeDilateNum = 9;
//int topBlackHatNum = 2;
double ViewScale = 1;
int t = 0;	//当前的撤销次数
int tMax = t;	//当前撤销次数的最大值
//bool ok = false;
bool fitWindow = true,saveFlag = false,ok = false;	//适合窗口标志,保存标志,确定标志
//CRect lastRect;
//CSize sizeTotal;
CPoint centerPoint;
Mat srcImg,tempImg,dstImg;	//原始图像、缓冲/显示图像、目标图像
Mat temp[10];	//撤销图像缓存
CString filePath,fileName,extension;//文件路径参数
vector<vector<Point>> contours;
vector<vector<Point>> hull;
vector<Vec4i> hierarchy;

// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	// 标准打印命令
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

// CImageProcessView 构造/析构

CImageProcessView::CImageProcessView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	//CBrush backBrush;
	//backBrush.CreateSolidBrush(RGB(255,0,255));
	//cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,0,(HBRUSH)::GetStockObject(255),0);
	return CScrollView::PreCreateWindow(cs);
}


BOOL CImageProcessView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	hAccel = LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(ID_EDIT_REDO));
	if(TranslateAccelerator(GetSafeHwnd(),hAccel,pMsg))
		return true;
	return CScrollView::PreTranslateMessage(pMsg);
}


BOOL CImageProcessView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rt;
	//GetClientRect(&rt);
	//Gdiplus::Bitmap bitmap(srcImg.cols, srcImg.rows, srcImg.cols * srcImg.channels(),  PixelFormat24bppRGB, (BYTE*)tempImg.data);//根据Mat矩阵创建一个GDI+中的Bitmap位图
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
 //   //绘图
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
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	 CBrush backBrush;
//	 backBrush.CreateSolidBrush(RGB(255,0,255));
//
//	//保存旧的brush
//	 CBrush* pOldBrush = pDC->SelectObject(&backBrush);
//	 CRect rect;
//	 pDC->GetClipBox(&rect);
//
//	//画需要的区域
//	 pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
//	 pDC->SelectObject(pOldBrush);
//
//
//	return CScrollView::OnEraseBkgnd(pDC);
//}


// CImageProcessView 绘制

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
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
		//水平方向显示调整
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
		//垂直方向显示调整
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
		//绘图
		Gdiplus::Graphics g(memDC.m_hDC);
		Gdiplus::Bitmap bitmap(tempImg.cols,tempImg.rows,tempImg.cols*tempImg.channels(),PixelFormat24bppRGB,(BYTE*)tempImg.data);//根据Mat矩阵创建一个GDI+中的Bitmap位图
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
		//撤销保存图像
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
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


BOOL CImageProcessView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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


// CImageProcessView 打印


void CImageProcessView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CImageProcessView 诊断

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessView 消息处理程序

void CImageProcessView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDraw = false;
	ReleaseCapture();
	ClipCursor(NULL);
	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageProcessView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	if((!srcImg.empty())&&(t>0))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	if((!srcImg.empty())&&(t<tMax))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditClear()
{
	// TODO: 在此添加命令处理程序代码
	if(contours.size() == 0)
	{
		MessageBox(_T("没有可清理的缓存！"),_T("提示"),MB_OK);
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
		MessageBox(_T("清理完毕！"),_T("提示"),MB_OK);
		return;
	}
}


void CImageProcessView::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditProperty()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditSet()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【方框滤波】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterBox(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterMean()
{
	// TODO: 在此添加命令处理程序代码
	CDlgFilter dlg;
	dlg.filterWays = 1;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//meanFilterValue = dlg.m_filterValue;
	//blur(srcImg,tempImg,Size(meanFilterValue+1,meanFilterValue+1));
	//imshow("【均值滤波】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterMean(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterGaussian()
{
	// TODO: 在此添加命令处理程序代码
	CDlgFilter dlg;
	dlg.filterWays = 2;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//gaussianFilterValue = dlg.m_filterValue;
	//GaussianBlur(srcImg,tempImg,Size(gaussianFilterValue*2+1,gaussianFilterValue*2+1),0,0);
	//imshow("【高斯滤波】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterMedian()
{
	// TODO: 在此添加命令处理程序代码
	CDlgFilter dlg;
	dlg.filterWays = 3;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//medianFilterValue = dlg.m_filterValue;
	//medianBlur(srcImg,tempImg,medianFilterValue*2+1);
	//imshow("【中值滤波】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterMedian(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterBilateral()
{
	// TODO: 在此添加命令处理程序代码
	CDlgFilter dlg;
	dlg.filterWays = 4;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//bilateralFilterValue = dlg.m_filterValue;
	//bilateralFilter(srcImg,tempImg,bilateralFilterValue,bilateralFilterValue*2,bilateralFilterValue/2);
	//imshow("【双边滤波】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFilterBilateral(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyDilate()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 0;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//dilate(srcImg,tempImg,element);
	////imshow("【膨胀】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyDilate(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyErode()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 1;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//erode(srcImg,tempImg,element);
	////imshow("【腐蚀】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyErode(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyOpen()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 2;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_OPEN,element);
	////imshow("【开运算】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyOpen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyClose()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 3;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_CLOSE,element);
	////imshow("【闭运算】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyClose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyGradient()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 4;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_GRADIENT,element);
	////imshow("【梯度运算】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyGradient(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyTophat()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 5;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_TOPHAT,element);
	////imshow("【顶帽运算】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyTophat(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessMorphologyBlackhat()
{
	// TODO: 在此添加命令处理程序代码
	CDlgMorphology dlg;
	dlg.m_morphWays = 6;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//elementShape = dlg.shape;
	//structElementSize = dlg.m_structElementSize;
	//Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
	//morphologyEx(srcImg,tempImg,MORPH_BLACKHAT,element);
	////imshow("【黑帽运算】",tempImg);
	//element.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessMorphologyBlackhat(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CImageProcessView::OnProcessPyramidUp()
{
	// TODO: 在此添加命令处理程序代码
	ok = true;
	pyrUp(tempImg,tempImg,Size(tempImg.cols*2,tempImg.rows*2));
	//imshow("【金字塔】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessPyramidUp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessPyramidDown()
{
	// TODO: 在此添加命令处理程序代码
	ok = true;
	pyrDown(tempImg,tempImg,Size(tempImg.cols/2,tempImg.rows/2));
	//imshow("【金字塔】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessPyramidDown(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessResize()
{
	// TODO: 在此添加命令处理程序代码
	CDlgResize dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//resize(srcImg,tempImg,Size(dlg.m_width,dlg.m_height),0,0,dlg.m_interWays);
	//imshow("尺寸调整",tempImg);
	Invalidate();
	UpdateWindow();
}



void CImageProcessView::OnUpdateProcessResize(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFloodfill()
{
	// TODO: 在此添加命令处理程序代码
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
	////imshow("【效果图】",dstImg);
	//mask.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessFloodfill(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessThreshold()
{
	// TODO: 在此添加命令处理程序代码
	CDlgThreshold dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else srcImg.copyTo(tempImg);
	//threshValue = dlg.m_threshold;
	//maxValue = dlg.m_maxValue;
	//thresholdType = dlg.m_type;
	//threshold(srcImg,tempImg,threshValue,maxValue,thresholdType);
	//imshow("【阈值化】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessThreshold(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessThresholdAdaptive()
{
	// TODO: 在此添加命令处理程序代码
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
	////imshow("【灰度图像】",grayImg);
	//adaptiveThreshold(grayImg,tempImg,maxValue,adaptiveMethod,thresholdType,blockSize*2+1,C);
	////imshow("【自适应阈值化】",tempImg);
	//grayImg.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateProcessThresholdAdaptive(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeCanny()
{
	// TODO: 在此添加命令处理程序代码
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
	////imshow("【Canny边缘检测】",tempImg);
	//mask.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeCanny(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeSobel()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【Sobel边缘检测】",tempImg);
	sobelX.release();
	sobelY.release();
	sobelXAbs.release();
	sobelYAbs.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeSobel(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeLaplacian()
{
	// TODO: 在此添加命令处理程序代码
	CDlgEdgeLaplacian dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	Mat grayImg;
	cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
	//imshow("【灰度图像】",grayImg);
	GaussianBlur(grayImg,grayImg,Size(3,3),0,0);
	Laplacian(grayImg,tempImg,dlg.m_depth,dlg.m_kSize,dlg.m_scale,dlg.m_delta);
	//imshow("【Laplacian边缘检测】",tempImg);
	grayImg.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeLaplacian(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEdgeScharr()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【Scharr边缘检测】",tempImg);
	scharrX.release();
	scharrY.release();
	scharrXAbs.release();
	scharrYAbs.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEdgeScharr(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformHoughHoughlines()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【标准霍夫变换】",dst);
	midImg.release();
	//vector<Vec2f>(lines).swap(lines);
	//lines.clear();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformHoughHoughlines(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformHoughHoughlinesp()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【累计霍夫变换】",dst);
	midImg.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformHoughHoughlinesp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformHoughHoughcircles()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【霍夫圆变换】",dst);
	grayImg.release();
	blurImg.release();
	dst.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformHoughHoughcircles(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(false);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformAffineWarp()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【Warp】",tempImg);
	warpMat.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformAffineWarp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


//void CImageProcessView::OnTransformAffineRotation()
//{
//	// TODO: 在此添加命令处理程序代码
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
//		//imshow("【WarpRotation】",tempImg);
//		Invalidate();
//		UpdateWindow();
//		rotationMat.release();
//	}
//}
//
//
//void CImageProcessView::OnUpdateTransformAffineRotation(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//	if(!srcImg.empty())	pCmdUI->Enable(true);
//	else pCmdUI->Enable(false);
//}


void CImageProcessView::OnTransformRemap()
{
	// TODO: 在此添加命令处理程序代码
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
	//imshow("【重映射】",tempImg);
	mapx.release();
	mapy.release();
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformRemap(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEqualizehist()
{
	// TODO: 在此添加命令处理程序代码
	Mat grayImg;
	cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
	equalizeHist(grayImg,tempImg);
	ok = true;
	//imshow("【直方图均衡化】",tempImg);
	Invalidate();
	UpdateWindow();
}


void CImageProcessView::OnUpdateTransformEqualizehist(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursFindcontours()
{
	// TODO: 在此添加命令处理程序代码
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

	TCHAR szFilter[] = _T("XML文件(*.xml)|*.xml|YAML文件(*.yaml;*.yml)|*.yaml;*.yml|TXT文件(*.txt)|*.txt||");
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
			//id.Format(_T("[:第%d个轮廓"),i);
			fs<<"points"<<"[:";
			for(unsigned int j=0;j<contours[i].size();j++)
				fs<<contours[i][j].x<<contours[i][j].y;
			fs<<"]";
		}
		fs<<"]";
		fs.release();
		CString f;
		f.Format(_T("文件已保存到%s\n\t是否打开？"),p);
		nRes = MessageBox(f,_T("提示"),MB_YESNO);
		if(nRes == IDYES)
		{
			
		}
	}
}


void CImageProcessView::OnUpdateContoursFindcontours(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursConvexhull()
{
	// TODO: 在此添加命令处理程序代码
	CDlgConvexHull dlg;
	INT_PTR nRes = dlg.DoModal();
	if(nRes == IDOK)	ok = true;
	else return;
	if(dlg.m_select)
	{
		if(contours.size() == 0)
		{
			MessageBox(_T("请先进行“查找轮廓”操作！"),_T("提示"),MB_OK);
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

	TCHAR szFilter[] = _T("XML文件(*.xml)|*.xml|YAML文件(*.yaml;*.yml)|*.yaml;*.yml|TXT文件(*.txt)|*.txt||");
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
			//id.Format(_T("[:第%d个轮廓"),i);
			fs<<"points"<<"[:";
			for(unsigned int j=0;j<hull[i].size();j++)
				fs<<"x"<<hull[i][j].x<<"y"<<hull[i][j].y;
			fs<<"]";
		}
		fs<<"]";
		fs.release();
		CString f;
		f.Format(_T("文件已保存到%s\n\t是否打开？"),p);
		nRes = MessageBox(f,_T("提示"),MB_YESNO);
		if(nRes == IDYES)
		{
			
		}
	}
	//vector<vector<Point>>(hull).swap(hull);
	//hull.clear();
}


void CImageProcessView::OnUpdateContoursConvexhull(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursPoly()
{
	// TODO: 在此添加命令处理程序代码
	if((contours.size() == 0)&(hull.size() == 0))
	{
		MessageBox(_T("请先查找轮廓或凸包！"),_T("提示"),MB_OK);
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursMoments()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageProcessView::OnUpdateContoursMoments(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnContoursWatershed()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageProcessView::OnUpdateContoursWatershed(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnAdvancedDefog()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnAdvancedInpaint()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageProcessView::OnUpdateAdvancedInpaint(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(!srcImg.empty())	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}