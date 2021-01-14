
/// ���� 4�� 1, 2, 3�� ������ ��Ÿ���� ����� �� 7������ �ִ�. ���� ��Ÿ�� ���� ���� 1�� �̻� ����ؾ� �Ѵ�.
/// 1 + 1 + 1 + 1
/// 1 + 1 + 2
/// 1 + 2 + 1
/// 2 + 1 + 1
/// 2 + 2
/// 1 + 3
/// 3 + 1
/// ���� n�� �־����� ��, n�� 1, 2, 3�� ������ ��Ÿ���� ����� ���� ���ϴ� ���α׷��� �ۼ��Ͻÿ�.

/// pre  : ù° �ٿ� �׽�Ʈ ���̽��� ���� T�� �־�����. �� �׽�Ʈ ���̽��� �� �ٷ� �̷���� �ְ�, ���� n�� �־�����. n�� ����̸� 1,000,000���� �۰ų� ����.
///		1 000 000 : �鸸 < int32

/// post : �� �׽�Ʈ ���̽�����, n�� 1, 2, 3�� ������ ��Ÿ���� ����� ���� 1,000,000,009 (10�� 9)�� ���� �������� ����Ѵ�.
///		77 ���� 2,147,483,648 �� �ʰ�


/// <summary>
///  �׳� ������ ��� �ϴ� �޸� �ʰ�
/// </summary>
/// <returns></returns>

#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����

#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

//   32 MB  < 256 MB
// 10^9 * 3 < uint32
int vec[1000001] = { 0 };	
int recursion(int n) {
	if (vec[n] == 0)
		vec[n] = (recursion(n - 3) + recursion(n - 2) + recursion(n - 1));
	return vec[n];
}

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	vec[0] = vec[1] = 1; vec[2] = 2;
	int ii = 0, n, end; cin >> end;
	for (int i = 0; i < end; i++) {
		ii = 0;
		cin >> n;
		cout << recursion(n) << endl;
	}
}

