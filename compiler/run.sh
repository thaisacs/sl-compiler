bison -d -o parser.c parser.y
flex -i -o scanner.c  scanner.l
gcc -I include/ -std=c99 -pedantic -o slc source/*.c *.c

mv parser.h include
mv parser.c source
mv scanner.c source

./slc  < all/pr08.sl

#for n in `seq 1 1 9`
#do
#  echo $n'------------------------------------'
#  ./slc  < all/pr0$n.sl > results/pr0$n.mep
#  diff all/pr0$n.mep results/pr0$n.mep
#  echo $n'------------------------------------'
#done

#for n in `seq 10 1 27`
#do
#  echo $n'------------------------------------'
#  ./slc  < all/pr$n.sl > results/pr$n.mep
#  diff all/pr$n.mep results/pr$n.mep
#  echo $n'------------------------------------'
#done

#for n in `seq 31 1 37`
#do
#  ./slc  < all/pr$n.sl
##  ./test_tree < all/pr$n.sl > teste
##  diff all/result$n teste
##  echo $n'------------------------------------'
##  rm teste
#done
#
#for n in `seq 42 1 43`
#do
#  ./slc  < all/pr$n.sl
##  ./test_tree < all/pr$n.sl > teste
##  diff all/result$n teste
##  echo $n'------------------------------------'
##  rm teste
#done

rm source/parser.c
rm include/parser.h
rm source/scanner.c
