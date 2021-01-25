// 11399

/// ���� [2, 5, 1, 4, 3] ������ ���� ����, 2�� ����� 1�и���, 5�� ����� 1+2 = 3��, 
/// 1�� ����� 1+2+3 = 6��, 4�� ����� 1+2+3+3 = 9��, 3�� ����� 1+2+3+3+4 = 13���� �ɸ��� �ȴ�. 
/// �� ����� ���� �����ϴµ� �ʿ��� �ð��� ���� 1+3+6+9+13 = 32���̴�. 
/// �� ������� �� �ʿ��� �ð��� ���� �ּҷ� ���� ���� ����.

/// pre  : ù° �ٿ� ����� �� N(1 �� N �� 1,000)�� �־�����. ��° �ٿ��� �� ����� ���� �����ϴµ� �ɸ��� �ð� Pi�� �־�����. (1 �� Pi �� 1,000)

/// post : ù° �ٿ� �� ����� ���� �����ϴµ� �ʿ��� �ð��� ���� �ּڰ��� ����Ѵ�.



/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;


int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);

	int N;			cin >> N;
	int time = 0, tem, sum = 0;;
	multiset<int> set;

	for (int i = 0; i < N; i++) {
		cin >> tem;
		set.emplace(tem);
	}
	for (auto& item : set) {
		sum += item;
		time += sum;
	}
	cout << time;
}

