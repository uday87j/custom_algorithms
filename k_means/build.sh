#$CLANG -std=c++11 -g nurikabe.cpp board.cpp test.cpp -o nurik

CFLAGS="-std=c++11 "
DFLAGS=$CFLAGS"-g -Wall -Werror"
RFLAGS=$CFLAGS"-O3"
PFLAGS=$CFLAGS"-g -pg"
CFLAGS=$RFLAGS

SRC="test_kmeans.cpp"

INC="-I ../"

EX="kmeans"
PEX="kmeans_valgrind"

echo "There are $# args"
if [ $# > 1 ];then
    if [ "$1" = "-d" ];then
        CFLAGS=$DFLAGS
    elif [ "$1" = "-p" ];then
        CFLAGS=$PFLAGS
        EX=$PEX
    fi
fi

echo "Source files: $SRC"
echo "CFLAGS = $CFLAGS"
echo "CLANG = $CLANG"

$CLANG $CFLAGS $INC $SRC -o $EX
