#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char* path, char* filename) {
	int fd;
	struct stat st;
	struct dirent de;
	char buf[512];

	if ((fd = open(path, O_RDONLY)) < 0) {
		printf("find: cannot open %s\n", path);
		exit(1);
	}
	if (fstat(fd, &st) < 0) {
		printf("find: cannot stat %s\n", path);
		exit(1);
	}
	if (st.type != T_DIR) {
		printf("find: %s is not a directory\n", path);
		exit(1);
	}
	if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {	// add 1 twice: one for '/', one for '\0'
		printf("find: path too long.\n");
		exit(1);
	}
	strcpy(buf, path);
	char* p = buf + strlen(buf);
	*p++ = '/';
	while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		if (!strcmp(de.name, ".") || !strcmp(de.name, "..") ||	// not recurse into the "." and ".."
			de.inum == 0) continue;		// will read the empty, should not be recursed	
		strcpy(p, de.name);
		if (stat(buf, &st) < 0) {
			printf("cannot stat %s\n", buf);
			exit(1);
		}
		if (st.type == T_FILE) {
			if (!strcmp(de.name, filename)) {
				printf("%s\n", buf);
				continue;
			}
		}
		else if (st.type == T_DIR) { find(buf, filename); }
	}
	close(fd);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("please enter the searching path and filename.\n");
		exit(1);
	}
	else if (argc > 3) {
		printf("too many arguments. Format should be find <path> <file>\n");
		exit(1);
	}
	else if (argc == 2) { find(".", argv[1]); }
	else { find(argv[1], argv[2]); }
	
	exit(0);
}
