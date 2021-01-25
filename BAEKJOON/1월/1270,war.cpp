// 1270

/// ������ ����� ���븦 ���� �� ������ ȥ����Ű�� ���ؼ� �츮 ������ �����°� ǥ������ �ʰ�, ������ ��ȣ�� ǥ���ߴ�.
/// ��� ������ �� ��ȣ�� ������ ���簡 ������ �ʰ��Ѵٸ� �� ���� �� ��ȣ�� ������ �����Ͽ� ���̰� �ȴ�.
/// �̶�, �� ������ ������ ������ ��ȣ�� ����Ͽ���.����, ���� ������ ��â���� ���̶�� ��SYJKGW���� �� ����ǥ ���� ����Ѵ�.

/// pre  : ù° �ٿ��� ���� ���� n(n<=200)�� �־�����. �׸��� �� ��° �ٿ��� n+1��° �ٿ��� ���� ó���� ���� Ti(i��° ���� �����, Ti<=100,000)��, 
///			Ti���� ���� (���� ������ ���� ��ȣ)�� �־�����. i��° ���� j��° ���� ��ȣ Nij�� �־�����. ( | Nij | <= 2^31 )

/// post : ù° �ٿ��� ������ ���� ���¸� ������� ����Ѵ�. ���� ���� ���谡 �Ǿ��ִٸ� �� ������ ������ ��ȣ�� ����ϰ�, �ƴ϶�� ��SYJKGW���� �� ����ǥ ���� ����Ѵ�.



/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	 // scanf ���� ����

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>
#include <utility>
#include <limits.h>	// INT_MAX ����

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
		// num : �Է� ��
		cin >> cnt;
		for (; cnt; cnt--) {
			cin >> num;
			vec.insert(num);
			//cout << num << " ";
		}

		// num : ���� ��
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
		if (cnt > max)	// ������ ���ڿ� ����
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

		sin.clear();						// ���� ����
		vec.clear();						// vector ����
	}
}
