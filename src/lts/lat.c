#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#if !__APPLE__
#include "strlcpy.h"
#endif /* !__APPLE__ */

int main(int argc, char *argv[])
{
	struct stat st;
	char lmt[100];
	char f[100] = "%a %b %e %H:%M:%S %Z %Y";
	int i = 1;


	if (argc < 2) {
		fprintf(stderr, "usage: %s [-f format] file...\n", argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "-f") == 0) {
		if (argv[2] == NULL) {
			fprintf(stderr, "usage: %s [-f format] file...\n", argv[0]);
			return 1;
		}

		if (strlcpy(f, argv[2], sizeof(f)) >= sizeof(f)) {
			fprintf(stderr, "%s: Buffer overflow\n", argv[0]);
			return 1;
		}

		i = 3;
	}

	if (argc - i < 1) {
		fprintf(stderr, "usage: %s [-f format] file...\n", argv[0]);
		return 1;
	}

	for (; i < argc ; i++ ) {
		if (lstat(argv[i], &st) == -1) {
			perror(argv[0]);
			return 1;
		}

	        strftime(lmt, sizeof(lmt), f, localtime(&st.st_atime));

		if (printf("%s %s\n", lmt, argv[i]) < 0) {
			perror(argv[0]);
			return 1;
		}
	}

	return 0;
}
