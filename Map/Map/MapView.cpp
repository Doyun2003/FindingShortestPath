
// MapView.cpp: CMapView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Map.h"
#endif

#include "MapDoc.h"
#include "MapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define POINT_SIZE 10

// CMapView

IMPLEMENT_DYNCREATE(CMapView, CView)

BEGIN_MESSAGE_MAP(CMapView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMapView 생성/소멸

CMapView::CMapView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_map.LoadBitmap(IDB_MYMAP);
	m_map.GetBitmap(&m_mapInfo);
	m_selectPoint[0] = -1;
	m_selectPoint[1] = -1;
	m_polyLine.SetSize(1);
}

CMapView::~CMapView()
{
}

BOOL CMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapView 그리기

void CMapView::OnDraw(CDC* pDC)
{
	CMapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CBrush* pOldBrush = NULL;
	CPen* pOldPen = NULL;

	// 지도 그리기
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&m_map);
	pDC->BitBlt(0, 0, m_mapInfo.bmWidth, m_mapInfo.bmHeight, &memDC, 0, 0, SRCCOPY);

	// 점 그리기
	CBrush RedBrush(RGB(255, 0, 0));
	pOldBrush = pDC->SelectObject(&RedBrush);
	int pointNum = m_pointArr.GetCount();
	for (int i = 0; i < pointNum; i++) {
		pDC->Ellipse(m_pointArr[i].x - POINT_SIZE, m_pointArr[i].y - POINT_SIZE, m_pointArr[i].x + POINT_SIZE, m_pointArr[i].y + POINT_SIZE);
	}
	pDC->SelectObject(pOldBrush);

	// 점 선택
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->Ellipse(m_select.x - POINT_SIZE, m_select.y - POINT_SIZE, m_select.x + POINT_SIZE, m_select.y + POINT_SIZE);
	CPen bluePen(PS_SOLID, 2, RGB(0, 0, 255));
	pOldPen = pDC->SelectObject(&bluePen);
	for (int i = 0; i < 2; i++) {
		if (m_selectPoint[i] >= 0) {
			pDC->Ellipse(m_pointArr[m_selectPoint[i]].x - POINT_SIZE, m_pointArr[m_selectPoint[i]].y - POINT_SIZE,
				m_pointArr[m_selectPoint[i]].x + POINT_SIZE, m_pointArr[m_selectPoint[i]].y + POINT_SIZE);
		}
	}
	pDC->SelectObject(pOldPen);
	

	// 경로 그리기
	CPen RedPen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&RedPen);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 0));

	int polyNum = m_polyLine.GetCount();
	for (int i = 0; i < polyNum; i++) {
		int cnt = m_polyLine[i].m_points.GetCount();

		if (cnt >= 2) {
			pDC->Polyline(m_polyLine[i].m_points.GetData(), cnt);
		}

		for (int j = 0; j < cnt - 1; j++) {
			CPoint p1 = m_polyLine[i].m_points[j];
			CPoint p2 = m_polyLine[i].m_points[j + 1];

			double segmentDist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
			CString strLen;
			strLen.Format(_T("%.1f"), segmentDist);

			CSize textSize = pDC->GetTextExtent(strLen);

			int midX = (p1.x + p2.x) / 2;
			int midY = (p1.y + p2.y) / 2;

			int textX = midX - (textSize.cx / 2);
			int textY = midY - (textSize.cy / 2);

			pDC->SetTextColor(RGB(0, 0, 0));
			for (int dx = -2; dx <= 2; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dx == 0) continue;
					pDC->TextOutW(textX + dx, textY + dy, strLen);
				}
			}

			pDC->SetTextColor(RGB(100, 255, 255));
			pDC->TextOut(textX, textY, strLen);
		}
	}
	pDC->SetBkMode(oldBkMode);
	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(pOldPen);

	// 최단 경로 그리기
	if (m_shortestPath.size() >= 2) {
		CPen pathPen(PS_SOLID, 4, RGB(0, 255, 0));
		pOldPen = pDC->SelectObject(&pathPen);

		double totalShortestDistance = 0.0;

		for (size_t i = 0; i < m_shortestPath.size() - 1; i++) {
			int startIdx = m_shortestPath[i];
			int endIdx = m_shortestPath[i + 1];

			if (startIdx < m_pointArr.GetCount() && endIdx < m_pointArr.GetCount()) {
				CPoint p1 = m_pointArr[startIdx];
				CPoint p2 = m_pointArr[endIdx];

				pDC->MoveTo(m_pointArr[startIdx]);
				pDC->LineTo(m_pointArr[endIdx]);

				totalShortestDistance += sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
			}
		}
		pDC->SelectObject(pOldPen);
		CString shortestStr;
		shortestStr.Format(_T("최단경로 길이: %.1f"), totalShortestDistance);

		CPen edgePen;
		edgePen.CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		pDC->SelectObject(&edgePen);
		pDC->Rectangle(m_mapInfo.bmWidth - 200, m_mapInfo.bmHeight - 33, m_mapInfo.bmWidth, m_mapInfo.bmHeight);
		pDC->TextOut(m_mapInfo.bmWidth - 180, m_mapInfo.bmHeight - 25, shortestStr);
	}
}


// CMapView 인쇄

BOOL CMapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMapView 진단

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CView::AssertValid();
}

void CMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapDoc* CMapView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapDoc)));
	return (CMapDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapView 메시지 처리기


void CMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nFlags & MK_CONTROL) { // 지도에 점 찍기
		m_pointArr.Add(point);	
	}
	else if (GetKeyState(VK_MENU) < 0 && (nFlags & MK_SHIFT)) {
		int pointNum = m_pointArr.GetCount();
		for (int i = 0; i < pointNum; i++) {
			CRgn rgn;
			rgn.CreateEllipticRgn(m_pointArr[i].x - POINT_SIZE, m_pointArr[i].y - POINT_SIZE, m_pointArr[i].x + POINT_SIZE, m_pointArr[i].y + POINT_SIZE);
			if (rgn.PtInRegion(point)) {
				m_selectPoint[0] = m_selectPoint[1];
				m_selectPoint[1] = i;	// 왼쪽 시프트

				if (m_selectPoint[0] != -1 && m_selectPoint[1] != -1) {
					m_shortestPath = CMyLine::FindShortestPath(
						m_pointArr,
						m_polyLine,
						m_selectPoint[0],
						m_selectPoint[1]
					);

					if (m_shortestPath.empty()) {
						MessageBox(_T("연결된 경로가 없습니다."));
					}
				}
			}
		}
	}
	else if (GetKeyState(VK_MENU) < 0) {
		int pointNum = m_pointArr.GetCount();
		for (int i = 0; i < pointNum; i++) {
			CRgn rgn;
			rgn.CreateEllipticRgn(m_pointArr[i].x - POINT_SIZE, m_pointArr[i].y - POINT_SIZE, m_pointArr[i].x + POINT_SIZE, m_pointArr[i].y + POINT_SIZE);
			if (rgn.PtInRegion(point)) {
				m_select = m_pointArr[i];
				int index = m_polyLine.GetCount();
				if (index-- > 0) 
					m_polyLine[index].m_points.Add(m_pointArr[i]);
			}
		}
	}


	CView::OnLButtonDown(nFlags, point);
	Invalidate();
}


void CMapView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_selectPoint[0] = -1;
	m_selectPoint[1] = -1;

	m_shortestPath.clear();

	m_polyLine.SetSize(m_polyLine.GetCount() + 1);

	CView::OnLButtonDblClk(nFlags, point);
	Invalidate();
}


void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar) {
	case VK_DELETE: {
		int line_idx = m_polyLine.GetCount();
		if (GetKeyState(VK_CONTROL) < 0) {
			if (line_idx > 0) {
				int point_idx = m_polyLine[line_idx - 1].m_points.GetCount();
				if (point_idx == 0) {
					m_polyLine.SetSize(line_idx - 1);
				}
			}
		}
		else {
			if (line_idx > 0) {
				int point_idx = m_polyLine[line_idx - 1].m_points.GetCount();
				if (point_idx > 0)
					m_polyLine[line_idx - 1].m_points.RemoveAt(point_idx - 1, 1);
			}
		}

		Invalidate();
		break;
	}
	case VK_BACK: {
		bool bHasPath = false;
		int polyCount = m_polyLine.GetCount();

		for (int i = 0; i < polyCount; i++) {
			if (m_polyLine[i].m_points.GetCount() > 0) {
				bHasPath = true;
				break;
			}
		}

		int pointCount = m_pointArr.GetCount();
		if (pointCount > 0) {
			m_pointArr.RemoveAt(pointCount - 1);
		}
		Invalidate();
		break;
	}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}