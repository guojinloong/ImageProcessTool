// MDIClient.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "MDIClient.h"


// CMDIClient

IMPLEMENT_DYNAMIC(CMDIClient, CWnd)

CMDIClient::CMDIClient()
{

}

CMDIClient::~CMDIClient()
{
}


BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMDIClient 消息处理程序




BOOL CMDIClient::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 CBrush backBrush;
	 backBrush.CreateSolidBrush(RGB(255,0,255));

	//保存旧的brush
	 CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	 CRect rect;
	 pDC->GetClipBox(&rect);

	//画需要的区域
	 pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	 pDC->SelectObject(pOldBrush);

	return CWnd::OnEraseBkgnd(pDC);
}
