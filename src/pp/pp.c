#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#if !__APPLE__
/*
 * Appends src to string dst of size dsize (unlike strncat, dsize is the
 * full size of dst, not space left).  At most dsize-1 characters
 * will be copied.  Always NUL terminates (unless dsize <= strlen(dst)).
 * Returns strlen(src) + MIN(dsize, strlen(initial dst)).
 * If retval >= dsize, truncation occurred.
 */
size_t
strlcat(char *dst, const char *src, size_t dsize)
{
	const char *odst = dst;
	const char *osrc = src;
	size_t n = dsize;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end. */
	while (n-- != 0 && *dst != '\0')
		dst++;
	dlen = dst - odst;
	n = dsize - dlen;

	if (n-- == 0)
		return(dlen + strlen(src));
	while (*src != '\0') {
		if (n != 0) {
			*dst++ = *src;
			n--;
		}
		src++;
	}
	*dst = '\0';

	return(dlen + (src - osrc));	/* count does not include NUL */
}
#endif /* !__APPLE__ */

#define MAXSIZE 12000
#define MAXINDENTSIZE 80
#define MAXARGSIZE 20

char *argv0;

int
efgetc(FILE *fp)
{
	int c;

	c = fgetc(fp);

	if (c == EOF &&
		ferror(fp) != 0) {
			perror(argv0);
			exit(1);
	}

	return c;
}

int
efseek(FILE *fp, long offset, int whence)
{
	int s;

	s = fseek(fp, offset, whence);

	if (s == -1) {
		perror(argv0);
		exit(1);
	}

	return s;
}

int
eungetc(int c, FILE *fp)
{
	int uc;

	uc = ungetc(c, fp);

	if (uc == EOF) {
		perror(argv0);
		exit(1);
	}

	return uc;
}

size_t
estrlcat(char *dst, const char *src, size_t dstsize)
{
	size_t n;

	n = strlcat(dst, src, dstsize);

	if (n >= dstsize) {
		fprintf(stderr, "%s: Buffer overflow\n", argv0);
		exit(1);
	}

	return n;
}

int
eclose(int d)
{
	int s;

	s = close(d);

	if (s == -1) {
		perror(argv0);
		exit(1);
	}

	return s;
}

char *
strappend(char *s, char c)
{
	size_t len;

	len = strlen(s);

	s[len] = c;
	s[len + 1] = '\0';

	return s ;
}

int
shebang(FILE *fp)
{
	int c;

	c = efgetc(fp);

	if (c != '#') {
		eungetc(c, fp);
		return 0;
	}

	c = efgetc(fp);

	if (c != '!') {
		eungetc(c, fp);
		eungetc('#', fp);
		return 0;
	}

	c = efgetc(fp);

	if (c == '\n') {
		eungetc(c, fp);
		eungetc('!', fp);
		eungetc('#', fp);
		return 0;
	}

	for (;;) {
		c = efgetc(fp);
		if (c == '\n')
			break;
	}

	return 1;
}

int
token(FILE *fp, int consume)
{
	int c;

	c = efgetc(fp);

	if (c != '#') {
		eungetc(c, fp);
		return 0;
	}

	c = efgetc(fp);
	if (c != '!') {
		eungetc(c, fp);
		eungetc('#', fp);
		return 0;
	};

	c = efgetc(fp);

	if (c != '\n') {
		eungetc(c, fp);
		eungetc('!', fp);
		eungetc('#', fp);
		return 0;
	}

	if (consume == 0) {
		eungetc(c, fp);
		eungetc('!', fp);
		eungetc('#', fp);
	}

	return 1;
}

size_t
output(char *s)
{
	return estrlcat(s, "echo \"", MAXSIZE);
}

size_t
outputend(char *s)
{
	return estrlcat(s, "\"\n", MAXSIZE);
}

int main(int argc, char **argv)
{
	char s[MAXSIZE] = "", indent[MAXINDENTSIZE] = "",
		*eargv[MAXARGSIZE] = {"sh", "-c", s, "sh", NULL};
	FILE *fp;
	int size, c, i, fd[2], o = 0, status = 0, d = 0;


	argv0 = argv[0];
	if (argc == 1) {
		fprintf(stderr, "usage: %s file [args ...]\n", argv[0]);
		return 1;

	}

	if (strcmp(argv[1], "-d") == 0) {
		if (argc == 2) {
			fprintf(stderr, "usage: %s file [args ...]\n", argv[0]);
			return 1;

		}
		d = 1;
		fp = fopen(argv[2], "r");
	} else {
		fp = fopen(argv[1], "r");
	}


	if (!fp) {
		fprintf(stderr, "%s: %s: %s\n",
			argv[0], d ? argv[2] : argv[1], strerror(errno));
		return 1;
	}

	efseek (fp, 0, SEEK_END);
	size = ftell(fp);

	if(size == 0) return 0;

	efseek(fp, -1, SEEK_END);
	c = efgetc(fp);

	if (c != '\n') {
		fprintf(stderr, "%s: %s: No newline at end of file\n",
			argv[0], d ? argv[2] : argv[1]);
		exit(1);
	}

	efseek(fp, 0, SEEK_SET);

	shebang(fp);

	if (!token(fp, 1)) {
		output(s);
		o = 1;
	}

	for(;;) {
		if (token(fp, 1)) {
			if (!o) {
				c = efgetc(fp);
				if (c == EOF)
					goto done;
				eungetc(c, fp);
				output(s);
			} else
				outputend(s);
			o = !o;
		}

		c = efgetc(fp);
		if (c == EOF)
			break;

		if (c == '\n') {
			indent[0] = 0;
			c = efgetc(fp);
			if (c == EOF)
				break;

			for (;;) {
				if (c != ' ' && c != '\t')
					break;
				strappend(indent, c);
				c = efgetc(fp);
				if (c == EOF)
					break;
			}

			eungetc(c, fp);

			if (token(fp, 0) && o)
				continue;

			strappend(s, '\n');
			estrlcat(s, indent, MAXSIZE);
			continue;
		}

		strappend(s, c);
	}
	if (o) {
		outputend(s);
		o = 0;
	}
	done:

	if(d) {
		puts(s);
		return 0;
	}

	if (pipe(fd) == -1) {
		perror(argv0);
		return 1;
	}

	switch (fork()) {
		case -1:
			perror(argv0);
			return 1;
		case 0:
			eclose(fd[1]);
			eclose(fd[0]);
			for (i = 2; i < argc; i++)
				eargv[i+2] = argv[i];

			if (execv("/bin/sh", eargv) == -1) {
				perror(argv0);
				return 1;
			}
		break;
		default:
			eclose(fd[1]);
			eclose(fd[0]);
			if (wait(&status) == -1) {
				perror(argv0);
				return 1;
			}
	}

	return status;
}
