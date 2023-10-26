int NqueenTest(int* nqueen_array, int length)		//传入N皇后的数组(从0开始存)以及N的大小
{
	int size_set = 10;
	if (length <= 4) {
		printf("棋盘太小！\n");
		return -4;
	}
	if (length <= size_set) {			//小棋盘二维打印
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				if (j == nqueen_array[i]) {
					printf("* ");
				}
				else {
					printf("0 ");
				}
			}
			printf("\n");
		}
	}
	else {
		for (int i = 0; i < length; i++) {
			printf("%d\n", nqueen_array[i]);
		}
	}
	for (int i = 0; i < length; i++) {
		if (nqueen_array[i] < 0 || nqueen_array[i] >= length) {
			printf("皇后摆出棋盘了\n");
			return -3;
		}
		for (int j = i + 1; j < length; j++) {
			if (nqueen_array[i] == nqueen_array[j]) {
				printf("直线冲突\n");
				return -2;
			}
			if (abs(nqueen_array[i] - nqueen_array[j]) == abs(i - j)) {		//abs需要include<stdlib.h>
				printf("斜线冲突\n");
				return -1;
			}
		}
	}
	printf("N皇后的解正确\n");
	return 1;
}