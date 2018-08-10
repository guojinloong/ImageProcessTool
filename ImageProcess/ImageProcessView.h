
// ImageProcessView.h : CImageProcessView 类的接口

#include "DlgProperty.h"
#include "DlgSet.h"
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
//#include "DlgPicture.h"
//#include "DlgAffineWarpRotation.h"
#include "DlgRemap.h"
#include "DlgFloodFill.h"
#include "DlgResize.h"
#include "DlgDefog.h"
#include "DlgFindContours.h"
#include "DlgConvexHull.h"
#include "DlgPoly.h"

#pragma once

class CImageProcessView : public CScrollView
{
private:
	CDlgSet *m_pDlgSet;
	//CDlgSet m_dlgSet;
	CDlgFilter *m_pDlgFilter;
	CDlgFloodFill *m_pDlgFloodFill;
protected: // 仅从序列化创建
	bool m_bDraw;
	HCURSOR m_hCursor;
	CPoint m_pOld;
	CImageProcessView();
	DECLARE_DYNCREATE(CImageProcessView)

// 特性
public:
	CImageProcessDoc* GetDocument() const;

// 操作
public:
	HACCEL hAccel;
	COLORREF color;//背景颜色
	CFont font;//字体信息
	int thickness;
	int lineType;
	int shift;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnProcessFilterBox();
	afx_msg void OnProcessFilterMean();
	afx_msg void OnProcessFilterGaussian();
	afx_msg void OnProcessFilterMedian();
	afx_msg void OnProcessFilterBilateral();
	afx_msg void OnProcessMorphologyDilate();
	afx_msg void OnProcessMorphologyErode();
	afx_msg void OnProcessMorphologyOpen();
	afx_msg void OnProcessMorphologyClose();
	afx_msg void OnProcessMorphologyGradient();
	afx_msg void OnProcessMorphologyTophat();
	afx_msg void OnProcessMorphologyBlackhat();
	afx_msg void OnProcessFloodfill();
	afx_msg void OnProcessThreshold();
	afx_msg void OnProcessThresholdAdaptive();
	afx_msg void OnTransformEdgeCanny();
	afx_msg void OnTransformEdgeSobel();
	afx_msg void OnTransformEdgeLaplacian();
	afx_msg void OnTransformEdgeScharr();
	afx_msg void OnUpdateProcessFilterBox(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessFilterMean(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessFilterGaussian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessFilterMedian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessFilterBilateral(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyDilate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyErode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyClose(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyGradient(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyTophat(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessMorphologyBlackhat(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessThreshold(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessThresholdAdaptive(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformEdgeCanny(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformEdgeSobel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformEdgeLaplacian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformEdgeScharr(CCmdUI *pCmdUI);
	afx_msg void OnTransformHoughHoughlines();
	afx_msg void OnTransformHoughHoughlinesp();
	afx_msg void OnTransformHoughHoughcircles();
	afx_msg void OnTransformAffineWarp();
	//afx_msg void OnTransformAffineRotation();
	afx_msg void OnTransformRemap();
	afx_msg void OnTransformEqualizehist();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateTransformHoughHoughlines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformHoughHoughlinesp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformHoughHoughcircles(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformAffineWarp(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateTransformAffineRotation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformRemap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformEqualizehist(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProcessFloodfill(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnProcessPyramidUp();
	afx_msg void OnUpdateProcessPyramidUp(CCmdUI *pCmdUI);
	afx_msg void OnProcessPyramidDown();
	afx_msg void OnUpdateProcessPyramidDown(CCmdUI *pCmdUI);
	afx_msg void OnProcessResize();
	afx_msg void OnUpdateProcessResize(CCmdUI *pCmdUI);
	afx_msg void OnAdvancedDefog();
	afx_msg void OnUpdateAdvancedDefog(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContoursFindcontours();
	afx_msg void OnUpdateContoursFindcontours(CCmdUI *pCmdUI);
	afx_msg void OnContoursConvexhull();
	afx_msg void OnUpdateContoursConvexhull(CCmdUI *pCmdUI);
	afx_msg void OnContoursPoly();
	afx_msg void OnUpdateContoursPoly(CCmdUI *pCmdUI);
	afx_msg void OnEditSet();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnEditProperty();
	afx_msg void OnUpdateEditProperty(CCmdUI *pCmdUI);
	afx_msg void OnContoursMoments();
	afx_msg void OnUpdateContoursMoments(CCmdUI *pCmdUI);
	afx_msg void OnContoursWatershed();
	afx_msg void OnUpdateContoursWatershed(CCmdUI *pCmdUI);
	afx_msg void OnAdvancedInpaint();
	afx_msg void OnUpdateAdvancedInpaint(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // ImageProcessView.cpp 中的调试版本
inline CImageProcessDoc* CImageProcessView::GetDocument() const
   { return reinterpret_cast<CImageProcessDoc*>(m_pDocument); }
#endif

