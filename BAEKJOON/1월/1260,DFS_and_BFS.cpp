// 1260

/// �׷����� DFS�� Ž���� ����� BFS�� Ž���� ����� ����ϴ� ���α׷��� �ۼ��Ͻÿ�. 
/// ��, �湮�� �� �ִ� ������ ���� ���� ��쿡�� ���� ��ȣ�� ���� ���� ���� �湮�ϰ�, �� �̻� �湮�� �� �ִ� ���� ���� ��� �����Ѵ�. 
/// ���� ��ȣ�� 1������ N�������̴�.

/// pre  : ù° �ٿ� ������ ���� N(1 �� N �� 1,000), ������ ���� M(1 �� M �� 10,000), 
/// Ž���� ������ ������ ��ȣ V�� �־�����. ���� M���� �ٿ��� ������ �����ϴ� �� ������ ��ȣ�� �־�����. 
/// � �� ���� ���̿� ���� ���� ������ ���� �� �ִ�. �Է����� �־����� ������ ������̴�.

/// post : ù° �ٿ� DFS�� ������ �����, �� ���� �ٿ��� BFS�� ������ ����� ����Ѵ�. V���� �湮�� ���� ������� ����ϸ� �ȴ�.


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


#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����

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
	arr[V-1] = 0;					// 0 : �湮
	vec.emplace(V-1);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_mat"> �׷���		</param>
	/// <param name="N">	����		</param>
	/// <param name="V">	���� ���	</param>
	while (!vec.empty()) {
		int i = vec.front(); vec.pop();
		cout << i + 1 << ' ';
		for (int j = 0; j < N; j++) {
			if (mat[i][j] && arr[j]) {
				arr[j] = 0;			// 0 : �湮
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
	arr[V - 1] = 0;						// 0 : �湮
	vec.emplace(make_pair(V - 1,0));	// ��ġ V�� ����, 0�� �˻��� ����
	cout << V << ' ';
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_mat"> �׷���		</param>
	/// <param name="N">	����		</param>
	/// <param name="V">	���� ���	</param>
	while (!vec.empty()) {
		ij = vec.top();
		int i = ij.first;
		int j = ij.second;
		vec.pop();
		// cout << " pop " << i+1 << " " << j+1 << endl;
		for (; j < N; j++) {
			// cout << " search " << i + 1 << " " << j + 1 << endl;
			if (mat[i][j] && arr[j]) {
				arr[j] = 0;							// 0 : �湮
				vec.emplace(make_pair(i,j+1));		// ��ġ i�� ����, ��� i�� ��� j+1���� ������ �˻��� ����
				cout << j + 1 << ' ';
				i = j;
				j = -1;								// for�� ������
			}
		}
	}
	delete[] arr;
}


int main() {
	cin.tie(NULL); cout.tie(NULL);
	ios::sync_with_stdio(false);
	
	int N, M, V; // ������ ����, ������ ����, ���� ����
	cin >> N >> M >> V;

	int** mat = new int* [N];
	for (int i = 0; i < N; i++) {
		mat[i] = new int[N];
		memset(mat[i], 0, sizeof(int) * N);		// int�� ������ ũ�� ����
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

