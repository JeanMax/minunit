/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <zboub@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 11:31:07 by mcanal            #+#    #+#             */
/*   Updated: 2017/03/30 00:02:11 by mc               ###   ########.fr       */
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
	char s[MAX_NAME_SIZE + 1];
	MU_TEST_FUN f;
};
typedef struct s_fun  t_fun;

typedef t_fun MU_TEST_SUITE[];


# define WRITE(x, y, z) (void)(write(x, y, z) + 1)

/* #  define PRINT(str, ...) printf((str), __VA_ARGS__) */
/* #  define PRINT_ERR(str, ...) fprintf(stderr, str, __VA_ARGS__) */
# define PRINT(x) WRITE(STDIN_FILENO, x, strlen(x))
# define PRINT_ERR(x) WRITE(STDERR_FILENO, x, strlen(x))

/* #  define PUTS(str, ...) printf(str"\n", __VA_ARGS__) */
/* #  define PUTS_ERR(str, ...) fprintf(stderr, str"\n", __VA_ARGS__) */
# define PUTS(x) PRINT(x), WRITE(STDIN_FILENO, "\n", 1)
# define PUTS_ERR(x) PRINT_ERR(x), WRITE(STDERR_FILENO, "\n", 1)

# define STRINGIFY(x) #x
# define TOSTRING(x)  STRINGIFY(x)
# define AT           __FILE__":"TOSTRING(__LINE__)



# define FAIL_MSG(msg, test, type) WHITE"\n"AT": "RED type WHITE msg	\
	BASIC"\n\t("#test")\n\t "GREEN"^\n"BASIC
# define FAIL_ASSERT_MSG(msg, test) FAIL_MSG(msg, test, "Assertion failed: ")
# define SIG_ASSERT_MSG(msg, test, sig) FAIL_MSG(msg, test, sig" caught: ")
# define SUCCESS_ASSERT_MSG         GREEN"."BASIC

# define MU_PASS(msg)       PRINT(GREEN), PRINT(msg), PRINT("\n"BASIC) //TODO
# define MU_FAIL(msg)       PRINT_ERR(RED), FAIL_MSG(msg, "", "Error: ") //TODO
# define MU_FAIL_FATAL(msg) MU_FAIL(msg), return  //TODO


# define HANDLE_SIG(msg, test)                                      \
	do {                                                            \
        if (wait(&g_status) != -1 && WIFSIGNALED(g_status))			\
        {															\
            switch(WTERMSIG(g_status))								\
            {														\
            case SIGSEGV:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGSEV"));     \
                break;                                              \
            case SIGABRT:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGABRT"));    \
                break;                                              \
            case SIGBUS:                                            \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGBUS"));     \
                break;                                              \
            case SIGFPE:                                            \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGFPE"));     \
                break;                                              \
            case SIGHUP:                                            \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGHUP"));     \
                break;                                              \
            case SIGILL:                                            \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGILL"));     \
                break;                                              \
            case SIGCHLD:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGCHLD"));    \
                break;                                              \
            case SIGINT:                                            \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGINT"));     \
                break;                                              \
            case SIGKILL:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGKILL"));    \
                break;                                              \
            case SIGPIPE:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGPIPE"));    \
                break;                                              \
            case SIGQUIT:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGQUIT"));    \
                break;                                              \
            case SIGTERM:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGTERM"));    \
                break;                                              \
            case SIGALRM:                                           \
            case SIGVTALRM:                                         \
            case SIGPROF:                                           \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "SIGALRM"));    \
                break;                                              \
            default:                                                \
                PRINT_ERR(SIG_ASSERT_MSG(msg, test, "???"));        \
            }														\
        }															\
	} while (0)


# define MU_ASSERT(msg, test)											\
	do {																\
		if (g_exit) {													\
			g_exit = FALSE;												\
			return ;													\
		}																\
		if (!fork()) {													\
            alarm(3);													\
			if ((test)) {												\
				PRINT(SUCCESS_ASSERT_MSG);								\
				exit(EXIT_SUCCESS);										\
			}															\
			PRINT_ERR(FAIL_ASSERT_MSG(msg, test));						\
            exit(EXIT_FAILURE);											\
        } else {														\
            HANDLE_SIG(msg, test);										\
			if (WIFEXITED(g_status) && WEXITSTATUS(g_status) == EXIT_SUCCESS) \
				g_asserts_success++;									\
			else														\
				g_success = FALSE;										\
			g_asserts_run++;											\
        }																\
	} while (0)

# define MU_ASSERT_FATAL(msg, test)										\
	do {																\
		MU_ASSERT(msg, test);											\
		if (!WIFEXITED(g_status) || WEXITSTATUS(g_status) != EXIT_SUCCESS) \
			return ;												\
	} while (0) //TODO

# define MU_RUN_TEST(test)						\
	do {										\
		g_success = TRUE;						\
		test();									\
		g_tests_run++;							\
		if (g_success)							\
			g_tests_success++;					\
		PUTS("");								\
	} while (0)

# define MU_RUN_SUITE(name, suite)                                      \
	do {                                                                \
        PUTS("+ Running suite "name":");								\
		int tests_run = g_tests_run;									\
		int tests_success = g_tests_success;							\
		int asserts_run = g_asserts_run;								\
		int asserts_success = g_asserts_success;						\
        for (size_t ii = 0; ii < sizeof(suite) / sizeof(t_fun); ii++) {	\
			printf("-%s: ", suite[ii].s);								\
            MU_RUN_TEST(suite[ii].f);									\
		}																\
		printf("Tests:    %d/%d\n", g_tests_success - tests_success , g_tests_run - tests_run); \
		printf("Asserts:  %d/%d\n", g_asserts_success - asserts_success, g_asserts_run - asserts_run); \
		if (g_asserts_success - asserts_success == g_asserts_run - asserts_run)	\
			MU_PASS("Suite passed!\n");									\
		else															\
			PUTS("");													\
    } while (0)

#define MU_DUMMY_FUN(x)											\
	void  __attribute__((weak)) x()										\
	{																	\
		PUTS_ERR(WHITE"\n"AT": "RED"Error: "WHITE"missing function"BASIC"\n\t"#x"()\n\t"GREEN"^\n"BASIC); \
		g_success = FALSE;												\
		g_exit = TRUE;													\
	}



# define MU_HAI()									\
	do {											\
		setbuf(stdout, NULL);						\
		g_tests_run = 0;							\
		g_tests_success = 0;						\
		g_asserts_run = 0;							\
		g_asserts_success = 0;						\
		g_success = TRUE;							\
		g_status = 0;								\
		g_exit = FALSE;								\
	} while (0)

# define MU_KTHXBYE()													\
	do {																\
		printf("Total Tests:    %d/%d\n", g_tests_success, g_tests_run); \
		printf("Total Asserts:  %d/%d\n", g_asserts_success, g_asserts_run); \
		if (g_asserts_success == g_asserts_run)							\
			MU_PASS("\nAll tests passed!"), exit(EXIT_SUCCESS);			\
		PUTS(RED"Some test(s) failed :/"BASIC);							\
		exit(EXIT_FAILURE);												\
	} while (0)



int g_tests_run;
int g_tests_success;

int g_asserts_run;
int g_asserts_success;

int g_success;
int g_status;
int g_exit;

#endif /* MINUNIT */
