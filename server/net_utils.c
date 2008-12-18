#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "net_utils.h"

int debug = 0;

int
str_to_argv (char *str, char ***argv)
{
	int argc = 0, alloc = STD_ALLOC;
	char **targv;

	*argv = NULL;

	if (!*str || !(targv = (char **) malloc((1 + alloc) *sizeof(char *))))
		return argc;

	for (;;) {
		while (*str && isspace(*str))
			*str++ = '\0';
		if (!*str)
			break;

		if (argc >= MAX_ARGC)
			break;

		if (argc >= alloc) {
			alloc *= 2;
			if (!(targv = (char **) realloc(targv, (1 + alloc) * sizeof(char *))))
				return argc;
		}
		targv[argc++] = str;
		while(*str && !isspace(*str))
			str++;
		if (!*str)
			break;
	}
	targv[argc] = NULL;
	*argv = targv;

	return argc;
}

char *
argv_to_str (char **argv)
{
	char *buf, *token;
	int alloc = STD_BUF, len = 0, slen, i;

	if (!(buf = (char *) malloc(alloc * sizeof(char))))
		return buf;
	for (i = 0; argv[i]; i++) {
		token = argv[i];
		slen = strlen(token);
		if (slen >= alloc + len + 1) {
			alloc = MAX (2*len, len + 2*slen + 1);
			if (!(buf = (char *) realloc(buf, alloc * sizeof(char))))
				return buf;
		}
		memcpy(buf+len, token, slen);
		len += slen;
		buf[len++] = ' ';
	}
	buf[len] = '\0';

	return buf;
}


void 
ndprintf (FILE *fd, char *fmt, ...)
{
	va_list vp;

	if (debug) {
		va_start (vp, fmt);
		vfprintf(fd, fmt, vp);
		va_end(vp);
	}
}

void
ndprintf_die (FILE *fd, char *fmt, ...)
{
	va_list vp;
	if (debug) {
		va_start(vp, fmt);
		vfprintf(fd, fmt, vp);
		va_end(vp);
	}
	exit(EXIT_FAILURE);
}

void sockwrite (int fd, int status, char *fmt, ...)
{
	char *str, *tmp;

	va_list vp;
	if (fmt) {
		va_start(vp, fmt);
		vasprintf(&tmp, fmt, vp);
		va_end(vp);
		asprintf(&str, "%d %s", status, tmp);
	}
	else
		asprintf(&str, "%d", status);
	write(fd, str, strlen(str));
	if (fmt)
		free(tmp);
	free(str);
}

int
str_isnumber (char *str)
{
	const int len = strlen(str);
	int i;

	for (i = 0; i < len; i++) {
		if (!isnumber(str[i]))
			return 0;
	}
	return 1;
}
