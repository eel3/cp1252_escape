/* ********************************************************************** */
/**
 * @brief   Command line tool to escape characters greater than 0x7F.
 * @author  eel3
 * @date    2020/06/21
 *
 * @par Compilers
 * - TDM-GCC 4.8.1 (Windows 7 64bit SP1)
 * - Visual Studio 2005 SP1 (Windows 7 64bit SP1)
 * - Visual Studio 2010 SP1 (Windows 7 64bit SP1)
 */
/* ********************************************************************** */


#if defined(_WIN32) || defined(_WIN64)
#	if !defined(__MINGW32__) && !defined(__MINGW64__)
#		ifndef _CRT_SECURE_NO_WARNINGS
#			define _CRT_SECURE_NO_WARNINGS
#		endif /* ndef _CRT_SECURE_NO_WARNINGS */
#	endif /* !defined(__MINGW32__) && !defined(__MINGW64__) */
#endif /* defined(_WIN32) || defined(_WIN64) */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ---------------------------------------------------------------------- */
/* Constant */
/* ---------------------------------------------------------------------- */

/** File separator */
#if defined(_WIN32) || defined(_WIN64)
#	define  PATH_SEP  '\\'
#else /* defined(_WIN32) || defined(_WIN64) */
#	define  PATH_SEP  '/'
#endif /* defined(_WIN32) || defined(_WIN64) */


/* ---------------------------------------------------------------------- */
/* Variable */
/* ---------------------------------------------------------------------- */

/** The program name. */
static const char *program_name;


/* ---------------------------------------------------------------------- */
/* Function-like macro */
/* ---------------------------------------------------------------------- */

/* ====================================================================== */
/**
 * @brief  Return true if string s1 equal to s2.
 *
 * @param[in] s1  string 1.
 * @param[in] s2  string 2.
 *
 * @retval !=0  Equal.
 * @retval   0  Not equal.
 */
/* ====================================================================== */
#define  STREQ(s1, s2)  (((s1)[0] == (s2)[0]) && (strcmp((s1), (s2)) == 0))


/* ---------------------------------------------------------------------- */
/* Function */
/* ---------------------------------------------------------------------- */

/* ====================================================================== */
/**
 * @brief  Tiny copy of basename(3).
 *
 * @param[in] *name  (See man 3 basename).
 *
 * @return  (See man 3 basename).
 */
/* ====================================================================== */
static const char *my_basename(const char * const name)
{
	const char *bn;

	assert(name != NULL);

	bn = strrchr(name, PATH_SEP);
	return (bn == NULL) ? name : bn+1;
}

/* ====================================================================== */
/**
 * @brief  Show usage,
 *
 * @param[in,out] *out  Output stream.
 */
/* ====================================================================== */
static void usage(FILE * const out)
{
	assert(out != NULL);

	(void) fprintf(out, "usage: %s [-h] [file ...]\n", program_name);
}

/* ====================================================================== */
/**
 * @brief  Escape characters greater than 0x7F.
 *
 * @param[in,out] *in   input stream
 * @param[in,out] *out  output stream
 */
/* ====================================================================== */
static void do_escape(FILE *in, FILE *out)
{
	int c;

	assert((in != NULL) && (out != NULL));

	while ((c = fgetc(in)) != EOF) {
		if (c >= 0x80) {
			(void) fprintf(out, "\\x%02X", (unsigned) c);
		} else {
			(void) fputc(c, out);
		}
	}
}

/* ********************************************************************** */
/**
 * @brief  Main routine.
 *
 * @retval EXIT_SUCCESS  OK (success).
 * @retval EXIT_FAILURE  NG (failure).
 */
/* ********************************************************************** */
int main(int argc, char *argv[])
{
	int retval;

	program_name = my_basename(argv[0]);

	for (; (argc > 1) && (argv[1][0] == '-') && (argv[1][1] != '\0'); argc--, argv++) {
		const char *p = &argv[1][1];

		if (argv[1][1] == '-') {
			p = &argv[1][2];

			if (*p == '\0') {
				argc--, argv++;
				break;
			} else if (STREQ(p, "help")) {
				usage(stdout);
				return EXIT_SUCCESS;
			} else {
				usage(stderr);
				return EXIT_FAILURE;
			}
			continue;
		}

		do switch (*p) {
		case 'h':
			usage(stdout);
			return EXIT_SUCCESS;
		default:
			usage(stderr);
			return EXIT_FAILURE;
		} while (*++p != '\0');
	}

	retval = EXIT_SUCCESS;

	if (argc <= 1) {
		do_escape(stdin, stdout);
	} else {
		int i;
		FILE *in;

		for (i = 1; i < argc; ++i) {
			if (STREQ(argv[i], "-")) {
				in = stdin;
			} else if (errno = 0, (in = fopen(argv[i], "r")) == NULL) {
				perror(argv[i]);
				retval = EXIT_FAILURE;
				continue;
			} else {
				/*EMPTY*/
			}

			do_escape(in, stdout);

			if (in != stdin) {
				(void) fclose(in);
			}
		}
	}

	return retval;
}
