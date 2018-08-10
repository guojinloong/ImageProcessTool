
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
#include <iostream>

int boxFilterValue = 3;
int meanFilterValue = 2;
int gaussianFilterValue = 3;
int medianFilterValue = 4;
int bilateralFilterValue = 5;
int elementShape = MORPH_RECT;
int structElementSize = 3;
float pyr = 1.0;
//bool threshType = true;
double threshValue = 100;	//��ֵ
double maxValue = 255;	//�������ص����ֵ
int adaptiveMethod = ADAPTIVE_THRESH_MEAN_C;	//ָ��Ҫʹ�õ�����Ӧ��ֵ�㷨ADAPTIVE_THRESH_MEAN_C��ADAPTIVE_THRESH_GAUSSIAN_C
int thresholdType = THRESH_BINARY;	//��ֵ���ͣ�ȡֵΪTHRESH_BINARY��THRESH_BINARY_INV��THRESH_TRUNC��THRESH_TOZERO��THRESH_TOZERO_INV������Ӧ��ֵֻ��ȡǰ������
int blockSize = 3;	//���ڼ�������Ӧ��ֵ��С��һ�����ص�����ߴ磬ȡֵΪ3��5��7��
double C = 0;	//����Ӧ��ֵ����ȥƽ�����Ȩƽ��ֵ��ĳ���ֵͨ��Ϊ�����������������Ҳ����Ϊ�����
int cannyThreshold = 3;	//��һ���ͺ�����ֵ
int cannyRatio = 3;	//��һ���ͺ�����ֵ�͵ڶ����ͺ�����ֵ��ֵ��ȡ2��3
int ddepth = CV_16S;	//���ͼ����ȣ�ȡ-1/CV_16S/CV_32F/CV_64F
int sobelKernelSize = 0;	//Sobel�˵Ĵ�С������ȡ1��3��5��7
int laplacianKernelSize = 3;	//���ڼ�����׵������˲����Ŀ׾��ߴ磬����Ϊ��������Laplacian�͵Ĵ�С��
double scale = 1;	//�����㵼��ʱ��ѡ�ģ����ű�������scale
double delta = 0;	//�ڽ������Ŀ��ͼ֮ǰ��ѡ��deltaֵ
double angle = 30.0;	//��ת�Ƕȣ�Ϊ��ֵ��ʾ����ʱ����ת������ԭ�������Ͻǣ�
int remapWays = 1;	//��ӳ�䷽ʽ��ˮƽ���񡢴�ֱ����
int interWays = INTER_LINEAR;	//��ֵ��ʽ����ȡINTER_NEAREST(����ڲ�ֵ)��INTER_LINEAR(˫���Բ�ֵ)��INTER_CUBIC(˫����������ֵ4x4)��INTER_LANCZOS4(Lanczos��ֵ8x8)
//double scale = 0.8;	//����ϵ��
//int maxIterationNum = 10;
//int openCloseNum = 9;
//int erodeDilateNum = 9;
//int topBlackHatNum = 2;
float ViewScale = 1;
int scaleFlag = 1;
CSize sizeTotal;
CPoint CenterPoint;
Mat srcImg,tempImg,dstImg;
CString fileName,extension;
CvvImage image;
// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessView, CScrollView)
	// ��׼��ӡ����
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
//	ON_COMMAND(ID_PROCESS_FLOODFILL, &CImageProcessView::OnProcessFloodfill)
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
//ON_COMMAND(ID_FILE_SAVE, &CImageProcessView::OnFileSave)
//ON_COMMAND(ID_FILE_OPEN, &CImageProcessView::OnFileOpen)
//ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
//ON_COMMAND(ID_FILE_SAVE, &CImageProcessView::OnFileSave)
//ON_COMMAND(ID_FILE_OPEN, &CImageProcessView::OnFileOpen)
ON_COMMAND(ID_FILE_SAVE, &CImageProcessView::OnFileSave)
END_MESSAGE_MAP()

// CImageProcessView ����/����

CImageProcessView::CImageProcessView()
{
	// TODO: �ڴ˴���ӹ������

}

CImageProcessView::~CImageProcessView()
{
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CImageProcessView ����

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	fileName  = pDoc->GetTitle();
	//extension = pDoc->GetPathName();
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	HDC hDC=pDC->GetSafeHdc();
	//CRect rect;
	//GetClientRect(&rect);
	//GetDlgItem(IDR_MAINFRAME)->GetClientRect(&rect);
	//float widthCoff = (float)rect.Width() / srcImg.cols;
	//float heightCoff = (float)rect.Height() / srcImg.rows;
	//float result = widthCoff < heightCoff ? widthCoff:heightCoff;
	//int widthResult = srcImg.cols*result;
	//int heightResult = srcImg.rows*result;
	//resize(srcImg,tempImg,Size(widthResult,heightResult));
	//int tlx = rect.TopLeft().x;
	//int tly = rect.TopLeft().y;
	//int brx = tlx + tempImg.cols - 1;
	//int bry = tly + tempImg.rows - 1;
	//rect.SetRect(tlx, tly, brx, bry);
	//image.DrawToHDC(hDC,&rect);
	
	//CvvImage &img = pDoc->DocImage;
	IplImage img = tempImg;
	image.CopyOf(&img);
	CRect rt;
	GetClientRect(&rt);
	if(ViewScale == 1)	CenterPoint = CPoint(rt.Width()/2,rt.Height());
	int imageX = image.Width()*ViewScale;
	int imageY = image.Height()*ViewScale;
	
	int rectL,rectT,rectR,rectB;
	if(rt.Width()>imageX)
	{
		scaleFlag = 1;
		sizeTotal = CSize(imageX,imageY);
		rectL = (rt.Width()-imageX)/2;
		rectR = (rt.Width()+imageX)/2;
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
	}
	else
	{
		scaleFlag = 0;
		rectL = 0;
		rectR = rt.Width();
		rectT = 0;
		rectB = imageY;
	}
	CRect rect(rectL,rectT,rectR,rectB);
	SetScrollSizes(MM_TEXT,sizeTotal);
	image.DrawToHDC(hDC,&rect);
}

void CImageProcessView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
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


//void CImageProcessView::OnFileOpen()
//{
//	 //TODO: �ڴ���������������
//	TCHAR szFilter[] = _T("JPEG�ļ�(*.jpeg;*.jpg;*,jpe;*jp2)|*.jpeg;*.jpg;*,jpe;*jp2|PNG�ļ�(*.png)|*.png|Windowsλͼ(*.bmp;*.dib)|*.bmp;*.dib|TIFF�ļ�(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters��դ�ļ�(*.sr;*.ras)|*.sr;*.ras|��Я�ļ���ʽ(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
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
//		imshow("��ԭʼͼ��",srcImg);
//		IplImage src = srcImg;
//		image.CopyOf(&src,1);
//	}
//}


void CImageProcessView::OnFileSave()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		TCHAR szFilter[] = _T("JPEG�ļ�(*.jpg;*,jpe;*.jpeg;*jp2)|*.jpg;*,jpe;*.jpeg;*jp2|PNG�ļ�(*.png)|*.png|Windowsλͼ(*.bmp;*.dib)|*.bmp;*.dib|TIFF�ļ�(*.tiff;*.tif)|*.tiff;*.tif|Sun Rasters��դ�ļ�(*.sr;*.ras)|*.sr;*.ras|��Я�ļ���ʽ(*.pbm;*.pgm;*.ppm)|*.pbm;*.pgm;*.ppm||"); 
		CFileDialog fileDlg(FALSE,extension,fileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,this);
		CString filePath;
		if(fileDlg.DoModal() == IDOK)
		{
			filePath = fileDlg.GetPathName();
			OnFileSave();
			//CStringA file(filePath.GetBuffer(0));
			//filePath.ReleaseBuffer();
			//string path=file.GetBuffer(0);
			//file.ReleaseBuffer();
			//if(!dstImg.data)	dstImg = srcImg.clone();
			//imwrite(path,dstImg);
		}
	}
	else
	{
		MessageBox(_T("û������ͼƬ��"),_T("����"),MB_ICONWARNING);
	}
}


void CImageProcessView::OnProcessFilterBox()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		tempImg = srcImg.clone();
		boxFilter(srcImg,tempImg,-1,Size(boxFilterValue+1,boxFilterValue+1));
		//imshow("�������˲���",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterMean()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		blur(srcImg,tempImg,Size(meanFilterValue+1,meanFilterValue+1));
		//imshow("����ֵ�˲���",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterGaussian()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		GaussianBlur(srcImg,tempImg,Size(gaussianFilterValue*2+1,gaussianFilterValue*2+1),0,0);
		//imshow("����˹�˲���",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterMedian()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		medianBlur(srcImg,tempImg,medianFilterValue*2+1);
		//imshow("����ֵ�˲���",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessFilterBilateral()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		bilateralFilter(srcImg,tempImg,bilateralFilterValue,bilateralFilterValue*2,bilateralFilterValue/2);
		//imshow("��˫���˲���",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessMorphologyDilate()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		dilate(srcImg,tempImg,element);
		//imshow("�����͡�",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyErode()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		erode(srcImg,tempImg,element);
		//imshow("����ʴ��",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyOpen()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_OPEN,element);
		//imshow("�������㡿",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyClose()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_CLOSE,element);
		//imshow("�������㡿",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyGradient()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_GRADIENT,element);
		//imshow("���ݶ����㡿",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyTophat()
{
	// TODO: �ڴ���������������
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_TOPHAT,element);
		//imshow("����ñ���㡿",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


void CImageProcessView::OnProcessMorphologyBlackhat()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat element = getStructuringElement(elementShape,Size(structElementSize*2+1,structElementSize*2+1),Point(structElementSize,structElementSize));
		morphologyEx(srcImg,tempImg,MORPH_BLACKHAT,element);
		//imshow("����ñ���㡿",tempImg);
		Invalidate();
		UpdateWindow();
		element.release();
	}
}


//void CImageProcessView::OnProcessFloodfill()
//{
//	// TODO: �ڴ���������������
//}


void CImageProcessView::OnProcessPyramid()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		if(pyr == 1)	tempImg = srcImg.clone();
		if((pyr>1)&&(pyr<10))	pyrUp(srcImg,tempImg,Size((int)(srcImg.cols*pyr),(int)(srcImg.cols*pyr)));
		else if((pyr<1)&&(pyr>0))	pyrDown(srcImg,tempImg,Size((int)(srcImg.cols*pyr),(int)(srcImg.cols*pyr)));
		//imshow("����������",tempImg);
		Invalidate();
		UpdateWindow();
	}
}

void CImageProcessView::OnProcessThreshold()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		threshold(srcImg,tempImg,threshValue,maxValue,thresholdType);
		//imshow("����ֵ����",tempImg);
		Invalidate();
		UpdateWindow();
	}
}


void CImageProcessView::OnProcessThresholdAdaptive()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat grayImg;
		cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
		//imshow("���Ҷ�ͼ��",grayImg);
		adaptiveThreshold(grayImg,tempImg,maxValue,adaptiveMethod,thresholdType,blockSize,C);
		//imshow("������Ӧ��ֵ����",tempImg);
		Invalidate();
		UpdateWindow();
		grayImg.release();
	}
}


void CImageProcessView::OnTransformEdgeCanny()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat mask;
		blur(srcImg,tempImg,Size(3,3));
		Canny(tempImg,mask,cannyThreshold,cannyThreshold*cannyRatio);
		tempImg=Scalar::all(0);
		srcImg.copyTo(tempImg,mask);
		//imshow("��Canny��Ե��⡿",tempImg);
		Invalidate();
		UpdateWindow();
		mask.release();
	}
}


void CImageProcessView::OnTransformEdgeSobel()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat sobelX,sobelY,sobelXAbs,sobelYAbs;
		Sobel(srcImg,sobelX,ddepth,1,0,sobelKernelSize*2+1,scale,delta);
		convertScaleAbs(sobelX,sobelXAbs);

		Sobel(srcImg,sobelY,ddepth,0,1,sobelKernelSize*2+1,scale,delta);
		convertScaleAbs(sobelY,sobelYAbs);

		addWeighted(sobelXAbs,0.5,sobelYAbs,0.5,0,tempImg);
		//imshow("��Sobel��Ե��⡿",tempImg);
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
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat grayImg;
		cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
		//imshow("���Ҷ�ͼ��",grayImg);
		GaussianBlur(grayImg,grayImg,Size(3,3),0,0);
		Laplacian(grayImg,tempImg,ddepth,laplacianKernelSize*2+1,scale,delta);
		//imshow("��Laplacian��Ե��⡿",tempImg);
		Invalidate();
		UpdateWindow();
		grayImg.release();
	}
}


void CImageProcessView::OnTransformEdgeScharr()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat scharrX,scharrY,scharrXAbs,scharrYAbs;
		Scharr(srcImg,scharrX,ddepth,1,0,scale,delta);
		convertScaleAbs(scharrX,scharrXAbs);

		Scharr(srcImg,scharrY,ddepth,0,1,scale,delta);
		convertScaleAbs(scharrY,scharrYAbs);

		addWeighted(scharrXAbs,0.5,scharrYAbs,0.5,0,tempImg);
		//imshow("��Scharr��Ե��⡿",tempImg);
		Invalidate();
		UpdateWindow();
		scharrX.release();
		scharrY.release();
		scharrXAbs.release();
		scharrYAbs.release();
	}
}


void CImageProcessView::OnUpdateProcessFilterBox(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessFilterMean(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessFilterMedian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessFilterBilateral(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyDilate(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyErode(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyOpen(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyClose(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyGradient(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyTophat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessMorphologyBlackhat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessPyramid(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessThreshold(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateProcessThresholdAdaptive(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateTransformEdgeCanny(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateTransformEdgeSobel(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateTransformEdgeLaplacian(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnUpdateTransformEdgeScharr(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(true);
}


void CImageProcessView::OnTransformHoughHoughlines()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat dst,midImg;
		Canny(srcImg,midImg,50,200,3);
		cvtColor(midImg,tempImg,CV_GRAY2BGR);
		tempImg.copyTo(dst);
		vector<Vec2f> lines;
		HoughLines(midImg,lines,1,CV_PI/180,200,0,0);
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
			line(dst,pt1,pt2,Scalar(0,0,255),1);
		}
		//imshow("����׼����任��",dst);
		Invalidate();
		UpdateWindow();
		dst.release();
		midImg.release();
	}
}


void CImageProcessView::OnTransformHoughHoughlinesp()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat dst,midImg;
		Canny(srcImg,midImg,50,200,3);
		cvtColor(midImg,tempImg,CV_GRAY2BGR);
		tempImg.copyTo(dst);
		vector<Vec4i> lines;
		HoughLinesP(midImg,lines,1,CV_PI/180,80,50,10);
		for(unsigned int i=0;i<lines.size();i++)
		{
			Vec4i l=lines[i];
			line(dst,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,255,20),1,CV_AA);
		}
		//imshow("���ۼƻ���任��",dst);
		Invalidate();
		UpdateWindow();
		dst.release();
		midImg.release();
	}
}


void CImageProcessView::OnTransformHoughHoughcircles()
{
	// TODO: �ڴ���������������
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
		//imshow("������Բ�任��",dst);
		Invalidate();
		UpdateWindow();
		grayImg.release();
		blurImg.release();
		dst.release();
	}
}


void CImageProcessView::OnTransformAffineWarp()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
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
		warpAffine(srcImg,tempImg,warpMat,srcImg.size());
		//imshow("��Warp��",tempImg);
		Invalidate();
		UpdateWindow();
		warpMat.release();
	}
}


void CImageProcessView::OnTransformAffineRotation()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat rotationMat(2,3,CV_32FC1);
		Point2f anchor((float)srcImg.cols/2,(float)srcImg.rows/2);
		rotationMat=getRotationMatrix2D(anchor,angle,scale);
		warpAffine(srcImg,tempImg,rotationMat,srcImg.size());
		//imshow("��WarpRotation��",tempImg);
		Invalidate();
		UpdateWindow();
		rotationMat.release();
	}
}


void CImageProcessView::OnTransformRemap()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
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
				case 1:if(i>srcImg.cols*0.25&&i<srcImg.cols*0.75&&j>srcImg.rows*0.25&&j<srcImg.rows*0.75)
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
				case 2:
					mapx.at<float>(j,i)=static_cast<float>(i);
					mapy.at<float>(j,i)=static_cast<float>(srcImg.rows-j);
					break;
				case 3:
					mapx.at<float>(j,i)=static_cast<float>(srcImg.cols-i);
					mapy.at<float>(j,i)=static_cast<float>(j);
					break;
				case 4:
					mapx.at<float>(j,i)=static_cast<float>(srcImg.cols-i);
					mapy.at<float>(j,i)=static_cast<float>(srcImg.rows-j);
					break;
				}
			}
		}
		remap(srcImg,tempImg,mapx,mapy,interWays,BORDER_CONSTANT,Scalar(0,0,0));
		//imshow("����ӳ�䡿",tempImg);
		Invalidate();
		UpdateWindow();
		mapx.release();
		mapy.release();
	}
}


void CImageProcessView::OnTransformEqualizehist()
{
	// TODO: �ڴ���������������
	if(srcImg.data)
	{
		Mat grayImg;
		cvtColor(srcImg,grayImg,COLOR_BGR2GRAY);
		equalizeHist(grayImg,tempImg);
		//imshow("��ֱ��ͼ���⻯��",tempImg);
		Invalidate();
		UpdateWindow();
	}
}

//void CImageProcessView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// �˹���Ҫ�� Windows Vista ����߰汾��
//	// _WIN32_WINNT ���ű��� >= 0x0600��
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL CImageProcessView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if((zDelta > 0)&&(scaleFlag == 1))
	{
		if(ViewScale+0.08 < 5)	ViewScale = ViewScale + 0.08;
	}
	else if(zDelta < 0)
	{
		if(ViewScale-0.02 > 0 )		ViewScale = ViewScale - 0.02;
	}
	else  return CView::OnMouseWheel(nFlags, zDelta, pt);
	CenterPoint = pt;
	Invalidate();
	UpdateWindow();
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}