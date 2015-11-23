set terminal svg enhanced size 1000 1000 fname "Times" fsize 36
set output "ci.svg"
set title "A simple plot of random integer co-ordinates"
set xlabel "x"
set ylabel "y"
plot "./cp.dat" using 1:2 title ""
