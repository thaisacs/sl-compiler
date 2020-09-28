flex -i -o scanner.c scanner.l
gcc -std=c99 -pedantic -o test_scanner *.c

for n in `seq 1 1 9`
do
  ./test_scanner < all/pr0$n.sl > teste
  diff all/tokens0$n.res teste
  rm teste
done

for n in `seq 10 1 27`
do
  ./test_scanner < all/pr$n.sl > teste
  diff all/tokens$n.res teste
  rm teste
done
