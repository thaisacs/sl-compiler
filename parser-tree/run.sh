bison -d -o parser.c parser.y
flex -i -o scanner.c scanner.l
gcc -std=c99 -pedantic -o test_tree *.c

for n in `seq 1 1 1`
do
  ./test_tree < all/pr06.sl > teste
  #diff all/result0$n teste
  cat teste
  echo $n'------------------------------------'
  rm teste
done

#for n in `seq 10 1 27`
#do
#  ./test_tree < all/pr$n.sl > teste
#  #diff all/result$n teste
#  cat teste
#  echo $n'------------------------------------'
#  rm teste
#done
#
#for n in `seq 31 1 37`
#do
#  ./test_parser < all/pr$n.sl > teste
#  diff all/result$n teste
#  echo $n'------------------------------------'
#  rm teste
#done

#for n in `seq 41 1 43`
#do
#  ./test_parser < all/pr$n.sl > teste
#  diff all/result$n teste
#  echo $n'------------------------------------'
#  rm teste
#done
rm parser.c
rm parser.h
rm scanner.c
