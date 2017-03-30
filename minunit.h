/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <zboub@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 11:31:07 by mcanal            #+#    #+#             */
/*   Updated: 2017/03/30 23:29:06 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
** MU_HAI
** MU_KTHXBYE
** MU_DUMMY_FUN
**
** MU_TEST_FUN
** MU_TEST_SUITE
**
** MU_INIT_TEST
** MU_INIT_SUITE
**
** MU_RUN_TEST
** MU_RUN_SUITE
**
** MU_ASSERT
** MU_ASSERT_FATAL
**
** -TODO:
** MU_PASS
** MU_FAIL
** MU_FAIL_FATAL
**
** MU_ASSERT_EQUAL
** MU_ASSERT_EQUAL_FATAL
** MU_ASSERT_NOT_EQUAL
** MU_ASSERT_NOT_EQUAL_FATAL
**
** MU_ASSERT_STR_EQUAL
** MU_ASSERT_STR_EQUAL_FATAL
** MU_ASSERT_STR_NOT_EQUAL
** MU_ASSERT_STR_NOT_EQUAL_FATAL
**
** MU_ASSERT_MEM_EQUAL
** MU_ASSERT_MEM_EQUAL_FATAL
** MU_ASSERT_MEM_NOT_EQUAL
** MU_ASSERT_MEM_NOT_EQUAL_FATAL
**
** MU_ASSERT_DOUBLE_EQUAL
** MU_ASSERT_DOUBLE_EQUAL_FATAL
** MU_ASSERT_DOUBLE_NOT_EQUAL
** MU_ASSERT_DOUBLE_NOT_EQUAL_FATAL
*/

#ifndef MINUNIT
# define MINUNIT

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

# define TRUE  1
# define FALSE 0

# define WHITE "\033[37;01m"
# define RED   "\033[31;01m"
# define GREEN "\033[32;01m"
# define BLUE  "\033[34;01m"
# define BASIC "\033[0m"

typedef void           (*MU_TEST_FUN)(void);

#define MAX_NAME_SIZE 63
struct s_fun
{
	MU_TEST_FUN f;
	char s[MAX_NAME_SIZE + 1];
};
typedef struct s_fun  t_fun;

typedef t_fun MU_TEST_SUITE[];

# define STRINGIFY(x) #x
# define TOSTRING(x)  STRINGIFY(x)
# define AT           __FILE__":"TOSTRING(__LINE__)

# define FAIL_MSG(test, type, msg) WHITE"\n"AT": "RED type WHITE msg	\
	BASIC"\n\t("#test")\n\t "GREEN"^\n"BASIC
# define FAIL_ASSERT_MSG(test, msg) FAIL_MSG(test, "Assertion failed: ", msg)
# define SIG_STR(sig) ((sig) >= MIN_SIG && (sig) <= MAX_SIG ? g_sig_str[(sig)] : "SIG???")
# define SIG_ASSERT_MSG(test, msg) FAIL_MSG(test, "%s caught: ", msg)
# define SUCCESS_ASSERT_MSG         GREEN"."BASIC

# define MU_PASS(msg, ...) printf(GREEN msg "\n" BASIC, ##__VA_ARGS__) //TODO
# define MU_FAIL(msg, ...) fprintf(stderr, RED FAIL_MSG("", "Error: ", msg), ##__VA_ARGS__) //TODO
# define MU_FAIL_FATAL(msg, ...) MU_FAIL(msg, ##__VA_ARGS__), return  //TODO

# define MU_ASSERT(test, msg, ...)										\
	do {																\
		if (g_exit) {													\
			g_exit = FALSE;												\
			return ;													\
		}																\
		if (!fork()) {													\
            alarm(3);													\
			if ((test)) {												\
				printf(SUCCESS_ASSERT_MSG);								\
				exit(EXIT_SUCCESS);										\
			}															\
			fprintf(stderr, FAIL_ASSERT_MSG(test, msg), ##__VA_ARGS__);	\
            exit(EXIT_FAILURE);											\
        } else {														\
			if (wait(&g_status) != -1 && WIFSIGNALED(g_status)) {		\
				fprintf(stderr, SIG_ASSERT_MSG(test, msg), SIG_STR(WTERMSIG(g_status)), ##__VA_ARGS__); \
			}															\
			if (WIFEXITED(g_status) && WEXITSTATUS(g_status) == EXIT_SUCCESS) {	\
				g_asserts_success++;									\
			} else {													\
				g_success = FALSE;										\
			}															\
			g_asserts_run++;											\
        }																\
	} while (0)

# define MU_ASSERT_FATAL(test, msg, ...)								\
	do {																\
		MU_ASSERT(test, msg, ##__VA_ARGS__);							\
		if (!WIFEXITED(g_status) || WEXITSTATUS(g_status) != EXIT_SUCCESS) { \
			return ;													\
		}																\
	} while (0) //TODO

# define MU_RUN_TEST(test)						\
	do {										\
		g_success = TRUE;						\
		test();									\
		g_tests_run++;							\
		if (g_success) {						\
			g_tests_success++;					\
		}										\
		printf("\n");							\
	} while (0)

# define MU_RUN_SUITE(suite, name, ...)									\
	do {                                                                \
		setbuf(stdout, NULL); /* TODO: find a better place for this */	\
        printf("+ Suite "name":\n", ##__VA_ARGS__);						\
		int tests_run = g_tests_run;									\
		int tests_success = g_tests_success;							\
		int asserts_run = g_asserts_run;								\
		int asserts_success = g_asserts_success;						\
        for (size_t ii = 0; ii < sizeof(suite) / sizeof(t_fun); ii++) {	\
			printf("++ Test %s: ", suite[ii].s);						\
            MU_RUN_TEST(suite[ii].f);									\
		}																\
		printf("Tests:    %d/%d\n", g_tests_success - tests_success , g_tests_run - tests_run); \
		printf("Asserts:  %d/%d\n", g_asserts_success - asserts_success, g_asserts_run - asserts_run); \
		if (g_asserts_success - asserts_success == g_asserts_run - asserts_run)	{ \
			printf(GREEN"Suite passed."BASIC"\n\n");					\
			g_suites_success++;											\
		} else {														\
			printf(RED"Suite failed."BASIC"\n\n");						\
		}																\
		g_suites_run++;													\
    } while (0)

#define MU_DUMMY_FUN(x)													\
	void  __attribute__((weak)) x()										\
	{																	\
		fprintf(stderr, WHITE"\n"AT": "RED"Error: "WHITE"missing function"BASIC"\n\t"#x"()\n\t"GREEN"^\n"BASIC"\n"); \
		g_success = FALSE;												\
		g_exit = TRUE;													\
	}



# define MU_HAI()														\
		char g_sig_str[MAX_SIG + 1][MAX_SIG_STR] = {					\
			"SIG???",													\
			"SIGHUP",			/* 1: Hangup (POSIX).  */				\
			"SIGINT",			/* 2: Interrupt (ANSI).  */				\
			"SIGQUIT",			/* 3: Quit (POSIX).  */					\
			"SIGILL",			/* 4: Illegal instruction (ANSI).  */	\
			"SIGTRAP",			/* 5: Trace trap (POSIX).  */			\
			"SIGABRT",			/* 6: Abort (ANSI).  */					\
			"SIGBUS",			/* 7: BUS error (4.2 BSD).  */			\
			"SIGFPE",			/* 8: Floating-point exception (ANSI).  */ \
			"SIGKILL",			/* 9: Kill, unblockable (POSIX).  */	\
			"SIGUSR1",			/* 10: User-defined signal 1 (POSIX).  */ \
			"SIGSEGV",			/* 11: Segmentation violation (ANSI).  */ \
			"SIGUSR2",			/* 12: User-defined signal 2 (POSIX).  */ \
			"SIGPIPE",			/* 13: Broken pipe (POSIX).  */			\
			"SIGALRM",			/* 14: Alarm clock (POSIX).  */			\
			"SIGTERM",			/* 15: Termination (ANSI).  */			\
			"SIGSTKFLT",		/* 16: Stack fault.  */					\
			"SIGCHLD",			/* 17: Child status has changed (POSIX).  */ \
			"SIGCONT",			/* 18: Continue (POSIX).  */			\
			"SIGSTOP",			/* 19: Stop, unblockable (POSIX).  */	\
			"SIGTSTP",			/* 20: Keyboard stop (POSIX).  */		\
			"SIGTTIN",			/* 21: Background read from tty (POSIX).  */ \
			"SIGTTOU",			/* 22: Background write to tty (POSIX).  */ \
			"SIGURG",			/* 23: Urgent condition on socket (4.2 BSD).  */ \
			"SIGXCPU",			/* 24: CPU limit exceeded (4.2 BSD).  */ \
			"SIGXFSZ",			/* 25: File size limit exceeded (4.2 BSD).  */ \
			"SIGVTALRM",		/* 26: Virtual alarm clock (4.2 BSD).  */ \
			"SIGPROF",			/* 27: Profiling alarm clock (4.2 BSD).  */ \
			"SIGWINCH",			/* 28: Window size change (4.3 BSD, Sun).  */ \
			"SIGPOLL",			/* 29: Pollable event occurred (System V).  */ \
			"SIGPWR",			/* 30: Power failure restart (System V).  */ \
			"SIGSYS"			/* 31: Bad system call.  */				\
		};																\
		int g_tests_run = 0;											\
		int g_tests_success = 0;										\
		int g_asserts_run = 0;											\
		int g_asserts_success = 0;										\
		int g_suites_run = 0;											\
		int g_suites_success = 0;										\
		int g_success = TRUE;											\
		int g_status = 0;												\
		int g_exit = FALSE	/* the missing semi-colon is needed */		\

# define MU_KTHXBYE()													\
	do {																\
		printf("Total Suites:   %d/%d\n", g_suites_success, g_suites_run); \
		printf("Total Tests:    %d/%d\n", g_tests_success, g_tests_run); \
		printf("Total Asserts:  %d/%d\n", g_asserts_success, g_asserts_run); \
		if (g_asserts_success == g_asserts_run) {						\
			MU_PASS("\nAll tests passed!");								\
			exit(EXIT_SUCCESS);											\
		}																\
		printf(RED"Some test(s) failed :/\n"BASIC);						\
		exit(EXIT_FAILURE);												\
	} while (0)



# define MIN_SIG SIGHUP
# define MAX_SIG SIGSYS
# define MAX_SIG_STR 10

// the name of each signal by index, according to signum.h
extern char g_sig_str[MAX_SIG + 1][MAX_SIG_STR];

extern int g_tests_run;
extern int g_tests_success;

extern int g_asserts_run;
extern int g_asserts_success;

extern int g_suites_run;
extern int g_suites_success;

extern int g_success;
extern int g_status;
extern int g_exit;

#endif /* MINUNIT */
