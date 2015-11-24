set terminal svg enhanced size 1000 1000 fname "Times" fsize 36
set output "ci.svg"
set title "A simple plot of random integer co-ordinates"
set xlabel "x"
set ylabel "y"
set xrange [0:]
set yrange [0:]
plot "./ci.dat" using 1:2 title ""
