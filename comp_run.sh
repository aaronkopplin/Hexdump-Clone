clang -Wall hexdump.c

file="hello.txt"

./a.out $file > actual

hexdump -v $file > expected

clear

# cat actual
# echo --------------------------------------------------------------------
# cat expected

diff -w actual expected
