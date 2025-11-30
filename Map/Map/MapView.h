
// MapView.h: CMapView 클래스의 인터페이스
//

#pragma once
#include "CMyLine.h"

class CMapView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMapView() noexcept;
	DECLARE_DYNCREATE(CMapView)

// 특성입니다.
public:
	CMapDoc* GetDocument() const;
protected:
	CBitmap m_map;
	BITMAP m_mapInfo;

	CArray<CPoint, CPoint> m_pointArr;
	int m_selectPoint[2];
	CArray<CMyLine, CMyLine&> m_polyLine;

	std::vector<int> m_shortestPath;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MapView.cpp의 디버그 버전
inline CMapDoc* CMapView::GetDocument() const
   { return reinterpret_cast<CMapDoc*>(m_pDocument); }
#endif

