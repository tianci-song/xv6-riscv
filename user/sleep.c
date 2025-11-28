#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("arg count not correct, it should be 1.\n");
		exit(1);
	}
	int n = atoi(argv[1]);
	if (pause(n) == -1) {
		printf("sleep error!\n");
		exit(1);
	}

	exit(0);
}
