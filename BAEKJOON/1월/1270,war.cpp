// 1270

/// 하지만 당신은 군대를 보낼 때 적군을 혼란시키기 위해서 우리 나라의 군대라는걸 표시하지 않고, 군대의 번호로 표시했다.
/// 어느 땅에서 한 번호의 군대의 병사가 절반을 초과한다면 그 땅은 그 번호의 군대의 지배하에 놓이게 된다.
/// 이때, 각 땅들을 지배한 군대의 번호를 출력하여라.만약, 아직 전쟁이 한창중인 땅이라면 “SYJKGW”을 쌍 따옴표 없이 출력한다.

/// pre  : 첫째 줄에는 땅의 개수 n(n<=200)이 주어진다. 그리고 두 번째 줄에서 n+1번째 줄에는 제일 처음에 숫자 Ti(i번째 땅의 병사수, Ti<=100,000)와, 
///			Ti개의 숫자 (각각 병사의 군대 번호)가 주어진다. i번째 땅의 j번째 병사 번호 Nij가 주어진다. ( | Nij | <= 2^31 )

/// post : 첫째 줄에는 각각의 땅의 상태를 순서대로 출력한다. 만약 땅이 지배가 되어있다면 그 지배한 병사의 번호를 출력하고, 아니라면 “SYJKGW”을 쌍 따옴표 없이 출력한다.



/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	 // scanf 에러 무시

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>
#include <utility>
#include <limits.h>	// INT_MAX 정의

using namespace std;


int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	int N, cnt = 0, max = -1, size;
	long long num, max_num;
	const string str = "SYJKGW\n";
	multiset<long long> vec;				// key, val
	//vector<string> vec2;
	string line;
	stringstream sin;

	cin >> N;
	cin.ignore(INT_MAX, '\n');				// buf clear

	for (int i = 0; i < N; i++, cnt = 0, max = 0) {
		// num : 입력 값
		cin >> cnt;
		for (; cnt; cnt--) {
			cin >> num;
			vec.insert(num);
			//cout << num << " ";
		}

		// num : 이전 값
		num = -1, cnt = 0;
		for (const auto& item : vec) {
			// cout << item << " ";
			if (item == num) { cnt++; }
			else if (cnt > max)
			{
				max_num = num;
				max = cnt;
				num = item;
				cnt = 1;
			}
			else {
				num = item;
				cnt = 1;
			}
		}
		if (cnt > max)	// 마지막 숫자에 대해
		{
			max_num = num;
			max = cnt;
		}

		
		if (vec.size() < max * 2) { // vec.size()
			cout << max_num << '\n';
			//vec2.push_back(to_string(max_num));
		}
		else {
			cout << str;
			//vec2.push_back(str);
		}

		sin.clear();						// 재사용 가능
		vec.clear();						// vector 비우기
	}
}
