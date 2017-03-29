/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minunit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcanal <zboub@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 11:32:38 by mcanal            #+#    #+#             */
/*   Updated: 2017/03/30 00:14:03 by mc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minunit.h"

void ft_putstr(const char *); //would be included from somewhere else
MU_DUMMY_FUN(ft_putstr);


static void test_foo()
{
	int foo = 7;

	MU_ASSERT("error, foo != 7", 7);
	MU_ASSERT("error, foo != 7", foo == 7);
	MU_ASSERT("error, foo != 7", foo == 7);
	for (int i = 1; i >= 0; i--)
		MU_ASSERT("error, 0 divide", foo / i);
}

static void test_bar()
{
	int bar = 4;

	MU_ASSERT("error, blabla", 5 == 5);
	MU_ASSERT("error, bar != 5", bar == 5);
	for (int i = 0; i < 10; i++)
		MU_ASSERT_FATAL("error, bar > i", bar > i);
}

static void test_missing()
{
	MU_PASS("cool stuff!");
	ft_putstr("héhé");
	MU_ASSERT("error, blabla", 5 == 5);
}

static void test_success()
{
	MU_ASSERT("error, blabla", 5 == 5);
	MU_ASSERT("error, blabla", 5 == 5);
	MU_ASSERT("error, blabla", 5 == 5);
	MU_ASSERT("error, blabla", 5 == 5);
}

static void all_tests()
{
	MU_TEST_SUITE suite_a = {
		{"test_missing", test_missing},
		{"test_foo", test_foo},
		{"test_bar", test_bar},
	};

	MU_TEST_SUITE suite_b = {
		{"test_success", test_success},
		{"test_success", test_success},
	};

	MU_RUN_SUITE("This is difficult", suite_a);
	MU_RUN_SUITE("but this is way easier", suite_b);
}


int main()
{
	MU_HAI();

	all_tests();

	MU_KTHXBYE();
	return EXIT_SUCCESS;
}
