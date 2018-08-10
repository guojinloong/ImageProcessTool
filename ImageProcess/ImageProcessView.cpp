
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

#include "DlgFilter.h"
#include "DlgMorphology.h"
#include "DlgThreshold.h"
#include "DlgThresholdAdaptive.h"
#include "DlgEdgeCanny.h"
#include "DlgEdgeSobel.h"
#include "DlgEdgeLaplacian.h"
#include "DlgEdgeScharr.h"
#include "DlgHoughLines.h"
#include "DlgHoughLinesP.h"
#include "DlgAffineWarp.h"
#include "DlgPicture.h"
#include "DlgAffineWarpRotation.h"
#include "DlgRemap.h"


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
bool ok = false;
bool fitWindow = true;	//适合窗口标志
//CRect lastRect;
//CSize sizeTotal;
//CPoint CenterPoint;
Mat srcImg,tempImg,dstImg;	//原始图像、缓冲/显示图像、目标图像
Mat temp[10];	//撤销图像缓存
CString filePath,fileName,extension;
//CvvImage image;

// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
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
	ON_COMMAND(ID_PROCESS_PYRAMID, &CImageProcessView::OnProcessPyramid)
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
ON_UPDATE_COMMAND_UI(ID_PROCESS_PYRAMID, &CImageProcessView::OnUpdateProcessPyramid)
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
ON_COMMAND(ID_TRANSFORM_AFFINE_ROTATION, &CImageProcessView::OnTransformAffineRotation)
ON_COMMAND(ID_TRANSFORM_REMAP, &CImageProcessView::OnTransformRemap)
ON_COMMAND(ID_TRANSFORM_EQUALIZEHIST, &CImageProcessView::OnTransformEqualizehist)
//ON_WM_ERASEBKGND()
ON_WM_MOUSEWHEEL()
ON_COMMAND(ID_FILE_SAVE, &CImageProcessView::OnFileSave)
ON_COMMAND(ID_EDIT_UNDO, &CImageProcessView::OnEditUndo)
ON_COMMAND(ID_EDIT_REDO, &CImageProcessView::OnEditRedo)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHLINES, &CImageProcessView::OnUpdateTransformHoughHoughlines)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHLINESP, &CImageProcessView::OnUpdateTransformHoughHoughlinesp)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_HOUGH_HOUGHCIRCLES, &CImageProcessView::OnUpdateTransformHoughHoughcircles)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_AFFINE_WARP, &CImageProcessView::OnUpdateTransformAffineWarp)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_AFFINE_ROTATION, &CImageProcessView::OnUpdateTransformAffineRotation)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_REMAP, &CImageProcessView::OnUpdateTransformRemap)
ON_UPDATE_COMMAND_UI(ID_TRANSFORM_EQUALIZEHIST, &CImageProcessView::OnUpdateTransformEqualizehist)
ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CImageProcessView::OnUpdateEditUndo)
ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CImageProcessView::OnUpdateEditRedo)
ON_UPDATE_COMMAND_UI(ID_PROCESS_FLOODFILL, &CImageProcessView::OnUpdateProcessFloodfill)
ON_COMMAND(ID_FILE_SAVE_AS, &CImageProcessView::OnFileSaveAs)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CImageProcessView::OnUpdateFileSave)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CImageProcessView::OnUpdateFileSaveAs)
END_MESSAGE_MAP()

// CImageProcessView 构造/析构

CImageProcessView::CImageProcessView()
{
	// TODO: 在此处添加构造代码
	//m_pDlgFilter = NULL;
}

CImageProcessView::~CImageProcessView()
{
	//if(!m_pDlgFilter)	delete m_pDlgFilter;
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CImageProcessView 绘制

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//image = pDoc->image;
	IplImage img = tempImg;
	CvvImage image;
	image.CopyOf(&img);
	CRect rt;
	GetClientRect(&rt);
	if((image.Width() > rt.Width())||(image.Height() > rt.Height()))
	{
		if(srcImg.data && fitWindow == true)
		{
			float widthCoff = (float)rt.Width() / image.Width();
			float heightCoff = (float)rt.Height() / image.Height();
			ViewScale = widthCoff < heightCoff ? widthCoff:heightCoff;
			fitWindow = false;
		}
	}
	//if(ViewScale == 1)	CenterPoint = CPoint(rt.Width()/2,rt.Height());
	int imageX = (int)(image.Width()*ViewScale);
	int imageY = (int)(image.Height()*ViewScale);
	
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
	}
	CSize sizeTotal(imageX,imageY);
	CRect rect(rectL,rectT,rectR,rectB);
	SetScrollSizes(MM_TEXT,sizeTotal);
	HDC hDC=pDC->GetSafeHdc();
	image.DrawToHDC(hDC,&rect);
	//撤销保存图像
	if(ok == true)
	{
		t++;
		if(t>9)
		{
			for(int i=0;i<9;i++)	temp[i]=temp[i+1];
			t=9;
		}
		tempImg.copyTo(srcImg);
		tempImg.copyTo(dstImg);
		tempImg.copyTo(temp[t]);
		ok = false;
		TRACE(_T("Back:%d\n"),t);
		tMax = t;
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


BOOL CImageProcessView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(zDelta > 0)
	{
		if(ViewScale+0.05 < 5)	ViewScale = ViewScale + 0.05;
	}
	else if(zDelta < 0)
	{
		if(ViewScale-0.02 > 0 )		ViewScale = ViewScale - 0.02;
	}
	else  return CView::OnMouseWheel(nFlags, zDelta, pt);
	//CenterPoint = pt;
	Invalidate();
	UpdateWindow();
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
	if((srcImg.data)&&(t>0))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码
	if(t<tMax)
	{
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
	if((srcImg.data)&&(t<tMax))	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


//void CImageProcessView::OnFileOpen()
//{
//	 //TODO: 在此添加命令处理程序代码
//	TCHAR szFilter[] = _T("JPEG文件(*.jpeg;*.jpg;*,jpe;*jp2)|*.jpeg;*.jpg;*,jpe;*jp2|PNG文件(*.png)|*.png|Windows位图(*.bmp;*.dib)|*.bmp;*.dib|TIFF文件(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters光栅文件(*.sr;*.ras)|*.sr;*.ras|便携文件格式(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
//	CFileDialog fileDlg(TRUE,_T("jpg"),NULL,0,szFilter,this);
//	CString filePath;
//	if(fileDlg.DoModal() == IDOK)
//	{
//		filePath = fileDlg.GetPathName();
//		fileName = fileDlg.GetFileTitle();
//		extension = fileDlg.GetFileExt();
//		CStringA file(filePath.GetBuffer(0));
//		filePath.ReleaseBuffer();
//		string path=file.GetBuffer(0);
//		file.ReleaseBuffer();
//		srcImg=imread(path);
//		srcImg.copyTo(tempImg);
//		imshow("【原始图像】",srcImg);
//		IplImage src = srcImg;
//		image.CopyOf(&src,1);
//	}
//}


void CImageProcessView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	INT_PTR nRes = MessageBox(_T("   确定保存？"),_T("提示"),MB_OKCANCEL|MB_ICONQUESTION);
	if(nRes == IDOK)
	{
		CStringA file(filePath.GetBuffer(0));
		filePath.ReleaseBuffer();
		string path=file.GetBuffer(0);
		file.ReleaseBuffer();
		if(!dstImg.data)	dstImg = srcImg.clone();
		imwrite(path,dstImg);
		TRACE(_T("Save Success!\n"));
	}
	else return;
}


void CImageProcessView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szFilter[] = _T("JPEG文件(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG文件(*.png)|*.png|Windows位图(*.bmp;*.dib)|*.bmp;*.dib|TIFF文件(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters光栅文件(*.sr;*.ras)|*.sr;*.ras|便携文件格式(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
	CFileDialog fileDlg(FALSE,extension,fileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,this);
	if(fileDlg.DoModal() == IDOK)
	{
		CString filePath = fileDlg.GetPathName();
		fileName = fileDlg.GetFileTitle();
		extension = fileDlg.GetFileExt();
		//CImageProcessDoc* pDoc = GetDocument();
		//pDoc->DoSave(filePath);
		CStringA file(filePath.GetBuffer(0));
		filePath.ReleaseBuffer();
		string path=file.GetBuffer(0);
		file.ReleaseBuffer();
		if(!dstImg.data)	dstImg = srcImg.clone();
		imwrite(path,dstImg);
		srcImg = imread(path);
		TRACE(_T("Save As Success!\n"));
	}
}


void CImageProcessView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnProcessFilterBox()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgFilter dlg;
		INT_PTR nRes = dlg.DoModal();
		//if(m_pDlgFilter == NULL)
		//{
		//	m_pDlgFilter = new CDlgFilter();
		//	m_pDlgFilter->Create(IDD_DIALOG_FILTER_BOX,this);
		//}
		//boxFilterValue = m_pDlgFilter->m_filterValue;
		//INT_PTR nRes = m_pDlgFilter->ShowWindow(SW_SHOW);
		if(nRes == IDOK)	ok = true;
		else return;
		boxFilterValue = dlg.m_filterValue;
		boxFilter(srcImg,tempImg,-1,Size(boxFilterValue+1,boxFilterValue+1));
		//imshow("【方框滤波】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterMean()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgFilter dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		meanFilterValue = dlg.m_filterValue;
		blur(srcImg,tempImg,Size(meanFilterValue+1,meanFilterValue+1));
		//imshow("【均值滤波】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterGaussian()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgFilter dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		gaussianFilterValue = dlg.m_filterValue;
		GaussianBlur(srcImg,tempImg,Size(gaussianFilterValue*2+1,gaussianFilterValue*2+1),0,0);
		//imshow("【高斯滤波】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterMedian()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgFilter dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		medianFilterValue = dlg.m_filterValue;
		medianBlur(srcImg,tempImg,medianFilterValue*2+1);
		//imshow("【中值滤波】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterBilateral()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgFilter dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		bilateralFilterValue = dlg.m_filterValue;
		bilateralFilter(srcImg,tempImg,bilateralFilterValue,bilateralFilterValue*2,bilateralFilterValue/2);
		//imshow("【双边滤波】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessMorphologyDilate()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		dilate(srcImg,tempImg,element);
		//imshow("【膨胀】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyErode()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		erode(srcImg,tempImg,element);
		//imshow("【腐蚀】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyOpen()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_OPEN,element);
		//imshow("【开运算】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyClose()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_CLOSE,element);
		//imshow("【闭运算】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyGradient()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_GRADIENT,element);
		//imshow("【梯度运算】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyTophat()
{
	// TODO: 在此添加命令处理程序代码
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_TOPHAT,element);
		//imshow("【顶帽运算】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyBlackhat()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgMorphology dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		elementShape = dlg.shape;
		structElementSize = dlg.m_structElementSize;
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_BLACKHAT,element);
		//imshow("【黑帽运算】",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessPyramid()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		if(pyr == 1)	tempImg = srcImg.clone();
		if((pyr>1)&&(pyr<10))	pyrUp(srcImg,tempImg,Size((int)(srcImg.cols*pyr),(int)(srcImg.cols*pyr)));
		else if((pyr<1)&&(pyr>0))	pyrDown(srcImg,tempImg,Size((int)(srcImg.cols*pyr),(int)(srcImg.cols*pyr)));
		//imshow("【金字塔】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFloodfill()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageProcessView::OnProcessThreshold()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgThreshold dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		threshValue = dlg.m_threshold;
		maxValue = dlg.m_maxValue;
		thresholdType = dlg.m_type;
		threshold(srcImg,tempImg,threshValue,maxValue,thresholdType);
		//imshow("【阈值化】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessThresholdAdaptive()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgThresholdAdaptive dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		blockSize = dlg.m_blockSize;
		maxValue = dlg.m_maxValue;
		adaptiveMethod = dlg.m_type1;
		thresholdType = dlg.m_type2;
		Mat grayImg;
		cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
		//imshow("【灰度图像】",grayImg);
		adaptiveThreshold(grayImg,tempImg,maxValue,adaptiveMethod,thresholdType,blockSize*2+1,C);
		//imshow("【自适应阈值化】",tempImg);
		Invalidate();
		UpdateWindow();
		grayImg.release();
	}
}


void CImageProcessView::OnTransformEdgeCanny()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgEdgeCanny dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		cannyThreshold1 = dlg.m_cannyThreshold1;
		cannyThreshold2 = dlg.m_cannyThreshold2;
		apertureSize = dlg.m_type;
		Mat mask;
		blur(srcImg,tempImg,Size(3,3));
		Canny(tempImg,mask,cannyThreshold1,cannyThreshold2,apertureSize);
		tempImg=Scalar::all(0);
		srcImg.copyTo(tempImg,mask);
		//imshow("【Canny边缘检测】",tempImg);
		Invalidate();
		UpdateWindow();
		mask.release();
	}
}


void CImageProcessView::OnTransformEdgeSobel()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
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
		Invalidate();
		UpdateWindow();
		sobelX.release();
		sobelY.release();
		sobelXAbs.release();
		sobelYAbs.release();
	}
}


void CImageProcessView::OnTransformEdgeLaplacian()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
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
		Invalidate();
		UpdateWindow();
		grayImg.release();
	}
}


void CImageProcessView::OnTransformEdgeScharr()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
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
		Invalidate();
		UpdateWindow();
		scharrX.release();
		scharrY.release();
		scharrXAbs.release();
		scharrYAbs.release();
	}
}


void CImageProcessView::OnTransformHoughHoughlines()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
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
			line(tempImg,pt1,pt2,Scalar(0,0,255),1);
		}
		//imshow("【标准霍夫变换】",dst);
		Invalidate();
		UpdateWindow();
		midImg.release();
		//vector<Vec2f>(lines).swap(lines);
		//lines.clear();
	}
}


void CImageProcessView::OnTransformHoughHoughlinesp()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
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
			line(tempImg,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,255,20),1,CV_AA);
		}
		//imshow("【累计霍夫变换】",dst);
		Invalidate();
		UpdateWindow();
		midImg.release();
	}
}


void CImageProcessView::OnTransformHoughHoughcircles()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		Mat grayImg,blurImg,dst;
		cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
		GaussianBlur(grayImg,blurImg,Size(3,3),2,2);
		vector<Vec3f> circles;
		HoughCircles(blurImg,circles,CV_HOUGH_GRADIENT,1.5,10,200,100,0,0);
		for(unsigned int i=0;i<circles.size();i++)
		{
			Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
			int radius=cvRound(circles[i][2]);
			circle(dst,center,3,Scalar(0,255,255),-1,8,0);
			circle(dst,center,radius,Scalar(255,0,0),3,8,0);
		}
		//imshow("【霍夫圆变换】",dst);
		Invalidate();
		UpdateWindow();
		grayImg.release();
		blurImg.release();
		dst.release();
	}
}


void CImageProcessView::OnTransformAffineWarp()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		//CDlgPicture dlg;
		CDlgAffineWarp dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		interWays = dlg.m_interWays;
		boarderValue = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
		Mat warpMat(2,3,CV_32FC1);
		Point2f srcTriangle[3];
		Point2f dstTriangle[3];
		tempImg=Mat::zeros(srcImg.rows,srcImg.cols,srcImg.type());
		srcTriangle[0]=Point2f(0,0);
		srcTriangle[1]=Point2f((float)srcImg.cols-1,0);
		srcTriangle[2]=Point2f(0,(float)srcImg.rows-1);
		dstTriangle[0]=Point2f((float)(srcImg.cols*0.00),(float)(srcImg.rows*0.33));
		dstTriangle[1]=Point2f((float)(srcImg.cols*0.65),(float)(srcImg.rows*0.35));
		dstTriangle[2]=Point2f((float)(srcImg.cols*0.15),(float)(srcImg.rows*0.6));
		warpMat=getAffineTransform(srcTriangle,dstTriangle);
		warpAffine(srcImg,tempImg,warpMat,srcImg.size(),interWays,0,boarderValue);
		//imshow("【Warp】",tempImg);
		Invalidate();
		UpdateWindow();
		warpMat.release();
	}
}


void CImageProcessView::OnUpdateTransformAffineWarp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformAffineRotation()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgAffineWarpRotation dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		angle = dlg.m_angle;
		scale = dlg.m_scale;
		interWays = dlg.m_interWays;
		boarderValue = Scalar(dlg.m_colorB,dlg.m_colorG,dlg.m_colorR);
		Mat rotationMat(2,3,CV_32FC1);
		Point2f anchor((float)srcImg.cols/2,(float)srcImg.rows/2);
		rotationMat=getRotationMatrix2D(anchor,angle,scale);
		warpAffine(srcImg,tempImg,rotationMat,srcImg.size(),interWays,0,boarderValue);
		//imshow("【WarpRotation】",tempImg);
		Invalidate();
		UpdateWindow();
		rotationMat.release();
	}
}


void CImageProcessView::OnUpdateTransformAffineRotation(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformRemap()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		CDlgRemap dlg;
		INT_PTR nRes = dlg.DoModal();
		if(nRes == IDOK)	ok = true;
		else return;
		remapWays = dlg.m_remapWays;
		interWays = dlg.m_interWays;
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
					mapy.at<float>(j,i)=static_cast<float>(srcImg.rows-j);
					break;
				case 0:
					mapx.at<float>(j,i)=static_cast<float>(srcImg.cols-i);
					mapy.at<float>(j,i)=static_cast<float>(j);
					break;
				case 2:
					mapx.at<float>(j,i)=static_cast<float>(srcImg.cols-i);
					mapy.at<float>(j,i)=static_cast<float>(srcImg.rows-j);
					break;
				}
			}
		}
		remap(srcImg,tempImg,mapx,mapy,interWays,BORDER_CONSTANT,Scalar(0,0,0));
		//imshow("【重映射】",tempImg);
		Invalidate();
		UpdateWindow();
		mapx.release();
		mapy.release();
	}
}


void CImageProcessView::OnUpdateTransformRemap(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnTransformEqualizehist()
{
	// TODO: 在此添加命令处理程序代码
	if(srcImg.data)
	{
		Mat grayImg;
		cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
		equalizeHist(grayImg,tempImg);
		ok = true;
		//imshow("【直方图均衡化】",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnUpdateTransformEqualizehist(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessFilterBox(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessFilterMean(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessFilterMedian(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessFilterBilateral(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyDilate(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyErode(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyOpen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyClose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyGradient(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyTophat(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessMorphologyBlackhat(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessThreshold(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessThresholdAdaptive(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessPyramid(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(false);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateProcessFloodfill(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(false);
	else pCmdUI->Enable(false);
}



void CImageProcessView::OnUpdateTransformEdgeCanny(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateTransformEdgeSobel(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateTransformEdgeLaplacian(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateTransformEdgeScharr(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateTransformHoughHoughlines(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateTransformHoughHoughlinesp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}


void CImageProcessView::OnUpdateTransformHoughHoughcircles(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(srcImg.data)	pCmdUI->Enable(false);
	else pCmdUI->Enable(false);
}