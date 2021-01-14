//1697
//
// �����̴� ������ ���ٲ����� �ϰ� �ִ�. 
// �����̴� ���� �� N(0 �� N �� 100,000)�� �ְ�, ������ �� K(0 �� K �� 100,000)�� �ִ�. 
// �����̴� �Ȱų� �����̵��� �� �� �ִ�. ����, �������� ��ġ�� X�� �� �ȴ´ٸ� 1�� �Ŀ� X-1 �Ǵ� X+1�� �̵��ϰ� �ȴ�. 
// �����̵��� �ϴ� ��쿡�� 1�� �Ŀ� 2*X�� ��ġ�� �̵��ϰ� �ȴ�.
// �����̿� ������ ��ġ�� �־����� ��, �����̰� ������ ã�� �� �ִ� ���� ���� �ð��� �� �� ������ ���ϴ� ���α׷��� �ۼ��Ͻÿ�.
//
// pre  : ù ��° �ٿ� �����̰� �ִ� ��ġ N�� ������ �ִ� ��ġ K�� �־�����. N�� K�� �����̴�.
// �����κ��� (0 �� N �� 100,000), (0 �� K �� 100,000)
//
// post : �����̰� ������ ã�� ���� ���� �ð��� ����Ѵ�
//
//
// <summary>
//  �����̰� 5-10-9-18-17 ������ ���� 4�ʸ��� ������ ã�� �� �ִ�.
//  5 17 >> 4
// 
//  K < N�� ��� ����
//  1000 1 > 999
// </summary>
// <returns></returns>
//
//
//#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����
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
//		bool index[100001] = { 0 }; // �ߺ� �˻�
//		cout << n << " " << k << " : ";
//
//		if (n != k) {
//			i -= 1;
//		}
//		que.push(n); /// ť�� ����
//
//		while (n != k) {
//			i++;
//			int end = que.size();
//
//			 cout << "i" << i << " end" << end << endl;// << " �� ������ �ʿ��մϴ�.\n";
//			for (int j = 0; j < end; j++) {
//				if (que.empty()) { cout << "empty queue !!"; }
//				n = que.front(); que.pop(); /// ť���� �M
//				 cout << n << endl;// << " �� ������ �ʿ��մϴ�.\n";
//				if (n == k) {
//					break;
//					cout << "����";
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
//		cout << i << endl;// << " �� ������ �ʿ��մϴ�.\n";
//		queue<int>().swap(que);
//	}
//	cout << "End";
//}




#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����

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
	bool index[100001] = { 0 }; // �ߺ� �˻�

	if (n != k) {
		i -= 1;
	}
	que.push(n); /// ť�� ����

	while (n != k) {
		i++;
		int end = que.size();

		// cout << "i" << i << " end" << end << endl;  // << " �� ������ �ʿ��մϴ�.\n";
		for (int j = 0; j < end; j++) {

			n = que.front(); que.pop(); /// ť���� �M
			 cout << n << endl;// << " �� ������ �ʿ��մϴ�.\n";
			if (n == k) {
				break;
			}

			/* �ð� �ʰ�
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
	cout << i << endl;// << " �� ������ �ʿ��մϴ�.\n";
	queue<int>().swap(que);
}
