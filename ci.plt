set terminal svg enhanced size 2000 2000 fname "Times" fsize 36
set output "ci.svg"
set title "A plot of closest pair of random integer co-ordinates"
set xlabel "x"
set ylabel "y"
set xrange [0:]
set yrange [0:]
set xtics 10
set ytics 10
plot "./ci.dat" using 1:2 w p pt 12 ps 0.8 lt 7 ti '', \
    "./cp_ci.dat" using 1:2 w p pt 12 ps 0.8 lt 1 ti ''
#plot "./ci.dat" using 1:2 title ""
