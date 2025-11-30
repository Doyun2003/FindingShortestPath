# MFC Shortest Path Finder (최단 경로 탐색기)
전자공학과 2학년 응용 프로그래밍 과제입니다.

MFC를 이용하여 구현한 지도 기반 최단 경로 탐색 프로그램입니다.
사용자가 직접 지도 위에 점(Node)과 선(Edge)를 그리고, 다익스트라(Dijkstra) 알고리즘을 수행하여 두 지점 사이의 최단 거리와 경로를 시각적으로 확인할 수 있습니다.


## 주요 기능 (Features)

* **지도 표시:** 학교 비트맵 지도를 배경으로 로드하여 표시
* **노드(Node) 생성:** 마우스 입력을 통해 자유롭게 지점 추가
* **엣지(Edge) 연결:** 점과 점 사이를 연결하여 이동 가능한 경로 생성
* **거리 계산:**
	* 생성된 경로(PolyLine)의 구간별 거리를 피타고라스 정리를 이용해 자동 계산 및 표시
	* 경로 위에 텍스트로 거리 수치 시각화 (가독성을 위한 테두리 처리 적용)
* **최단 경로 탐색:** 다익스트라 알고리즘을 적용하여 시작점과 도착점 사이의 가장 짧은 경로를 녹색 선으로 표시
* **편집 기능:** 잘못 생성된 점이나 선을 삭제하는 기능 포함


## 조작 방법 (Controls)

이 프로그램은 기보드와 마우스 조합으로 동작합니다.

| 동작 | 단축키 / 마우스 | 설명 |
| :--- | :--- | :--- |
| **점 추가** | `Ctrl` + `L-Click` | 지도 위에 새로운 노드를 생성합니다. |
| **경로 그리기** | `Alt` + `L-Click` | 기존에 생성된 점들을 연결하여 선을 만듭니다. |
| **경로 초기화** | `L-DblClick` | 경로 그리기를 멈추고 새로운 경로를 그릴 준비를 합니다. |
| **최단 경로 탐색** | `Alt` + `Shift` + `L-Click` | 시작점과 도착점을 순서대로 클릭하면 최단 경로가 녹색으로 표시됩니다. |
| **경로 삭제** | `Delete` | 마지막에 추가한 경로를 삭제합니다. |
| **이전 경로 삭제** | `Ctrl` + `Delete` | 더블 클릭으로 경로를 초기화해서 선이 삭제가 안될 때 사용하여 이전 경로를 삭제할 수 있도록 설정합니다. |

## 개발 환경 (Environment)

* **Language:** C++
* **Framework:** MFC (Microsoft Foundation Class)
* **IDE:** Visual Studio 2022
* **Algorithm:** Dijkstra Algorithm (Graph Theory)


## 설치 및 실행 (How to Run)

1. 이 저장소를 클론(Clone)합니다.
   ```bash
   git clone [https://github.com/Doyun2003/FindingShortestPath.git](https://github.com/Doyun2003/FindingShortestPath.git)
2. Map.sln 파일을 Visual Studio로 엽니다.
3. 솔루션 빌드(Build Solution)를 수행합니다. (Ctrl + Shift + B)
4. 프로그램을 실행합니다. (Ctrl + F5)
