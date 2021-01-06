

/// 골드바흐의 추측: 2보다 큰 짝수는 두 소수의 합으로 나타낼 수 있다.
/// 짝수 N을 두 소수의 합으로 나타내는 표현을 골드바흐 파티션이라고 한다. 짝수 N이 주어졌을 때, 골드바흐 파티션의 개수를 구해보자. 두 소수의 순서만 다른 것은 같은 파티션이다.
/// 정수 n이 주어졌을 때, n을 1, 2, 3의 합으로 나타내는 방법의 수를 구하는 프로그램을 작성하시오.

/// pre  : 첫째 줄에 테스트 케이스의 개수 T (1 ≤ T ≤ 100)가 주어진다. 각 테스트 케이스는 한 줄로 이루어져 있고, 정수 N은 짝수이고, 2 < N ≤ 1,000,000을 만족한다.
///		1 000 000 : 백만 < int32

/// post : 각각의 테스트 케이스마다 골드바흐 파티션의 수를 출력한다.

/// 제한시간     500ms
/// 메모리 제한  512 MB


/// <summary>
///  1은 소수가 아니다
/// </summary>
/// <returns></returns>

#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;

static int index[1000001] = { 1, 1 };		// n의 배수가 아니면(소수) 0
											// 0, 1은 소수가 아님
int decimal( vector<int>& pvec) {

	static int check = 2;					// check의 숫자를 검사할 차례
	int n = 1000000;
	// n까지 소수를 검사하지 않은 경우 계산
	for (; check <= n; check ++) {
		if (index[check] == 0) {
			pvec.push_back(check);
			for (int j = check*2; j < 1000001; j += check) {
				index[j] = 1;
			}
		}
	}

	pvec.push_back(1000003);// 1000003은 소수
	return pvec.size();
}


// 독일사람이신가요
int goldbach(const int n, const vector<int>& vec) {
	int gold = 0;
	vector<int>::const_iterator iter;
	auto ptr = vec.end();
	int half_n = n / 2 + 1;

	// 두 소수의 합으로 표현 되는지 확인
	// 소수는 n 보다 클 수 없음
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
	//std::cout << "Test() 함수를 수행하는 걸린 시간(초) : " << sec.count() << " seconds" << std::endl;
}

