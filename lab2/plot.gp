set terminal png truecolor
set output "histogram.png"
set grid
set xtic rotate
set style fill solid 0.66
set boxwidth 0.50
set datafile separator " "

DATAFILE = data_file

# Define a function that maps a number to a bin:
#  startx, endx: expected interval of values for x
#  n: number of bins
bin(x, startx, endx, n) = (x > endx)? n - 1 : ((x < startx)? 0 : floor(n * ((x - startx) / (endx - startx))))

# Define a function to map bin back to a real value
start_of_bin(i, startx, endx, n) = startx + i * ((endx - startx) / n) 

# Get file stats
stats data_file using 1 name 'x' nooutput

N = 100 # number of bins
START = x_min # start of range
END = x_max # end of range

# Configure x-axis
set xrange [0:N]
# Increments of sqrt(number of bins) look optimal
set for [i=0:N:+N**(0.5)] xtics (sprintf("%d", start_of_bin(i, START, END, N)) i)

# Plot histogram: map (multiple times) every bin to 1.0. 
# Must use smooth freq to actually count frequencies (see `help smooth freq`) !!
plot data_file using (bin($1, START, END, N)):2 with boxes title "Wav distribution"