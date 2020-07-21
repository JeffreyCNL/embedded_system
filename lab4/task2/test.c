#include <stdio.h>

int main(){

	float num = 12.32;
	long temp = (long)num;
	printf("%lu\n", (long) num * (1 << 12));
}