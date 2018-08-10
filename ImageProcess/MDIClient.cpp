// MDIClient.cpp : ʵ���ļ�
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



// CMDIClient ��Ϣ�������




BOOL CMDIClient::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	 CBrush backBrush;
	 backBrush.CreateSolidBrush(RGB(255,0,255));

	//����ɵ�brush
	 CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	 CRect rect;
	 pDC->GetClipBox(&rect);

	//����Ҫ������
	 pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	 pDC->SelectObject(pOldBrush);

	return CWnd::OnEraseBkgnd(pDC);
}
