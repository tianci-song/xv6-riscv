#include "kernel/types.h"
#include "user/user.h"

/* It should be noted that read is called three times, the third read is seperated
with the second read to avoid forking the process each time the pipe read. */

void new_proc(int* p) {
	close(p[1]);
	int prime;
	if (read(p[0], &prime, 4) != 4) {	// read first time, extract the prime number
		printf("process %d read prime number error!\n", prime);
		exit(1);
	}
	printf("primes: %d\n", prime);

	int num;
	if (read(p[0], &num, 4)) { // read second time, if no more data, then return, else fork new process
		int q[2];
		pipe(q);
		if (!fork()) { new_proc(q); }
		else {
			close(q[0]);
			if (num % prime) { write(q[1], &num, 4); }
			while (read(p[0], &num, 4)) {	// read third time, read for loop.
				if (num % prime) { write(q[1], &num, 4); }
			}
			close(q[1]);
			wait(0);
		}
	}
	close(p[0]);
}

int main() {
	int maxN = 35;
	int p[2];
	pipe(p);
	if (!fork()) { new_proc(p); }
	else {
		close(p[0]);
		for (int i=2; i<=maxN; ++i) { 
			write(p[1], &i, 4);
		}
		close(p[1]);
		wait(0);
		exit(0);
	}
	return 0;
}
