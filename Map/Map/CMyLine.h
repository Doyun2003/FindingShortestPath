#pragma once
#include <vector>

class CMyLine
{
    // 하나의 선(Polyline)을 나타내는 클래스
public:
    CArray<CPoint, CPoint> m_points; // 실제 점들이 저장될 배열

    // [핵심] 대입 연산자 오버로딩 (이게 있어야 2차원 배열이 가능해짐)
    // 다른 CMyLine을 내 안에 복사해 넣는 방법 정의
    const CMyLine& operator=(const CMyLine& src)
    {
        m_points.Copy(src.m_points); // CArray의 Copy 함수를 이용해 내용물 복사
        return *this;
    }

    static std::vector<int> FindShortestPath(
        const CArray<CPoint, CPoint>& allPoints,
        const CArray<CMyLine, CMyLine&>& allLines,
        int startNode,
        int endNode
    );
};