// 11399

/// 줄을 [2, 5, 1, 4, 3] 순서로 줄을 서면, 2번 사람은 1분만에, 5번 사람은 1+2 = 3분, 
/// 1번 사람은 1+2+3 = 6분, 4번 사람은 1+2+3+3 = 9분, 3번 사람은 1+2+3+3+4 = 13분이 걸리게 된다. 
/// 각 사람이 돈을 인출하는데 필요한 시간의 합은 1+3+6+9+13 = 32분이다. 
/// 이 방법보다 더 필요한 시간의 합을 최소로 만들 수는 없다.

/// pre  : 첫째 줄에 사람의 수 N(1 ≤ N ≤ 1,000)이 주어진다. 둘째 줄에는 각 사람이 돈을 인출하는데 걸리는 시간 Pi가 주어진다. (1 ≤ Pi ≤ 1,000)

/// post : 첫째 줄에 각 사람이 돈을 인출하는데 필요한 시간의 합의 최솟값을 출력한다.



/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

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

