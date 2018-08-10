
// ImageProcessView.h : CImageProcessView ��Ľӿ�
//
//#include "DlgFilter.h"
//#include "DlgMorphology.h"
#pragma once

class CImageProcessView : public CScrollView
{
//private:
//	CDlgFilter *m_pDlgFilter;
protected: // �������л�����
	CImageProcessView();
	DECLARE_DYNCREATE(CImageProcessView)

// ����
public:
	CImageProcessDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnFileOpen();
//	afx_msg void OnFileSave();
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
//	afx_msg void OnProcessFloodfill();
	afx_msg void OnProcessPyramid();
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
	afx_msg void OnUpdateProcessPyramid(CCmdUI *pCmdUI);
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
	afx_msg void OnTransformAffineRotation();
	afx_msg void OnTransformRemap();
	afx_msg void OnTransformEqualizehist();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileSave();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateTransformHoughHoughlines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformHoughHoughlinesp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformHoughHoughcircles(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformAffineWarp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformAffineRotation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformRemap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransformEqualizehist(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnProcessFloodfill();
	afx_msg void OnUpdateProcessFloodfill(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // ImageProcessView.cpp �еĵ��԰汾
inline CImageProcessDoc* CImageProcessView::GetDocument() const
   { return reinterpret_cast<CImageProcessDoc*>(m_pDocument); }
#endif

