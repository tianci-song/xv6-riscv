#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("args count not correct! It should be 2.\n");
		exit(1);
	}
	int p[2];
	char byte;
	pipe(p);
	int parent_id = getpid();
	int child_id = fork();
	if (child_id == 0) {
		int n = read(p[0], &byte, 1);	// read one byte from parent process
		if (n!=1) {
			printf("child process read incorrect number of bytes.\n");
			exit(1);
		}
		printf("child process, id: %d read %d byte %c from parent process, id: %d.\n", 
			getpid(), n, byte, parent_id);
		close(p[0]);
		write(p[1], &byte, 1);
		close(p[1]);
		exit(0);
	} else {
		byte = argv[1][0];
		write(p[1], &byte, 1);
		wait(0);	// wait for the child process to receive the byte
		close(p[1]);
		int n = read(p[0], &byte, 1);
		if (n!=1) {
			printf("parent process read incorrect number of bytes.\n");
			exit(1);
		}
		printf("parent process, id: %d read %d byte %c from child process, id: %d.\n", 
			parent_id, n, byte, child_id);
		close(p[0]);
	}
	exit(0);
}
