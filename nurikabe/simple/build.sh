#$CLANG -std=c++11 -g nurikabe.cpp board.cpp test.cpp -o nurik
CFLAGS="-g -std=c++11"
RFLAGS="-O3 -std=c++11"
SRC="nurikabe.cpp board.cpp test.cpp"
$CLANG $RFLAGS $SRC -o nurik
