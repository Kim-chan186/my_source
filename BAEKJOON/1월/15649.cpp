// 15649
// 2020KB 316ms

/// �ڿ��� N�� M�� �־����� ��, �Ʒ� ������ �����ϴ� ���̰� M�� ������ ��� ���ϴ� ���α׷��� �ۼ��Ͻÿ�.
/// 1���� N���� �ڿ��� �߿��� �ߺ� ���� M���� �� ����

/// pre  : ù° �ٿ� �ڿ��� N�� M�� �־�����. (1 �� M �� N �� 8)

/// post : �� �ٿ� �ϳ��� ������ ������ �����ϴ� ������ ����Ѵ�. �ߺ��Ǵ� ������ ���� �� ����ϸ� �ȵǸ�, �� ������ �������� �����ؼ� ����ؾ� �Ѵ�.
///			������ ���� ������ �����ϴ� ������ ����ؾ� �Ѵ�.


/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	// scanf ���� ����

// #include <stdio.h>
#include <iostream>
// #include <queue>
#include <set>
#include <algorithm>

using namespace std;

// 0 < n < 8
int factorial(int n) {
	int fac = 1;
	for (; n; n--) {
		fac *= n;
	}
	return fac;
}

// �� ����(combination)���� �ߺ� �˻�( n �ڸ��� )
// �ߺ��� ������ 1
// ������ ������ ������ȯ
bool getCombination(int* arr, int n, int& num) {
	num = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (arr[i] == arr[j]) return 0;
		}
		num *= 10;
		num += arr[i];
	}
	return 1;
}


// endl ���� �ð��ʰ�
// endl ȣ�� �� ��¹��۸� ����ִ� ����(flush)�� ���� �ð��ʰ��� �߻�
void printArr(int* arr, int len) {
	for (int i = 0; i < len; i++) {
		cout << arr[i] << " ";
	}cout << '\n'; 
}


int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	
	int n, m; cin >> n >> m;
	int inum = 0;					// ���� ��¿�
	set<int> sNum;					// ������
	set<int>::iterator iter;
	int* num = new int[m];			// �� ����


	for (int i = 0; i < n; i++) {
		sNum.emplace( i + 1 );
	}
	/*for (auto& item : sNum) {
		cout << item << " ";
	}cout << endl;*/

	for (int i = 0; i < m; i++) {
		num[i] = 1;
	}
	// printArr(num, m);
	// int end = factorial(m) / (factorial(n) * factorial(m - n));

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int end = m * n - 1 ;
	if (getCombination(num, m, inum))				// ����(Combination)�˻�
		cout << inum << '\n';
	


	for (int j = m-1; j >= 0;) {					// ������ �ڸ��� ����
		// printArr(num, m);      
		iter = sNum.find(num[j]);	++iter;			// ���� ������ ���� ��
		
		if (iter == sNum.end()) {					// �� ���ڰ� ������, ���� �ڸ�
			num[j] = *sNum.begin();
			j--;
		}
		else {
			num[j] = *iter;							// ���� ����
			if (getCombination(num, m, inum))		// ����(Combination)�˻�
				//cout << inum << endl;
				printArr(num, m);
			j = m - 1;								// �ڸ� �ʱ�ȭ
		}				
	}
}

