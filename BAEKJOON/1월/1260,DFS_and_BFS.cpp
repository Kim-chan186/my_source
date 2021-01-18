// 1260

/// 그래프를 DFS로 탐색한 결과와 BFS로 탐색한 결과를 출력하는 프로그램을 작성하시오. 
/// 단, 방문할 수 있는 정점이 여러 개인 경우에는 정점 번호가 작은 것을 먼저 방문하고, 더 이상 방문할 수 있는 점이 없는 경우 종료한다. 
/// 정점 번호는 1번부터 N번까지이다.

/// pre  : 첫째 줄에 정점의 개수 N(1 ≤ N ≤ 1,000), 간선의 개수 M(1 ≤ M ≤ 10,000), 
/// 탐색을 시작할 정점의 번호 V가 주어진다. 다음 M개의 줄에는 간선이 연결하는 두 정점의 번호가 주어진다. 
/// 어떤 두 정점 사이에 여러 개의 간선이 있을 수 있다. 입력으로 주어지는 간선은 양방향이다.

/// post : 첫째 줄에 DFS를 수행한 결과를, 그 다음 줄에는 BFS를 수행한 결과를 출력한다. V부터 방문된 점을 순서대로 출력하면 된다.


/// <summary>
/// input
/// 10 10 5
/// 1 10
/// 1 9
/// 1 8
/// 1 7
/// 1 2
/// 2 3
/// 2 5
/// 3 6
/// 3 4
/// 4 8
/// 
/// output
/// 5 2 1 7 8 4 3 6 9 10
/// 5 2 1 3 7 8 9 10 4 6
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>

using namespace std;

void printMat(int** mat, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << mat[i][j] << " ";
		}cout << "\n";
	}
	
}

void BFS(int** mat, int N, int V) {

	int* arr = new int[N];			// visited
	memset(arr, 1, sizeof(int) * N);

	queue<int> vec;
	arr[V-1] = 0;					// 0 : 방문
	vec.emplace(V-1);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_mat"> 그래프		</param>
	/// <param name="N">	노드수		</param>
	/// <param name="V">	시작 노드	</param>
	while (!vec.empty()) {
		int i = vec.front(); vec.pop();
		cout << i + 1 << ' ';
		for (int j = 0; j < N; j++) {
			if (mat[i][j] && arr[j]) {
				arr[j] = 0;			// 0 : 방문
				vec.emplace(j);
			}
		}
	}
	delete[] arr;
}


void DFS(int** mat, int N, int V) {

	int* arr = new int[N];				// visited
	memset(arr, 1, sizeof(int) * N);

	stack<pair<int, int>> vec;
	pair<int, int> ij;
	arr[V - 1] = 0;						// 0 : 방문
	vec.emplace(make_pair(V - 1,0));	// 위치 V를 저장, 0을 검사할 차례
	cout << V << ' ';
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_mat"> 그래프		</param>
	/// <param name="N">	노드수		</param>
	/// <param name="V">	시작 노드	</param>
	while (!vec.empty()) {
		ij = vec.top();
		int i = ij.first;
		int j = ij.second;
		vec.pop();
		// cout << " pop " << i+1 << " " << j+1 << endl;
		for (; j < N; j++) {
			// cout << " search " << i + 1 << " " << j + 1 << endl;
			if (mat[i][j] && arr[j]) {
				arr[j] = 0;							// 0 : 방문
				vec.emplace(make_pair(i,j+1));		// 위치 i를 저장, 노드 i는 노드 j+1과의 연결을 검사할 차례
				cout << j + 1 << ' ';
				i = j;
				j = -1;								// for의 증감식
			}
		}
	}
	delete[] arr;
}


int main() {
	cin.tie(NULL); cout.tie(NULL);
	ios::sync_with_stdio(false);
	
	int N, M, V; // 정점의 개수, 간선의 개수, 시작 정점
	cin >> N >> M >> V;

	int** mat = new int* [N];
	for (int i = 0; i < N; i++) {
		mat[i] = new int[N];
		memset(mat[i], 0, sizeof(int) * N);		// int형 데이터 크기 주의
	}

	for (int i = 0; i < M; i++) {
		int n1, n2;
		cin >> n1 >> n2;
		mat[n1 - 1][n2 - 1] = true;
		mat[n2 - 1][n1 - 1] = true;
	}

	//printMat(mat, N);
	DFS(mat, N, V); cout << "\n";
	BFS(mat, N, V);

	for (int i = 0; i < N; i++) {
		delete[] mat[i];
	}
	delete[] mat;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	
}

