#include <stdio.h>

//這裡用了int型別，所支持的整數範圍較小，且本程序僅支持非負整數，可能缺乏實際用途，僅供展示。
struct EX_GCD { //s,t是裴蜀等式中的係數，gcd是a,b的最大公因數
	int s;
	int t;
	int gcd;
};

struct EX_GCD extended_euclidean(int a, int b) {
	struct EX_GCD ex_gcd;
	if (b == 0) { //b=0時直接結束求解
		ex_gcd.s = 1;
		ex_gcd.t = 0;
		ex_gcd.gcd = 0;
		return ex_gcd;
	}
	int old_r = a, r = b;
	int old_s = 1, s = 0;
	int old_t = 0, t = 1;
	while (r != 0) { //按擴展歐基里德算法進行循環
		int q = old_r / r;
		int temp = old_r;
		old_r = r;
		r = temp - q * r;
		temp = old_s;
		old_s = s;
		s = temp - q * s;
		temp = old_t;
		old_t = t;
		t = temp - q * t;
	}
	ex_gcd.s = old_s;
	ex_gcd.t = old_t;
	ex_gcd.gcd = old_r;
	return ex_gcd;
	}

int main(void) {
	int a, b;
	printf("Please input two integers divided by a space.\n");
	scanf("%d%d", &a, &b);
	if (a < b) { //若a < b，則交換a與b以便求解
		int temp = a;
		a = b;
		b = temp;
	}
	struct EX_GCD solution = extended_euclidean(a, b);
	printf("%d*%d+%d*%d=%d\n", solution.s, a, solution.t, b, solution.gcd);
	printf("Press any key to exit.\n");
	getchar();
	getchar();
	return 0;
}