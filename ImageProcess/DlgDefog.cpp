// DlgDefog.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DlgDefog.h"
#include "afxdialogex.h"

extern Mat srcImg,tempImg;

// CDlgDefog 对话框

IMPLEMENT_DYNAMIC(CDlgDefog, CDialogEx)

CDlgDefog::CDlgDefog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDefog::IDD, pParent)
	, m_minWindowSize(15)
	, m_radius(5)
	, m_epsilon(0.001)
	, m_A(250)
	, m_algorithm(0)
	, m_guide(0)
	, m_depth(CV_8U)
	, m_maxValue(0)
{

}

CDlgDefog::~CDlgDefog()
{
}

void CDlgDefog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox1);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox2);
	DDX_Control(pDX, IDC_COMBO3, m_comboBox3);
	//DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollBar1);
	//DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollBar2);
	//DDX_Control(pDX, IDC_SCROLLBAR3, m_scrollBar3);
	DDX_Text(pDX, IDC_EDIT1, m_radius);
	DDV_MinMaxInt(pDX, m_radius, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_A);
	DDV_MinMaxInt(pDX, m_A, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_epsilon);
	DDV_MinMaxDouble(pDX, m_epsilon, 0.001, 0.05);
	DDX_Text(pDX, IDC_EDIT4, m_minWindowSize);
	DDV_MinMaxInt(pDX, m_minWindowSize, 3, 200);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_SLIDER4, m_slider2);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3);
	DDX_Control(pDX, IDC_CHECK1, m_caculateCheck);
	DDX_Control(pDX, IDC_EDIT2, m_edit1);
}


BEGIN_MESSAGE_MAP(CDlgDefog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgDefog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgDefog::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgDefog::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgDefog::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgDefog::OnCbnSelchangeCombo3)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgDefog::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgDefog::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT3, &CDlgDefog::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgDefog::OnEnChangeEdit4)
//	ON_WM_HSCROLL()
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgDefog::OnNMCustomdrawSlider1)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CDlgDefog::OnNMCustomdrawSlider4)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CDlgDefog::OnNMCustomdrawSlider3)
ON_BN_CLICKED(IDC_CHECK1, &CDlgDefog::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CDlgDefog 消息处理程序


BOOL CDlgDefog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(true);
	m_comboBox1.AddString(_T("暗通道优先法"));
	m_comboBox1.SetCurSel(0);
	m_comboBox2.AddString(_T("灰度图"));
	m_comboBox2.AddString(_T("彩色图"));
	m_comboBox2.SetCurSel(0);
	if(srcImg.depth() == CV_8U)
	{
		m_comboBox3.AddString(_T("CV_8U"));
		m_comboBox3.AddString(_T("CV_16S"));
		m_comboBox3.AddString(_T("CV_32F"));
		m_comboBox3.AddString(_T("CV_64F"));
	}
	else if((srcImg.depth() == CV_16U)||(srcImg.depth() == CV_16S))
	{
		m_comboBox3.AddString(_T("CV_16U"));
		m_comboBox3.AddString(_T("CV_16S"));
		m_comboBox3.AddString(_T("CV_32F"));
		m_comboBox3.AddString(_T("CV_64F"));
	}
	else if(srcImg.depth() == CV_32F)
	{
		m_comboBox3.AddString(_T("CV_32F"));
		m_comboBox3.AddString(_T("CV_64F"));
	}
	else if(srcImg.depth() == CV_64F)
	{
		m_comboBox3.AddString(_T("CV_64F"));
	}
	m_comboBox3.SetCurSel(0);
	
	//m_scrollBar1.SetScrollRange(0,100);
	//m_scrollBar1.SetScrollPos(5);
	//m_scrollBar2.SetScrollRange(1,50);
	//m_scrollBar2.SetScrollPos(2);
	//m_scrollBar3.SetScrollRange(3,200);
	//m_scrollBar3.SetScrollPos(150);
	m_slider1.SetRange(3,200);
	m_slider1.SetTicFreq(1);
	m_slider1.SetPos(m_minWindowSize);
	m_slider2.SetRange(5,100);
	m_slider2.SetTicFreq(1);
	m_slider2.SetPos(m_radius);
	m_slider3.SetRange(1,50);
	m_slider3.SetTicFreq(1);
	m_slider3.SetPos((int)(m_epsilon*1000));

	m_caculateCheck.SetCheck(0);

	m_progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_progress->SetRange(0,100);
	m_progress->SetStep(10);
	m_progress->SetPos(0);
	UpdateData(false);
	
	IplImage img = srcImg;
	image.CopyOf(&img);
	CRect rt;
	GetDlgItem(IDC_STATIC_SHOW)->GetClientRect(&rt);
	float ViewScale = 1;
	if((image.Width() > rt.Width())||(image.Height() > rt.Height()))
	{
		float widthCoff = (float)rt.Width() / image.Width();
		float heightCoff = (float)rt.Height() / image.Height();
		ViewScale = widthCoff < heightCoff ? widthCoff:heightCoff;
	}
	int imageX = (int)(image.Width()*ViewScale);
	int imageY = (int)(image.Height()*ViewScale);
	rect = CRect((rt.Width()-imageX)/2+1,(rt.Height()-imageY)/2+1,(rt.Width()+imageX)/2-1,(rt.Height()+imageY)/2-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgDefog::ContinueModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	IplImage img = tempImg;
	image.CopyOf(&img);
	CDC* pDC = GetDlgItem( IDC_STATIC_SHOW )->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	image.DrawToHDC(hDC,&rect);
	return CDialogEx::ContinueModal();
}


void CDlgDefog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_progress->SetPos(0);
	srcImg.copyTo(tempImg);
}


void CDlgDefog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_progress->SetPos(0);
	Mat gray,dst,luvImg,minImg,guideImg;
	int width = srcImg.cols;
	int height = srcImg.rows;
	cvtColor(srcImg,gray,COLOR_BGR2GRAY);
	cvtColor(srcImg,minImg,COLOR_BGR2GRAY);
	//gray.create(height,width,CV_32FC1);
	//minImg.create(height,width,CV_32FC1);
	double t0 = static_cast<double>(getTickCount());
	for(int i=0;i<height;i++)
	{
		uchar *data = gray.ptr<uchar>(i);
		for(int j=0;j<width;j++)
		{
			uchar *pixel = srcImg.ptr<uchar>(i,j);
			data[j] = pixel[0]<pixel[1]?pixel[0]:pixel[1];
			data[j] = data[j]<pixel[2]?data[j]:pixel[2];
		}
	}
	m_progress->SetPos(10);
	//destroyAllWindows();
	//imshow("gray",gray);
	for(int i=0;i<height;i++)
	{
		uchar *data = minImg.ptr<uchar>(i);
		for(int j=0;j<width;j++)
		{
			Rect rect1(0,0,gray.cols,gray.rows);
			Rect rect2(i-m_minWindowSize/2,j-m_minWindowSize/2,m_minWindowSize,m_minWindowSize);
			Rect rect;
			rect.x = max(rect1.x,rect2.x);
			rect.y = max(rect1.y,rect2.y);
			rect.width = min(rect1.y+rect1.width,rect2.y+rect2.width)-rect.y-1;
			rect.height = min(rect1.x+rect1.height,rect2.x+rect2.height)-rect.x-1;
			int minVal = 255;
			for(int k=rect.x;k<rect.x+rect.height;k++)
			{
				uchar *data1 = gray.ptr<uchar>(k);
				for(int l=rect.y;l<rect.y+rect.width;l++)
				{
					if(data1[l]<minVal)	minVal = data1[l];
					//IplImage img = gray;
					//minVal = minVal<CV_IMAGE_ELEM(&img,uchar,k,l)?minVal:CV_IMAGE_ELEM(&img,uchar,k,l);
				}
			}
			data[j] = minVal;
		}
	}
	m_progress->SetPos(30);
	//imshow("minImg",minImg);
	//imshow("minFilter",minImg);
	//vector<Mat> channels;
	//split(minImg,channels);
	//channels.at(0) = guidedFilter(minImg,channels.at(0),m_radius,m_epsilon);
	//channels.at(1) = guidedFilter(minImg,channels.at(1),m_radius,m_epsilon);
	//channels.at(2) = guidedFilter(minImg,channels.at(2),m_radius,m_epsilon);
	//merge(channels,guideImg);
	//imshow("guideFilter",guideImg);
	int count = width*height/1000;
	int *map = new int[count];
	int maxVal(0),maxI(0),maxJ(0);
	for(int c=0;c<count;c++)
	{
		maxVal = 0;
		for(int i=0;i<height;i++)
		{
			uchar *data = minImg.ptr<uchar>(i);
			for(int j=0;j<width;j++)
			{
				if(data[j]>maxVal)
				{
					maxVal = data[j];
					maxI = i;
					maxJ = j;
				}
			}
		}
		map[c] = maxI*width+maxJ+1;
		uchar *data = gray.ptr<uchar>(maxI);
		data[maxJ] = 0;
	}
	m_maxValue = 0;
	//int maxVal0 = 0;
	//int maxVal1 = 0;
	//int maxVal2 = 0;
	m_progress->SetPos(50);
	cvtColor(srcImg,luvImg,COLOR_BGR2Luv);
	//imshow("Luv",luvImg);
	for(int c=0;c<count;c++)
	{
		if(luvImg.at<Vec3b>(map[c]/width,map[c]%width-1)[0]>m_maxValue)	m_maxValue = luvImg.at<Vec3b>(map[c]/width,map[c]%width-1)[0];
		//if(srcImg.at<Vec3b>(map[c]/width,map[c]%width-1)[0]>maxVal0)	maxVal0 = srcImg.at<Vec3b>(map[c]/width,map[c]%width-1)[0];
		//if(srcImg.at<Vec3b>(map[c]/width,map[c]%width-1)[1]>maxVal1)	maxVal1 = srcImg.at<Vec3b>(map[c]/width,map[c]%width-1)[1];
		//if(srcImg.at<Vec3b>(map[c]/width,map[c]%width-1)[2]>maxVal2)	maxVal2 = srcImg.at<Vec3b>(map[c]/width,map[c]%width-1)[2];
	}
	//maxVal = maxVal0>maxVal1?maxVal0:maxVal1;
	//maxVal = maxVal>maxVal2?maxVal:maxVal2;
	//m_A = m_A<maxVal?m_A:maxVal;
	delete []map;
	if(m_caculateCheck.GetCheck())	m_A = m_maxValue;
	m_progress->SetPos(80);
	dst = srcImg.clone();
	for(int i=0;i<height;i++)
	{
		uchar *data = minImg.ptr<uchar>(i);
		for(int j=0;j<width;j++)
		{
			float t = 1-(float)data[j]/m_A;
			if(t<0)	t = 0;
			else if(t>1)	t=1;
			dst.at<Vec3b>(i,j)[0] = (uchar)((float)(srcImg.at<Vec3b>(i,j)[0]-m_A)/t + m_A);
			dst.at<Vec3b>(i,j)[1] = (uchar)((float)(srcImg.at<Vec3b>(i,j)[1]-m_A)/t + m_A);
			dst.at<Vec3b>(i,j)[2] = (uchar)((float)(srcImg.at<Vec3b>(i,j)[2]-m_A)/t + m_A);
			if(dst.at<Vec3b>(i,j)[0]>255)	dst.at<Vec3b>(i,j)[0] = 255;
			else if(dst.at<Vec3b>(i,j)[0]<0)	dst.at<Vec3b>(i,j)[0] = 0;
			if(dst.at<Vec3b>(i,j)[1]>255)	dst.at<Vec3b>(i,j)[1] = 255;
			else if(dst.at<Vec3b>(i,j)[1]<0)	dst.at<Vec3b>(i,j)[1] = 0;
			if(dst.at<Vec3b>(i,j)[2]>255)	dst.at<Vec3b>(i,j)[2] = 255;
			else if(dst.at<Vec3b>(i,j)[2]<0)	dst.at<Vec3b>(i,j)[2] = 0;
		}
	}
	//imshow("Defog",dst);
	m_progress->SetPos(100);
	double t1 = static_cast<double>(getTickCount());
	double t = (t1-t0)/getTickFrequency();
	tempImg = dst.clone();
	CString time;
	if(t<0.001)
	{
		t*=1000;
		time.Format(_T("Time:%fus"),(float)t);
	}
	else if(t<1)
	{
		t*=1000;
		time.Format(_T("Time:%fms"),(float)t);
	}
	else	time.Format(_T("Time:%fs"),(float)t);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(time);
	//CString A;
	//A.Format(_T("计算值：%d"),maxVal);
	//GetDlgItem(IDC_STATIC_A)->SetWindowText(A);
	gray.release();
	dst.release();
	minImg.release();
	guideImg.release();
	UpdateData(false);
}


void CDlgDefog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_caculateCheck.GetCheck())
	{
		if(m_maxValue!=0)	m_A = m_maxValue;
		m_edit1.SetReadOnly(1);
	}
	else
	{
		m_edit1.SetReadOnly(0);
	}
	UpdateData(false);
}


void CDlgDefog::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox1.GetCurSel();
	CString string;
	m_comboBox1.GetLBText(sel,string);
	if(string == _T("暗通道优先法"))	m_algorithm = 0;
}


void CDlgDefog::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox2.GetCurSel();
	CString string;
	m_comboBox2.GetLBText(sel,string);
	if(string == _T("灰度图"))	m_guide = 0;
	else if(string == _T("彩色图"))	m_guide = 1;
}


void CDlgDefog::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboBox3.GetCurSel();
	CString string;
	m_comboBox3.GetLBText(sel,string);
	if(string == _T("CV_8U"))	m_depth = CV_8U;
	else if(string == _T("CV_16U"))	m_depth = CV_16U;
	else if(string == _T("CV_16S"))	m_depth = CV_16S;
	else if(string == _T("CV_32F"))	m_depth = CV_32F;
	else if(string == _T("CV_64F"))	m_depth = CV_64F;
}


void CDlgDefog::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_radius<5)	m_radius = 5;
	else if(m_radius>100)	m_radius = 100;
	m_slider2.SetPos(m_radius);
	UpdateData(false);
}


void CDlgDefog::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_A<0)	m_A = 0;
	else if(m_A>255)	m_A = 255;
	UpdateData(false);
}


void CDlgDefog::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_epsilon<0.001)	m_epsilon = 0.001;
	else if(m_epsilon>0.05)	m_epsilon = 0.05;
	m_slider3.SetPos((int)(m_epsilon*1000));
	UpdateData(false);
}


void CDlgDefog::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_minWindowSize<3)	m_minWindowSize = 3;
	else if(m_minWindowSize>200)	m_minWindowSize = 200;
	m_slider1.SetPos(m_minWindowSize);
	UpdateData(false);
}

//void CDlgDefog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	int pos1 = m_scrollBar1.GetScrollPos();
//	switch(nSBCode)
//	{
//	case SB_LINEUP:
//		pos1 -= 1;
//		break;
//	case SB_LINEDOWN:
//		pos1 += 1;
//		break;
//	case SB_PAGEUP:
//		pos1 -= 10;
//		break;
//	case SB_PAGEDOWN:
//		pos1 += 10;
//		break;
//	case SB_TOP:
//		pos1 = 0;
//		break;
//	case SB_BOTTOM:
//		pos1 = 100;
//		break;
//	case SB_THUMBPOSITION:
//		pos1 = nPos;
//		break;
//	default:
//		;
//	}
//	m_scrollBar1.SetScrollPos(pos1);
//	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
//}


void CDlgDefog::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_minWindowSize = m_slider1.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgDefog::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_radius = m_slider2.GetPos();
	UpdateData(false);
	*pResult = 0;
}


void CDlgDefog::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_epsilon = (double)m_slider3.GetPos()/1000;
	UpdateData(false);
	*pResult = 0;
}


Mat guidedFilter( Mat I, Mat p, int r, double eps)
{
    int hei = I.rows;
    int wid = I.cols;
    //N = boxfilter(ones(hei, wid), r);
    Mat one = Mat::ones(hei, wid, CV_64FC1);
    Mat M,N;
	boxFilter(one,N,CV_64FC1,Size(r,r));
  
    //mean_I = boxfilter(I, r) ./ N;
    Mat mean_I(hei, wid, CV_64FC1);
	boxFilter(I,M,-1,Size(r,r));
    divide(M, N, mean_I);
  
    //mean_p = boxfilter(p, r) ./ N;
    Mat mean_p(hei, wid, CV_64FC1);
	boxFilter(p,M,-1,Size(r,r));
    divide(M, N, mean_p);
  
    //mean_Ip = boxfilter(I.*p, r) ./ N;
    Mat mul_Ip(hei, wid, CV_64FC1);
    Mat mean_Ip(hei, wid, CV_64FC1);
    multiply(I,p,mul_Ip);
	boxFilter(mul_Ip,M,-1,Size(r,r));
    divide(M, N, mean_Ip);  
  
    //cov_Ip = mean_Ip - mean_I .* mean_p  
    //this is the covariance of (I, p) in each local patch.
    Mat mul_mean_Ip(hei, wid, CV_64FC1);
    Mat cov_Ip(hei, wid, CV_64FC1);
    multiply(mean_I, mean_p, mul_mean_Ip);
    subtract(mean_Ip, mul_mean_Ip, cov_Ip);
  
    //mean_II = boxfilter(I.*I, r) ./ N;  
    Mat mul_II(hei, wid, CV_64FC1);
    Mat mean_II(hei, wid, CV_64FC1);
    multiply(I, I, mul_II);
	boxFilter(mul_II,M,-1,Size(r,r));
    divide(M, N, mean_II);
  
    //var_I = mean_II - mean_I .* mean_I;  
    Mat mul_mean_II(hei, wid, CV_64FC1);
    Mat var_I(hei, wid, CV_64FC1);
    multiply(mean_I, mean_I, mul_mean_II);
    subtract(mean_II, mul_mean_II, var_I);
  
    //a = cov_Ip ./ (var_I + eps);  
    Mat a(hei, wid, CV_64FC1);
    for( int i = 0; i< hei; i++){
        double *p = var_I.ptr<double>(i);
        for( int j = 0; j< wid; j++){
            p[j] = p[j] +eps;
        }
    }
    divide(cov_Ip, var_I, a);

    //b = mean_p - a .* mean_I;
    Mat a_mean_I(hei ,wid, CV_64FC1);
    Mat b(hei ,wid, CV_64FC1);
    multiply(a, mean_I, a_mean_I);
    subtract(mean_p, a_mean_I, b);

    //mean_a = boxfilter(a, r) ./ N;
    Mat mean_a(hei, wid, CV_64FC1);
	boxFilter(a,M,-1,Size(r,r));
    divide(M, N, mean_a);
    //mean_b = boxfilter(b, r) ./ N;
    Mat mean_b(hei, wid, CV_64FC1);
	boxFilter(b,M,-1,Size(r,r));
    divide(M, N, mean_b);

    //q = mean_a .* I + mean_b;
    Mat mean_a_I(hei, wid, CV_64FC1);
    Mat q(hei, wid, CV_64FC1);
    multiply(mean_a, I, mean_a_I);
    add(mean_a_I, mean_b, q);

    return q;
}  
