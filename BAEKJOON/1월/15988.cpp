
/// 정수 4를 1, 2, 3의 합으로 나타내는 방법은 총 7가지가 있다. 합을 나타낼 때는 수를 1개 이상 사용해야 한다.
/// 1 + 1 + 1 + 1
/// 1 + 1 + 2
/// 1 + 2 + 1
/// 2 + 1 + 1
/// 2 + 2
/// 1 + 3
/// 3 + 1
/// 정수 n이 주어졌을 때, n을 1, 2, 3의 합으로 나타내는 방법의 수를 구하는 프로그램을 작성하시오.

/// pre  : 첫째 줄에 테스트 케이스의 개수 T가 주어진다. 각 테스트 케이스는 한 줄로 이루어져 있고, 정수 n이 주어진다. n은 양수이며 1,000,000보다 작거나 같다.
///		1 000 000 : 백만 < int32

/// post : 각 테스트 케이스마다, n을 1, 2, 3의 합으로 나타내는 방법의 수를 1,000,000,009 (10억 9)로 나눈 나머지를 출력한다.
///		77 부터 2,147,483,648 을 초과


/// <summary>
///  그냥 깡으로 계산 하니 메모리 초과
/// </summary>
/// <returns></returns>

#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

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

