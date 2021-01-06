

/// �������� ����: 2���� ū ¦���� �� �Ҽ��� ������ ��Ÿ�� �� �ִ�.
/// ¦�� N�� �� �Ҽ��� ������ ��Ÿ���� ǥ���� ������ ��Ƽ���̶�� �Ѵ�. ¦�� N�� �־����� ��, ������ ��Ƽ���� ������ ���غ���. �� �Ҽ��� ������ �ٸ� ���� ���� ��Ƽ���̴�.
/// ���� n�� �־����� ��, n�� 1, 2, 3�� ������ ��Ÿ���� ����� ���� ���ϴ� ���α׷��� �ۼ��Ͻÿ�.

/// pre  : ù° �ٿ� �׽�Ʈ ���̽��� ���� T (1 �� T �� 100)�� �־�����. �� �׽�Ʈ ���̽��� �� �ٷ� �̷���� �ְ�, ���� N�� ¦���̰�, 2 < N �� 1,000,000�� �����Ѵ�.
///		1 000 000 : �鸸 < int32

/// post : ������ �׽�Ʈ ���̽����� ������ ��Ƽ���� ���� ����Ѵ�.

/// ���ѽð�     500ms
/// �޸� ����  512 MB


/// <summary>
///  1�� �Ҽ��� �ƴϴ�
/// </summary>
/// <returns></returns>

#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����

#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;

static int index[1000001] = { 1, 1 };		// n�� ����� �ƴϸ�(�Ҽ�) 0
											// 0, 1�� �Ҽ��� �ƴ�
int decimal( vector<int>& pvec) {

	static int check = 2;					// check�� ���ڸ� �˻��� ����
	int n = 1000000;
	// n���� �Ҽ��� �˻����� ���� ��� ���
	for (; check <= n; check ++) {
		if (index[check] == 0) {
			pvec.push_back(check);
			for (int j = check*2; j < 1000001; j += check) {
				index[j] = 1;
			}
		}
	}

	pvec.push_back(1000003);// 1000003�� �Ҽ�
	return pvec.size();
}


// ���ϻ���̽Ű���
int goldbach(const int n, const vector<int>& vec) {
	int gold = 0;
	vector<int>::const_iterator iter;
	auto ptr = vec.end();
	int half_n = n / 2 + 1;

	// �� �Ҽ��� ������ ǥ�� �Ǵ��� Ȯ��
	// �Ҽ��� n ���� Ŭ �� ����
	for (iter = vec.begin(); *iter < half_n; iter++) {

		if (! index[n - *iter]) {
			//cout << n << " - " << *iter << " : " << n - *iter << "|" << index[n - *iter] << endl;
			gold++;
		}
	}
	return gold;
}


int main() {

	cin.tie(NULL);
	ios::sync_with_stdio(false);

	vector<int> dec;
	int gold, n, T; 
	
	cin >> T;
	decimal(dec);
	//std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	for (int i = 0; i < T; i++) {
		cin >> n;
		cout << goldbach(n, dec) << endl;
	}

	//std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	//std::cout << "Test() �Լ��� �����ϴ� �ɸ� �ð�(��) : " << sec.count() << " seconds" << std::endl;
}

