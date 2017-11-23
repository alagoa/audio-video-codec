set terminal png truecolor
set output "histogram.png"
set grid
set style fill transparent solid 0.5 noborder
set boxwidth 0.50
set datafile separator " "
set xtics rotate
set style line 100 lt 1 lc rgb "white" lw 2
set style line 101 lt 0.5 lc rgb "white" lw 1
set grid mytics ytics ls 100, ls 101
set grid mxtics xtics ls 100, ls 101

DATAFILE = "histogram.dat"

# Define a function that maps a number to a bin:
#  startx, endx: expected interval of values for x
#  n: number of bins
bin(x, startx, endx, n) = (x > endx)? n - 1 : ((x < startx)? 0 : floor(n * ((x - startx) / (endx - startx))))

# Define a function to map bin back to a real value
start_of_bin(i, startx, endx, n) = startx + i * ((endx - startx) / n) 

# Get file stats
stats DATAFILE using 1 name 'x' nooutput

N = x_records # number of bins
START = x_min # start of range
END = x_max # end of range

# Configure x-axis
set xrange [0:N]
set for [i=0:N:+1000] xtics (sprintf("%d", start_of_bin(i, START, END, N)) i)

# Plot histogram: map (multiple times) every bin to 1.0. 
# Must use smooth freq to actually count frequencies (see `help smooth freq`) !!
plot DATAFILE using (bin($1, START, END, N)):2 smooth freq with boxes lt rgb "#406090" title "channel 0", \
	"" using (bin($1, START, END, N)):3 smooth freq with boxes lt rgb "#40FF00" title "channel 1", \
	"" using (bin($1, START, END, N)):4 smooth freq with boxes lt rgb "#D85040" title "channel mono", \
