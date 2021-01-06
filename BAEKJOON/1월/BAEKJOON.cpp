#define _CRT_SECURE_NO_WARNINGS	// scanf 에러 무시
//	cin.tie(NULL);
//	ios::sync_with_stdio(false);





#include<stdio.h>
// 1110
int main() {
    int n, init, count = 0;
    scanf("%d", &n);
    init = n;
    int a, b, c, d;
    do {
        a = n / 10;
        b = n % 10;
        c = (a + b) / 10;
        d = (a + b) % 10;
        n = b * 10 + d;
        count++;
    } while (n != init);
    printf("%d", count);
}



//#include <iostream>
//using namespace std;
//
//// 10951
//int main(void)
//{
//    int a = 0;
//    int b = 0;
//
//    while (cin >> a >> b)
//    {
//        cout << a + b << endl;
//    }
//}



//#include <iostream>
//using namespace std;
//
//// 10952
//int main(void) {
//    int a, b;
//
//    while (1) {
//        cin >> a >> b;
//        if (a != 0 || b != 0)
//            cout << a + b << endl;
//        else
//            break;
//    }
//}


//
//#include <stdio.h>
//
//// 10871
//int main() {
//    int n, x;
//    scanf("%d %d", &n, &x);
//
//    int value;
//    for (int i = 0; i < n; ++i) {
//        scanf("%d", &value);
//        if (value < x) printf("%d ", value);
//    }
//    return 0;
//}


//#include <iostream>
//using namespace std;
//
//// 2438
//int main()
//{
//    cin.tie(NULL);
//    ios::sync_with_stdio(false);
//
//    int T;
//    cin >> T;
//    for (int i = 1; i <= T; i++){
//        for (int j = 0; j < T-i; j++)
//            cout << ' ';
//        for (int j = 0; j < i; j++)
//            cout << '*';
//        cout << '\n';
//    }
//}


//#include <iostream>
//using namespace std;
//
//// 11022
//int main()
//{
//    cin.tie(NULL);
//    ios::sync_with_stdio(false);
//
//    int T;
//    int  A, B;
//    cin >> T;
//    for (int i = 1; i <= T; i++)
//    {
//        cin >> A >> B;
//        cout << "Case #" << i << ": " << A << " + " << B  << " = "<< A + B << endl;
//    }
//}



//#include <iostream>
//
//using namespace std;
//
//// 11021
//int main()
//{
//    cin.tie(NULL);
//    ios::sync_with_stdio(false);
//
//    int T;
//    int  A, B;
//    cin >> T;
//    for (int i = 1; i <= T; i++)
//    {
//        cin >> A >> B;
//        cout << "Case #" << i << ": " << A + B << endl;
//    }
//}


//#include <stdio.h>
//
//// 2742
//int main() {
//	int n;
//	scanf("%d", &n);
//	for (int i = n; i >= 1; --i) {
//		printf("%d\n", i);
//	}
//	return 0;
//}



//#include <stdio.h>
//
//// 2741
//int main() {
//	int n;
//	scanf("%d", &n);
//	for (int i = 1; i <= n; ++i) {
//		printf("%d\n", i);
//	}
//	return 0;
//}



//#include <iostream>
//#include <string>
//
//using namespace std;
//
//// 15552
//int main() {
//
//	int T;
//	int a, b;
//
//	// 빠른 A + B
//	cin.tie(NULL);
//	ios::sync_with_stdio(false);
//
//	cin >> T;
//
//	for (int i = 0; i < T; i++) {
//		cin >> a >> b;
//		cout << a + b << '\n';
//	}
//}



//#include <stdio.h>
//
//// 8393
//int main() {
//    int n;
//    scanf("%d", &n);
//
//    int sum = 0; // 시그마
//    for (int i = 1; i <= n; ++i) {
//        sum += i;
//    }
//    printf("%d\n", sum);
//
//    return 0;
//}



//#include <iostream>
//using namespace std;
//
//// 10950
//int main(void) {
//    int n, a, b;
//    cin >> n;
//    for (int i = 0; i < n; i++) {
//        cin >> a >> b;
//        cout << a + b << endl;
//    }
//}


//#include <stdio.h>
//
//// 2739
//int main() {
//    int n;
//    scanf("%d", &n);    // 구구단
//    for (int i = 1; i < 10; ++i) {
//        printf("%d * %d = %d\n", n, i, n * i);
//    }
//    return 0;
//}


//#include <stdio.h>
//
//// 2753
//int main() 
//{
//	int year; // 1 ~ 4000의 정수
//	scanf("%d", &year);
//
//	// 윤년은 4의 배수 이면서 100의 배수가 아니다
//	// 또는 400의 배수이다
//	bool result = (year % 4 == 0) - (year % 100 == 0) + (year % 400 == 0);
//	printf("%d", result);
//}


//#include <stdio.h>
//
//// 9498
//int main() 
//{
//	int A; // 0 ~ 100의 정수
//	scanf("%d", &A);
//	if (A < 60)
//		printf("F");
//	else if (A > 89)
//		printf("A");
//	else
//		printf("%c", 'D' - ((A / 10) - 6));
//	
//}

//#include <stdio.h>
//
//// 2588
///// <summary>
///// 세자리수 * 세자리수 곱셈
///// 중간과정 출력하기
///// </summary>
///// <returns></returns>
//int main() 
//{
//	int A, B;	// 세자리수 자연수
//	scanf("%d %d", &A, &B);
//	int b1, b2, b3; // B의 각 자리수 정수( 0 ~ 9 )
//	b1 = B / 100;
//	b2 = (B % 100) / 10;
//	b3 = (B % 10);
//	printf("%d\n", b3 * A);
//	printf("%d\n", b2 * A);
//	printf("%d\n", b1 * A);
//	printf("%d\n", B * A);
//}


//#include <stdio.h>
//int main() // 10430
//{
//    int A, B, C;
//    scanf_s("%d %d %d", &A, &B, &C);
//    printf("%d\n", (A + B) % C);
//    printf("%d\n", ((A % C) + (B % C)) % C);
//    printf("%d\n", (A*B) % C);
//    printf("%d\n", ((A % C) * (B % C)) % C);
//}



//#include <stdio.h>
//int main() // 10869
//{
//    int a, b;
//    scanf("%d %d", &a, &b);
//    printf("%d\n", a + b);
//    printf("%d\n", a - b);
//    printf("%d\n", a * b);
//    printf("%d\n", a / b);
//    printf("%d\n", a % b);
//}



//#include <stdio.h>
//int main() // 10998
//{
//    int a, b;
//    scanf_s("%d %d", &a, &b);
//    printf("%d", a * b);
//}



//#include <iostream>
//int main() // 10172
//{
//    std::cout << "|\\_/|\n";
//    std::cout << "|q p|   /}\n";
//    std::cout << "( 0 )\"\"\"\\\n";
//    std::cout << "|\"^ \"`    |\n";
//    std::cout << "||_/=\\\\__|";
//}



//#include <iostream>
//int main() // 10718
//{
//    std::cout << "강한친구 대한육군\n";
//    std::cout << "강한친구 대한육군";
//}

