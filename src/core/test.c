#include<stdlib.h>

int main() {
	int A[4] = (int[]) {1, 2, 3, 4};
	#pragma clang loop vectorize(enable)
	for (int i = 0; i < 4; i++) {
  		A[i] = i * 4;
	}
}
