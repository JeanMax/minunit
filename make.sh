#!/bin/bash -e

SRC=minunit_example.c
EXE=testux
PP=preprocessed_example.c
DUMMY=dummy
test $CC || CC=clang

# creating a 'pretty' output of file after preprocessing
$CC -E $SRC | grep -A1000 'MU_TEST_FUN' | sed 's!do {\|} while (0);!!g' | sed 's!\([{};]\) !\1\n!g' > $PP
emacs --daemon=zboub $PP --eval '
(progn
  (mark-whole-buffer)
  (indent-region (region-beginning) (region-end))
  (save-buffer)
  (kill-emacs))' &>/dev/null &


# missing fun
touch $DUMMY.c

# or with fun
# echo "void ft_putstr(const char *s) { (void)s; }" > $DUMMY.c

$CC -c $DUMMY.c -o $DUMMY.o
ar -rcs lib$DUMMY.a $DUMMY.o


# could use that with shared libs (-shared -fPIC)
# $CC -Wall -Wextra -Wl,--unresolved-symbols=ignore-in-object-files $SRC -o $EXE

# or this with static ones
$CC -Wall -Wextra $SRC -Wl,--whole-archive lib$DUMMY.a -Wl,--no-whole-archive -o $EXE

rm -f $DUMMY.{c,o} lib$DUMMY.a
./$EXE
