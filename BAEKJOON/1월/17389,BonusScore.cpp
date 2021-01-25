// 17389

/// OX표에 N개의 문제들이 있을 때, 1번 문제, 2번 문제, ..., N번 문제 순으로 채점된다.
/// 문제는 뒤로 갈수록 어려워지기 때문에, i 번 문제의 기본 점수는 i 점이다.
/// 문제를 맞히면 그 문제의 기본 점수(즉 i 번 문제의 경우 i 점)를 획득하며, 틀리면 얻지 못한다.
/// 기본 점수와 별개로, '보너스 점수'라는 값이 존재한다.이는 처음에는 0점이다.
/// 문제를 맞히면 그 때의 '보너스 점수'를 획득하고, '보너스 점수'의 값이 1점 증가한다.
/// 문제를 틀리면 '보너스 점수'를 얻지 못하고, '보너스 점수'의 값이 0점으로 초기화된다.
/// 민성이는 얼떨결에 숭고한 알고리즘 캠프 퀴즈 타임의 OX표를 채점해야 하는 업무를 맡게 되었다.
/// 수많은 OX표를 볼 생각에 머리가 지끈거리는 민성이는 프로그램을 세워 이를 자동화하려고 한다.

/// pre  : 첫 번째 줄에 OX표의 길이인 자연수 N이 주어진다. (1 ≤ N ≤ 10,000)
/// 두 번째 줄에 OX표를 의미하는 문자열 S가 주어진다.S는 O(알파벳 대문자 O, ASCII 코드 79)와 X(알파벳 대문자 X, ASCII 코드 88)로만 구성되어 있으며, 길이는 N이다.
/// 문자열 S의 i 번째 글자가 O이면 해당 참가자가 i 번째 문제를 맞혔음을 의미하고, X이면 틀렸음을 의미한다.

/// post : 첫 번째 줄에 입력으로 들어온 OX표의 점수를 출력한다.


/// <summary>
///
/// </summary>
/// <returns></returns>


#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시

#include <stdio.h>
#include <iostream>
#include <algorithm>

using namespace std;



int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	
	int N;			cin >> N;
	string str;		cin >> str;

	// XOOOXOX
	int score = 0, bonus = 0, i = 0;
	for (const auto& item : str) {	i++;
		if ('O' == item) {
			score += i + bonus;
			bonus++;
		}
		else {
			bonus = 0;
		}
	}
	cout << score;
}

