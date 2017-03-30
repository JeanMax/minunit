/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minunit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <zboub@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 11:32:38 by mcanal            #+#    #+#             */
/*   Updated: 2017/03/30 23:27:03 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minunit.h"

MU_DUMMY_FUN(ft_putstr);

//clang throw a warning without the proto, gcc throw an error with it :/
void ft_putstr(const char *); //would be included from somewhere else

static void test_foo()
{
	int foo = 7;

	MU_ASSERT(7, "foo != 7");
	MU_ASSERT(foo == 7, "foo != 7");
	MU_ASSERT(foo == 7, "foo != 7");
	for (int i = 1; i >= 0; i--)
		MU_ASSERT(foo / i, "foo / %d", i);
}

static void test_bar()
{
	int bar = 4;

	MU_ASSERT(5 == 5, "blabla");
	MU_ASSERT(bar == 5, "bar != 5");
	for (int i = 0; i < 10; i++)
		MU_ASSERT_FATAL(bar > i, "bar > %d", i);
}

static void test_missing()
{
	MU_PASS("cool stuff!");
	ft_putstr("héhé");
	MU_ASSERT(5 == 5, "blabla");
}

static void test_success()
{
	MU_ASSERT(5 == 5, "blabla");
	MU_ASSERT(5 == 5, "blabla");
	MU_ASSERT(5 == 5, "blabla");
	MU_ASSERT(5 == 5, "blabla");
}

static void all_tests()
{
	MU_TEST_SUITE suite_a = {
		{test_missing, "testing missing"},
		{test_foo, "testing foo"},
		{test_bar, "testing bar"},
	};

	MU_TEST_SUITE suite_b = {
		{test_success, "testing success"},
		{test_success, "testing success"},
	};

	MU_RUN_SUITE(suite_a, "This is difficult");
	MU_RUN_SUITE(suite_b, "but this is way easier");
}


MU_HAI(); //yep, this is really ugly :/

int main()
{

	all_tests();

	MU_KTHXBYE();
	return EXIT_SUCCESS;
}
