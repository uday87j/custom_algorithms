#$CLANG -std=c++11 -g nurikabe.cpp board.cpp test.cpp -o nurik

CFLAGS="-std=c++11 "
DFLAGS=$CFLAGS"-g"
RFLAGS=$CFLAGS"-O3"
PFLAGS=$CFLAGS"-g -pg"
CFLAGS=$RFLAGS
SRC="nurikabe.cpp board.cpp test.cpp"

echo "There are $# args"
if [ $# > 1 ];then
    if [ "$1" = "-d" ];then
        CFLAGS=$DFLAGS
    elif [ "$1" = "-p" ];then
        CFLAGS=$PFLAGS
    fi
fi

echo "Source files: $SRC"
echo "CFLAGS = $CFLAGS"

$CLANG $CFLAGS $SRC -o nurik
