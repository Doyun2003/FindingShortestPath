#include "pch.h"
#include "CMyLine.h"
#include <cmath>
#include <cfloat>

int GetIndexByPoint(const CArray<CPoint, CPoint>& points, CPoint pt) {
	int count = points.GetCount();
	for (int i = 0; i < count; i++) {
		if (points[i] == pt) return i;
	}
	return -1;
}

std::vector<int> CMyLine::FindShortestPath(
	const CArray<CPoint, CPoint>& allPoints,
	const CArray<CMyLine, CMyLine&>& allLines,
	int startNode,
	int endNode)
{
	int n = allPoints.GetCount();
	if (startNode < 0 || endNode < 0 || startNode >= n || endNode >= n)
		return {};

	std::vector<std::vector<double>> adj(n, std::vector<double>(n, DBL_MAX));

	int lineCount = allLines.GetCount();
	for (int i = 0; i < lineCount; i++) {
		const CMyLine& line = allLines[i];
		int ptCount = line.m_points.GetCount();

		for (int j = 0; j < ptCount - 1; j++) {
			CPoint p1 = line.m_points[j];
			CPoint p2 = line.m_points[j + 1];

			int u = GetIndexByPoint(allPoints, p1);
			int v = GetIndexByPoint(allPoints, p2);

			if (u != -1 && v != -1) {
				double dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
				if (dist < adj[u][v]) {
					adj[u][v] = dist;
					adj[v][u] = dist;
				}
			}
		}
	}


	std::vector<double> dist(n, DBL_MAX);
	std::vector<int> parent(n, -1);
	std::vector<bool> visited(n, false);

	dist[startNode] = 0;

	for (int i = 0; i < n; i++) {
		int u = -1;
		double minDist = DBL_MAX;

		for (int j = 0; j < n; j++) {
			if (!visited[j] && dist[j] < minDist) {
				minDist = dist[j];
				u = j;
			}
		}

		if (u == -1 || u == endNode) break;
		visited[u] = true;

		for (int v = 0; v < n; v++) {
			if (!visited[v] && adj[u][v] != DBL_MAX) {
				if (dist[u] + adj[u][v] < dist[v]) {
					dist[v] = dist[u] + adj[u][v];
					parent[v] = u;
				}
			}
		}
	}

	std::vector<int> path;
	if (dist[endNode] == DBL_MAX) return path;

	int curr = endNode;
	while (curr != -1) {
		path.insert(path.begin(), curr);
		curr = parent[curr];
	}

	return path;
}
