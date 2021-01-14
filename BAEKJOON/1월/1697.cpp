//1697
//
// 수빈이는 동생과 숨바꼭질을 하고 있다. 
// 수빈이는 현재 점 N(0 ≤ N ≤ 100,000)에 있고, 동생은 점 K(0 ≤ K ≤ 100,000)에 있다. 
// 수빈이는 걷거나 순간이동을 할 수 있다. 만약, 수빈이의 위치가 X일 때 걷는다면 1초 후에 X-1 또는 X+1로 이동하게 된다. 
// 순간이동을 하는 경우에는 1초 후에 2*X의 위치로 이동하게 된다.
// 수빈이와 동생의 위치가 주어졌을 때, 수빈이가 동생을 찾을 수 있는 가장 빠른 시간이 몇 초 후인지 구하는 프로그램을 작성하시오.
//
// pre  : 첫 번째 줄에 수빈이가 있는 위치 N과 동생이 있는 위치 K가 주어진다. N과 K는 정수이다.
// 문제로부터 (0 ≤ N ≤ 100,000), (0 ≤ K ≤ 100,000)
//
// post : 수빈이가 동생을 찾는 가장 빠른 시간을 출력한다
//
//
// <summary>
//  수빈이가 5-10-9-18-17 순으로 가면 4초만에 동생을 찾을 수 있다.
//  5 17 >> 4
// 
//  K < N인 경우 주의
//  1000 1 > 999
// </summary>
// <returns></returns>
//
//
//#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시
//
//#include <stdio.h>
//#include <iostream>
//#include <queue>
//#include <algorithm>
//
//using namespace std;
//
//int main() {
//	cin.tie(NULL);
//	ios::sync_with_stdio(false);
//	srand(time(NULL));
//
//	for (int nn = 0; nn < 100001; nn++) {
//		int k = rand() % 100001, n = rand() % 100001;
//
//		queue<int> que;
//		int i = 0;
//		// cin >> n >> k;
//		const int limit = 100000;
//		bool index[100001] = { 0 }; // 중복 검사
//		cout << n << " " << k << " : ";
//
//		if (n != k) {
//			i -= 1;
//		}
//		que.push(n); /// 큐에 넣음
//
//		while (n != k) {
//			i++;
//			int end = que.size();
//
//			 cout << "i" << i << " end" << end << endl;// << " 번 연산이 필요합니다.\n";
//			for (int j = 0; j < end; j++) {
//				if (que.empty()) { cout << "empty queue !!"; }
//				n = que.front(); que.pop(); /// 큐에서 뻄
//				 cout << n << endl;// << " 번 연산이 필요합니다.\n";
//				if (n == k) {
//					break;
//					cout << "같음";
//				}						
//				
//				 // index[n] = true;
//				 // if (2 * n <= limit && index[n*2] == false)	que.push(n * 2);
//				 // if (n > 0 && index[n] == true)				que.push(n - 1);
//				 // if (n +1 <= limit && index[n+1] == false)		que.push(n + 1);
//
//				else if (n > limit || n < 0 || index[n]) { // n < 0 
//					continue;
//				}
//				else {
//					index[n] = true;
//					que.push(n * 2);
//					que.push(n - 1);
//					que.push(n + 1);
//				}
//			}
//		}
//		cout << i << endl;// << " 번 연산이 필요합니다.\n";
//		queue<int>().swap(que);
//	}
//	cout << "End";
//}




#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);

	queue<int> que;
	int n, k, i = 0;
	cin >> n >> k;
	const int limit = 100000;
	bool index[100001] = { 0 }; // 중복 검사

	if (n != k) {
		i -= 1;
	}
	que.push(n); /// 큐에 넣음

	while (n != k) {
		i++;
		int end = que.size();

		// cout << "i" << i << " end" << end << endl;  // << " 번 연산이 필요합니다.\n";
		for (int j = 0; j < end; j++) {

			n = que.front(); que.pop(); /// 큐에서 뻄
			 cout << n << endl;// << " 번 연산이 필요합니다.\n";
			if (n == k) {
				break;
			}

			/* 시간 초과
			 index[n] = true;
			 if (2 * n <= limit && index[n*2] == false)		que.push(n * 2);
			 if (n > 0 && index[n] == true)					que.push(n - 1);
			 if (n +1 <= limit && index[n+1] == false)		que.push(n + 1);*/

			else if (n > limit || n < 0 || index[n]) { // n < 0 
				continue;
			}
			else {
				index[n] = true;
				que.push(n * 2);
				que.push(n - 1);
				que.push(n + 1);
			}
		}
	}
	cout << i << endl;// << " 번 연산이 필요합니다.\n";
	queue<int>().swap(que);
}
