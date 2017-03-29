#!/bin/bash -ex

SRC=minunit_example.c
EXE=testux
PP=testux.c
DUMMY=dummy
CC=clang

$CC -E $SRC -o $PP

# missing fun
touch $DUMMY.c

# moar fun
# echo "void ft_putstr(const char *s) { (void)s; }" > $DUMMY.c

$CC -c $DUMMY.c -o $DUMMY.o
ar -rcs lib$DUMMY.a $DUMMY.o

# could use that with shared libs (-shared -fPIC)
# $CC -Wall -Wextra -Wl,--unresolved-symbols=ignore-in-object-files $SRC -o $EXE

# or this with static ones
$CC -Wall -Wextra $SRC -Wl,--whole-archive lib$DUMMY.a -Wl,--no-whole-archive -o $EXE

rm -f $DUMMY.{c,o} lib$DUMMY.a
./$EXE
