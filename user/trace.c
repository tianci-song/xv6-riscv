#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("usage: trace <mask> <syscall> (<args>)\n");
		exit(1);
	}
	int mask = atoi(argv[1]);	// mask for enabling which sys call should be traced
	trace(mask);	// user space trace() invokes the syscall trace()
	if (!fork()) {
		char* params[MAXARG];
		for (int i = 0; i < argc - 2; ++i) {
			params[i] = argv[i+2];
		}
		exec(argv[2], params);
	}
	else {
		wait(0);
	}
	exit(0);
}
