// 15649
// 2020KB 316ms

/// 자연수 N과 M이 주어졌을 때, 아래 조건을 만족하는 길이가 M인 수열을 모두 구하는 프로그램을 작성하시오.
/// 1부터 N까지 자연수 중에서 중복 없이 M개를 고른 수열

/// pre  : 첫째 줄에 자연수 N과 M이 주어진다. (1 ≤ M ≤ N ≤ 8)

/// post : 한 줄에 하나씩 문제의 조건을 만족하는 수열을 출력한다. 중복되는 수열을 여러 번 출력하면 안되며, 각 수열은 공백으로 구분해서 출력해야 한다.
///			수열은 사전 순으로 증가하는 순서로 출력해야 한다.


/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

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

// 각 조합(combination)인지 중복 검사( n 자릿수 )
// 중복이 없으면 1
// 수열을 정수로 참조반환
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


// endl 사용시 시간초과
// endl 호출 시 출력버퍼를 비워주는 과정(flush)가 생겨 시간초과가 발생
void printArr(int* arr, int len) {
	for (int i = 0; i < len; i++) {
		cout << arr[i] << " ";
	}cout << '\n'; 
}


int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	
	int n, m; cin >> n >> m;
	int inum = 0;					// 수열 출력용
	set<int> sNum;					// 선택지
	set<int>::iterator iter;
	int* num = new int[m];			// 현 숫자


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
	if (getCombination(num, m, inum))				// 조합(Combination)검사
		cout << inum << '\n';
	


	for (int j = m-1; j >= 0;) {					// 마지막 자리수 부터
		// printArr(num, m);      
		iter = sNum.find(num[j]);	++iter;			// 현재 숫자의 다음 수
		
		if (iter == sNum.end()) {					// 더 숫자가 없으면, 다음 자리
			num[j] = *sNum.begin();
			j--;
		}
		else {
			num[j] = *iter;							// 다음 수열
			if (getCombination(num, m, inum))		// 조합(Combination)검사
				//cout << inum << endl;
				printArr(num, m);
			j = m - 1;								// 자리 초기화
		}				
	}
}

