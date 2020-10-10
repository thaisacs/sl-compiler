bison -d -o parser.c parser.y
flex -i -o scanner.c scanner.l
gcc -std=c99 -pedantic -o test_parser *.c

for n in `seq 1 1 9`
do
  ./test_parser < all/pr0$n.sl > teste
  diff all/result0$n teste
  echo $n'------------------------------------'
  rm teste
done

for n in `seq 10 1 27`
do
  ./test_parser < all/pr$n.sl > teste
  diff all/result$n teste
  echo $n'------------------------------------'
  rm teste
done

rm parser.c
rm parser.h
rm scanner.c
