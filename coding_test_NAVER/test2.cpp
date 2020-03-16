// NAVER_coding.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include <iostream>
#include <string>
#include <random>

#define TTT printf(" ** TEST0 **\n");
#define TTT1 printf(" ** TEST1 **\n");
#define TTT2 printf(" ** TEST2 **\n");
#define TTT3 printf(" ** TEST3 **\n");
#define TTT4 printf(" ** TEST4 **\n");

using namespace std;

std::random_device rd; std::mt19937 mersenne(rd()); 
std::uniform_int_distribution<> die('a', 'z');


bool get_str(string s, char* c, int& len) {
	const char* c_str = s.c_str();
	
	len = 0;
	for (; c_str[len] != NULL; len++) {
		if (c_str[len] != '?')
		{
			if (c_str[len] < 'a') {
				return 0;
			}
			else if (c_str[len] > 'z') {
				return 0;
			}
		}
		c[len] = c_str[len];
	}
	c[len] = NULL;
	return 1;
}
int sum_flag(int* flag, int num) {
	int sum = 0;
	for (int i = 0; i < num; i++, sum = flag[i]);
	return sum;
}
int check_flag(int* flag1, int* flag2, int num) {
	for (int i = 0; flag1[i] == flag2[i]; i++) {
		if (i == num - 1) {
			return 1;
		}
	}
	return 0;
}

bool check_string(string str, char* c_str) {

	int len = 0;
	get_str(str, c_str, len);
	for (int i = 0; i <= len / 2; i++) {
		int flag[3] = {0};
		if (c_str[i] == '?') {
			flag[0] = 1;
		}
		if (c_str[len - i - 1] == '?') {
			flag[1] = 1;
		}
		if (c_str[i] != c_str[len - i - 1]) {
			flag[2] = 1;
		}
		int f110[3] = { 1, 1, 0 },// 110
			f101[3] = { 1, 0, 1 },// 101
			f011[3] = { 0, 1, 1 };// 011

		int f001[3] = { 0, 0, 1 },// 001 NO
			f000[3] = { 0, 0, 0 };// 000 pass 

		//printf("  ## %c, %c \n", c_str[i], c_str[len - i - 1]);
		//printf("i : %d, flag : %d, %d, %d \n\n", i, flag[0], flag[1], flag[2]);

		// 111 NO error
		// 100 NO error
		// 010 NO error

		if (check_flag(flag, f000, 3)) {
			continue;
		}
		else if(check_flag(flag, f001, 3)) {
			cout << "대응 되는 위치에 서로 다른 문자가 있거나 규정외 문자가 포함되어있습니다 있습니다" << endl
				<< " 'a' to 'z', '?'로 구성된 회문으로 바꿀 수 있는 문자열을 제시 해주세요." << endl;
			return 0;
		}
		else if (check_flag(flag, f110, 3)) {
			c_str[i] = c_str[len - i -1] = die(mersenne);
		}
		else if (check_flag(flag, f101, 3)) {
			c_str[i] = c_str[len - i - 1];
		}
		else if (check_flag(flag, f011, 3)) {
			 c_str[len - i - 1] = c_str[i];
		}
		else {
			cout << " ** coding error !!" << endl;
		}
	}
	return 1;
}


string palindrome(string str) {

	char* c_str = new char[1000]; //1000 글자 제한

	if (check_string(str, c_str)) {
		return string(c_str)+'\n';
	}
	else {
		return "NO\n";
	}
}

int main()
{
	cout << palindrome("?ab??a") << endl;
	cout << palindrome("bab??a") << endl;

	cout << palindrome("H?") << endl;

	cout << palindrome("????asdf") << endl;
	cout << palindrome("a?b?ca") << endl;
	cout << palindrome("??") << endl;
}


