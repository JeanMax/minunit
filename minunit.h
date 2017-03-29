/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <zboub@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 11:31:07 by mcanal            #+#    #+#             */
/*   Updated: 2017/03/30 01:34:48 by mc               ###   ########.fr       */
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
# define SIG_ASSERT_MSG(test, sig, msg) FAIL_MSG(test, sig" caught: ", msg)
# define SUCCESS_ASSERT_MSG         GREEN"."BASIC

# define MU_PASS(msg, ...) printf(GREEN msg "\n" BASIC, ##__VA_ARGS__) //TODO
# define MU_FAIL(msg, ...) fprintf(stderr, RED FAIL_MSG("", "Error: ", msg), ##__VA_ARGS__) //TODO
# define MU_FAIL_FATAL(msg, ...) MU_FAIL(msg, ##__VA_ARGS__), return  //TODO


# define HANDLE_SIG(test, msg, ...)										\
	do {																\
        if (wait(&g_status) != -1 && WIFSIGNALED(g_status))				\
        {																\
            switch(WTERMSIG(g_status))									\
            {															\
				case SIGSEGV:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGSEV", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGABRT:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGABRT", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGBUS:                                            \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGBUS", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGFPE:                                            \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGFPE", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGHUP:                                            \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGHUP", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGILL:                                            \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGILL", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGCHLD:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGCHLD", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGINT:                                            \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGINT", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGKILL:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGKILL", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGPIPE:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGPIPE", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGQUIT:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGQUIT", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGTERM:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGTERM", msg), ##__VA_ARGS__); \
					break;                                              \
				case SIGALRM:                                           \
				case SIGVTALRM:                                         \
				case SIGPROF:                                           \
					fprintf(stderr, SIG_ASSERT_MSG(test, "SIGALRM", msg), ##__VA_ARGS__); \
					break;                                              \
				default:                                                \
					fprintf(stderr, SIG_ASSERT_MSG(test, "???", msg), ##__VA_ARGS__);	\
            }															\
        }																\
	} while (0)


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
            HANDLE_SIG(test, msg, ##__VA_ARGS__);							\
			if (WIFEXITED(g_status) && WEXITSTATUS(g_status) == EXIT_SUCCESS) \
				g_asserts_success++;									\
			else														\
				g_success = FALSE;										\
			g_asserts_run++;											\
        }																\
	} while (0)

# define MU_ASSERT_FATAL(test, msg, ...)								\
	do {																\
		MU_ASSERT(test, msg, ##__VA_ARGS__);								\
		if (!WIFEXITED(g_status) || WEXITSTATUS(g_status) != EXIT_SUCCESS) \
			return ;													\
	} while (0) //TODO

# define MU_RUN_TEST(test)						\
	do {										\
		g_success = TRUE;						\
		test();									\
		g_tests_run++;							\
		if (g_success)							\
			g_tests_success++;					\
		printf("\n");							\
	} while (0)

# define MU_RUN_SUITE(suite, name)										\
	do {                                                                \
        printf("+ Suite "name":\n");									\
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
		if (g_asserts_success - asserts_success == g_asserts_run - asserts_run)	\
			printf(GREEN"Suite passed."BASIC"\n\n"), g_suites_success++; \
		else															\
			printf(RED"Suite failed."BASIC"\n\n");						\
		g_suites_run++;													\
    } while (0)

#define MU_DUMMY_FUN(x)													\
	void  __attribute__((weak)) x()										\
	{																	\
		fprintf(stderr, WHITE"\n"AT": "RED"Error: "WHITE"missing function"BASIC"\n\t"#x"()\n\t"GREEN"^\n"BASIC"\n"); \
		g_success = FALSE;												\
		g_exit = TRUE;													\
	}



# define MU_HAI()								\
	do {										\
		setbuf(stdout, NULL);					\
		g_tests_run = 0;						\
		g_tests_success = 0;					\
		g_asserts_run = 0;						\
		g_asserts_success = 0;					\
		g_suites_run = 0;						\
		g_suites_success = 0;					\
		g_success = TRUE;						\
		g_status = 0;							\
		g_exit = FALSE;							\
	} while (0)

# define MU_KTHXBYE()													\
	do {																\
		printf("Total Suites:   %d/%d\n", g_suites_success, g_suites_run); \
		printf("Total Tests:    %d/%d\n", g_tests_success, g_tests_run); \
		printf("Total Asserts:  %d/%d\n", g_asserts_success, g_asserts_run); \
		if (g_asserts_success == g_asserts_run)							\
			MU_PASS("\nAll tests passed!"), exit(EXIT_SUCCESS);			\
		printf(RED"Some test(s) failed :/\n"BASIC);						\
		exit(EXIT_FAILURE);												\
	} while (0)



int g_tests_run;
int g_tests_success;

int g_asserts_run;
int g_asserts_success;

int g_suites_run;
int g_suites_success;

int g_success;
int g_status;
int g_exit;

#endif /* MINUNIT */
